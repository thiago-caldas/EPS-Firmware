#include "inc/main.h"

stMEASURES stMeasures;

void SENSORS_answerRequest( void )
{
   /* Instrucoes enviadas pelo computador de bordo
    *
    *  0x00 - Ler tensao do painel
    *  0x01 - Ler tensao da bateria
    *  0x02 - Ler tensao da carga
    *  0x03 - Ler corrente do painel
    *  0x04 - Ler corrente da bateria
    *  0x05 - Ler corrente da carga
    *
    */

    uint16_t data;

    if (stI2CSlave.rxBuff[0] == 0x00)       data = SENSORS_convertValueToSend( stMeasures.panelVoltage );
    else if (stI2CSlave.rxBuff[0] == 0x01)  data = SENSORS_convertValueToSend( stMeasures.battVoltage  );
    else if (stI2CSlave.rxBuff[0] == 0x02)  data = SENSORS_convertValueToSend( stMeasures.loadVoltage  );
    else if (stI2CSlave.rxBuff[0] == 0x03)  data = SENSORS_convertValueToSend( stMeasures.panelCurrent );
    else if (stI2CSlave.rxBuff[0] == 0x04)  data = SENSORS_convertValueToSend( stMeasures.battCurrent  );
    else if (stI2CSlave.rxBuff[0] == 0x05)  data = SENSORS_convertValueToSend( stMeasures.loadCurrent  );

    SENSORS_sendAnswer(data);
}

void SENSORS_configIna( void )
{

    SENSORS_sendByteToIna(CONFIG_REG_ADDR, 0x8000);        // 0x8000 -> Reseta os registradores

    I2C_masterRead (INA_ADDR, 2);                   // Verifica o reset

    /* Configuracao
     *
     * 0 111 011 100 100 101 -> 0x7725
     *
     * Reset = 0 -> 0
     * Habilita os 3 canais -> 111
     * Configura 64 leituras -> 011
     * Configura tempo de conversao de VBus para 1.1 ms -> 100
     * Configura tempo de conversao de VShunt para 1.1 ms -> 100
     * Configura modo de operacao para ler somente Vshunt de forma continua -> 101
     *
     * */

    SENSORS_sendByteToIna(CONFIG_REG_ADDR, 0x7725);        // 0x7725 -> Configuracao do INA

    I2C_masterRead (INA_ADDR, 2);                   // Verifica a configuracao
}

void SENSORS_readCurrent( uint8_t regAddr )
{
    uint16_t aux;
    float floatAux;

    I2C_clearMasterRxBuff();

    I2C_masterWrite (INA_ADDR, &regAddr, 1);   // Configura o registrador que sera lido pelo INA
    I2C_masterRead (INA_ADDR, 2);              // Le as medidas realizadas pelo sensor

    aux = stI2CMaster.rxBuff[0];

    aux = aux << 8;

    aux += (stI2CMaster.rxBuff[1] & 0xFF);

    floatAux = SENSORS_convertInaValue(aux);

    if(regAddr == PANEL_CURRENT_ADDR ) stMeasures.panelCurrent = floatAux;
    else if(regAddr == BATT_CURRENT_ADDR ) stMeasures.battCurrent =  floatAux;
    else if(regAddr == LOAD_CURRENT_ADDR ) stMeasures.loadCurrent =  floatAux;
}

float SENSORS_convertInaValue( uint16_t inaValue )
{
    /* Conversao conforme item 8.6.2.2 na pag. 27 do datasheet do INA3221 */

    uint8_t signal;
    uint16_t aux;
    float current;

    signal = (uint8_t) ((inaValue & 0x8000) >> 15);     // Pega  bit mais significativo como sinal

    if (signal)                                    // Se for negativo, calcula a partir do complemento de dois
    {
        aux = inaValue;
        aux &= 0x7FFF;                             // Zera o bit mais significativo
        aux -= 1;                                  // Subtrai 1
        aux ^= 0x7FFF;                             // Inverte os bits
        aux >>= 3;                                 // Desloca 3 bits para direita

        current = (float) aux;
        current *= 40;                             // Multiplica por 40 uV para obter a tensao no R shunt, em V
        current /= 1000000;                        //
        current /= 10;                             // Divide por R shunt para obter o valor de corrente, em A
        current *= 1000;                           // Converte para mA

        current *= 100;                            // Ajuste empirico para ler valor medido pelo multimetro

        if(signal) current *= -1;                  // Adequa o sinal da corrente
    }
    else
    {
        aux = inaValue;
        aux >>= 3;                                 // Desloca 3 bits para direita

        current = (float) aux;
        current *= 40;                             // Multiplica por 40 uV para obter a tensao no R shunt, em V
        current /= 1000000;                        //
        current /= 10;                             // Divide por R shunt para obter o valor de corrente, em A
        current *= 1000;                           // Converte para mA

        current *= 100;                            // Ajuste empirico para ler valor medido pelo multimetro
    }
    return current;
}

void SENSORS_convertFtoStr(float *data, char *string)
{
    int intN, decN;

    intN = (int) *data;
    decN = (int) ( (*data - intN) * 100);

    if(decN < 0) decN = (-1)*decN;

    sprintf(string, "%d.%d\0", intN, decN);
}

void SENSORS_sendByteToIna( uint8_t regAddr, uint16_t byte )
{
    /* Monta o dado para enviar primeiro o endereco do registro e depois o dado,
     * enviando primeiro o MSB e depois o LSB.
     */

    uint32_t data = 0;
    uint8_t MSB, LSB;

    LSB = (uint8_t) byte;
    MSB = (uint8_t) (byte >> 8);

    /* 0x 00 | LSB | MSB | regAddr */

    data = LSB;
    data = data << 8;
    data |= MSB;
    data = data << 8;
    data |= regAddr;

    I2C_masterWrite (INA_ADDR, (uint8_t *) &data , 3);
}

uint16_t SENSORS_convertValueToSend( float value )
{
    uint16_t convertedValue, aux;

    value *= 100;                                   // Multiplica por 100 para enviar um valor inteiro

    if (value > 32767 || value < -32768) return 0;  // Codigo de erro

    if (value > 0) convertedValue = (uint16_t) value;

    else                                    // Se for negativo, calcula o complemento de dois
    {
        value *= -1;
        aux = value;
        aux ^= 0xFFFF;                             // Inverte os bits
        aux += 1;                                  // Soma 1
        convertedValue = aux;
    }

    return convertedValue;
}

float SENSORS_convertReceivedValue( uint16_t value )
{
    float convertedValue;
    uint8_t signal;
    uint16_t aux;

    signal = (uint8_t) ((value & 0x8000) >> 15);     // Pega  bit mais significativo como sinal

    if (signal)                                      // Se for negativo, calcula a partir do complemento de dois
    {
        aux = value;
        aux -= 1;                                    // Subtrai 1
        aux ^= 0xFFFF;                               // Inverte os bits
        convertedValue = aux;
        convertedValue *= -1;
    }
    else convertedValue = value;

    convertedValue /= 100;                           // Divide por 100 para obter o valor decimal

    return convertedValue;
}

void SENSORS_sendAnswer( uint16_t byte )
{
    /* Monta o dado para enviar primeiro o MSB e depois o LSB */

    uint16_t data = 0;
    uint8_t MSB, LSB;

    LSB = (uint8_t) byte;
    MSB = (uint8_t) (byte >> 8);

    /* 0x LSB | MSB */

    data = LSB;
    data = data << 8;
    data |= MSB;

    I2C_slaveWrite((uint8_t *) &data, 2);
}
