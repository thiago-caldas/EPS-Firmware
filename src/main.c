#include "inc/main.h"

uint32_t uwTick = 0;

int main(void)
{
    PM5CTL0 &= ~LOCKLPM5;

	WDT_refresh();

    MAIN_mainInit();

    POWER_initSubsystemsPowerStatus();

    I2C_configMaster(100000);
    I2C_configSlave(0x34);

    INTERFACE_clearTerminal();

    SENSORS_configIna();

    stApplication.tmt = MAIN_getTick();
    APP_applicationRun();
}

void MAIN_gpioInit( void ){

    /* Configuracao da funcionalidade dos pinos usados para UART */

    P2SEL0 &= ~(BIT0 | BIT1);
    P2SEL1 |=  (BIT0 | BIT1);

    /* Configuracao da funcionalidade dos pinos usados para I2C B1 */

    P5SEL0 |=  (BIT0|BIT1);                 // P5SEL = 01
    P5SEL1 &= ~(BIT0|BIT1);                 // P5.0 = SDA, P5.1 = SCL
    P5DIR  &= ~(BIT0|BIT1);
    P5REN  |=  (BIT0|BIT1);
    P5OUT  |=  (BIT0|BIT1);

    /* Configuracao da funcionalidade dos pinos usados para I2C B2 */

    P7SEL0 |=  (BIT0|BIT1);                 // P7SEL = 01
    P7SEL1 &= ~(BIT0|BIT1);                 // P7.0 = SDA, P7.1 = SCL
    P7DIR  &= ~(BIT0|BIT1);
    P7REN  |=  (BIT0|BIT1);
    P7OUT  |=  (BIT0|BIT1);

    /* Configuracao de GPIOS que controlam a alimentacao dos subsistemas */

    /* P3.4 - Subsistema de comunicacao
     * P3.5 - Subsistema de controle e determinacao de atitude
     * P3.6 - Subsistema de comando
     * P3.7 - Subsistema de controle de temperatura
     * */

    P3DIR |= ( COMMUNICATION_POWER |
               ATTITUDE_POWER      |
               COMMAND_POWER       |
               THERMAL_POWER       );

    P3OUT &= ~(COMMUNICATION_POWER |
               ATTITUDE_POWER      |
               COMMAND_POWER       |
               THERMAL_POWER       );

    /* Configuracao da funcionalidade dos pinos de leitura do ADC */

    /* P3.0 - Sensor de temperatura
     * P3.1 - Tensao do painel
     * P3.2 - Tensao da bateria
     * P3.3 - Tensao da carga
     * */

    P3SEL0 |= (TEMP_SENSOR   |
               PANEL_VOLTAGE |
               BATT_VOLTAGE  |
               LOAD_VOLTAGE  );

    P3SEL1 |= (TEMP_SENSOR   |
               PANEL_VOLTAGE |
               BATT_VOLTAGE  |
               LOAD_VOLTAGE  );

    /* Configuracao do GPIO utilizado para o aquecedor da bateria  - P1.3 */

    P1DIR |= BATTERY_HEATER;
    P1OUT &= ~BATTERY_HEATER;
}

void MAIN_timerInit( void ){
    /* Configuracao do timer para contagem do tick */

    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;

    TA0CCR0 = 1000-1;             // Periodo de 1 ms
                                  // 1 ms -> 0.001 = 1/10^6 * (N+1)
                                  // (N+1) = 1000
    TA0CCTL0 = CCIE;

    /* Configuracao do timer para amostragem do ADC */

    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS | TACLR;

    TA1CCR0 = 100-1;             // Periodo de 100 us
                                 // 100 us -> 0.0001 = 1/10^6 * (N+1)
                                 // (N+1) = 100
    TA1CCTL0 = CCIE;
}

void MAIN_adcInit( void ){

    ADC12CTL0 = 0;                          // Zera o bit de ENC
    ADC12CTL0 = ADC12SHT0_2       |         // SHT = 16 batidas de clock
                ADC12SHT1_2       |         // SHT = 16 batidas de clock
                ADC12ON;                    // Liga o Conversor AD

    ADC12CTL1 = ADC12SHS_0        |         // Usa o bit SC como trigger para iniciar a conversao
                ADC12SHP          |         // Usa o timer interno do ADC12
                ADC12SSEL_0       |         // Usa o MODCLK @5MHz como clock do ADC
                ADC12CONSEQ_1;              // Modo: Sequence-of-channels

    ADC12CTL2 = ADC12RES_2;                 // Usa resolucao de 12 bits

    ADC12CTL3 = ADC12CSTARTADD_0;           // Guarda o resultado no MEM0

    ADC12MCTL0 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL1 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL2 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL3 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL4 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL5 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL6 = ADC12INCH_12;              // Entrada no pino P3.0
    ADC12MCTL7 = ADC12INCH_12;              // Entrada no pino P3.0

    ADC12MCTL8  = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL9  = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL10 = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL11 = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL12 = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL13 = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL14 = ADC12INCH_13;             // Entrada no pino P3.1
    ADC12MCTL15 = ADC12INCH_13;             // Entrada no pino P3.1

    ADC12MCTL16 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL17 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL18 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL19 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL20 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL21 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL22 = ADC12INCH_14;             // Entrada no pino P3.2
    ADC12MCTL23 = ADC12INCH_14;             // Entrada no pino P3.2

    ADC12MCTL24 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL25 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL26 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL27 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL28 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL29 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL30 = ADC12INCH_15;             // Entrada no pino P3.3
    ADC12MCTL31 = ADC12INCH_15 |            // Entrada no pino P3.3
                  ADC12EOS;

    ADC12IER0 = ADC12IE31;                  // Habilita a interrupcao do MEM31
    ADC12CTL0 |= ADC12ENC;
}

void MAIN_mainInit( void ){
    CLOCK_init();                // Inicializa o sistema de clocks
                                // ACLK @32768Hz, SMCLK @1MHz,
                                // MCLK @16MHz
    MAIN_gpioInit();
    MAIN_timerInit();
    MAIN_adcInit();
    UART_init();

    __enable_interrupt();
}

uint32_t MAIN_getTick( void ){
    return uwTick;
}
