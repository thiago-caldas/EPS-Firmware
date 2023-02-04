#include "inc/main.h"

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    switch(__even_in_range(UCA0IV,18)) {
    case 0x02:                                    // Vector 2: UCRXIFG
        stUart.rxBuff[stUart.rxIndex] = UCA0RXBUF;

        if(stUart.rxIndex < SIZE_UART_RX_BUFF-1) stUart.rxIndex++;
    break;
    case 0x04:                                    // Vector 4: UCTXIFG
        if( stUart.bytesToSend ){
            stUart.bytesToSend--;

            UCA0TXBUF = stUart.txBuff[stUart.txIndex++];

        }else{
            UCA0IE &= ~UCTXIE;
        }
    break;
    }
}
#pragma vector = TIMER0_A0_VECTOR;
__interrupt void TIMER0_A0_ISR(){
    uwTick++;
}

#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12_ISR( void ){
    unsigned int volatile *ptrAdcMem;

    uint16_t sum = 0;

    int i;

    /*                               Medida da temperatura                               */


    ptrAdcMem = &ADC12MEM0;               // Inicia lendo o valor do sensor de temperatura

    for( i = 8; i; i-- )
    sum += *ptrAdcMem++;

    stMeasures.battTemp = sum/8;       // Divide por 8 para tirar a media

    /* Multiplica por 3 e divide por 4096 para converter para tensao (em V)
     * Cada 10 mV corresponde a 1 °C - LM35 -> Multiplicacao por 100           */

    stMeasures.battTemp = (stMeasures.battTemp * 3.3 * 100) /4096;


    /*                               Medida da tensao do painel                           */


    ptrAdcMem = &ADC12MEM8;              // Le o valor da tensao do painel

    sum = 0;
    for( i = 8; i; i-- )
    sum += *ptrAdcMem++;

    stMeasures.panelVoltage = sum/8;  // Divide por 8 para tirar a media
    stMeasures.panelVoltage = stMeasures.panelVoltage * 3.3 / 4096;  // Converte para tensao
    stMeasures.panelVoltage *= 4;  // Compensa o divisor resistivo da entrada


    /*                               Medida da tensao da bateria                          */


    ptrAdcMem = &ADC12MEM16;             // Le o valor da tensao da bateria

    sum = 0;
    for( i = 8; i; i-- )
    sum += *ptrAdcMem++;

    stMeasures.battVoltage = sum/8;  // Divide por 8 para tirar a media
    stMeasures.battVoltage = stMeasures.battVoltage * 3.3 / 4096;  // Converte para tensao
    stMeasures.battVoltage *= 2;  // Compensa o divisor resistivo da entrada


    /*                               Medida da tensao da carga                           */


    ptrAdcMem = &ADC12MEM24;             // Le o valor da tensao da carga

    sum = 0;
    for( i = 8; i; i-- )
    sum += *ptrAdcMem++;

    stMeasures.loadVoltage = sum/8;  // Divide por 8 para tirar a media
    stMeasures.loadVoltage = stMeasures.loadVoltage * 3.3 / 4096;  // Converte para tensao
    stMeasures.loadVoltage *= 2;  // Compensa o divisor resistivo da entrada

    ADC12CTL0 |= ADC12ENC;                 // Habilita o trigger
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_A0_ISR(void){
    ADC12CTL0 &= ~ADC12SC;                  // Gera um flanco de subida no bit SC
    ADC12CTL0 |= ADC12SC;                   //

    TA1CCR0 += 100-1;
}

#pragma vector = USCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{
    switch(__even_in_range(UCB2IV,30))
    {
        case USCI_I2C_UCRXIFG0:
            if (stI2CSlave.rxIndex < SIZE_I2C_RX_BUFF) stI2CSlave.rxBuff[stI2CSlave.rxIndex++] = UCB2RXBUF;
            break;
        case USCI_I2C_UCTXIFG0:
            if ( !stI2CSlave.sendingData )
            {
                SENSORS_answerRequest();
                stI2CSlave.sendingData = 1;
            }
            if( stI2CSlave.bytesToSend ){
                stI2CSlave.bytesToSend--;

                UCB2TXBUF = stI2CSlave.txBuff[stI2CSlave.txIndex++];

            }else{
                stI2CSlave.rxIndex = 0;
                memset( stI2CSlave.rxBuff, 0, SIZE_I2C_RX_BUFF );
                stI2CSlave.sendingData = 0;
            }
            break;
    }
}
