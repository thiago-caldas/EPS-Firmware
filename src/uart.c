#include "inc/main.h"

stUART stUart;

/* UART baudrate para o modem -> 9600*/
void UART_init( void ){

       UCA0CTLW0 = UCSWRST;                    // Entra no modo de configuracao da interface serial
       UCA0CTLW0 |= UCSSEL__SMCLK;             // Configura SMCLK como fonte de clock para o a porta serial

       // Calculo do Baud Rate
       // 1000000/(16*9600) = 6.51
       // Parte decimal = 0.51
       // UCBRFx = int ( (0.51)*16) = 8
       UCA0BRW = 6;                            // 1000000/16/9600
       UCA0MCTLW |= (UCOS16 | UCBRF_8);

       UCA0CTLW0 &= ~UCSWRST;                  // Sai do modo de configuracao
       UCA0IE |= UCRXIE;                       // Habilita interrupcao serial
}

void UART_write( char *data )
{
    stUart.bytesToSend = strlen( (char *)data );

    if( data == NULL || stUart.bytesToSend == 0 )
    return;

    memset( stUart.txBuff, 0, SIZE_UART_TX_BUFF );

    memcpy( stUart.txBuff, data, stUart.bytesToSend );

    UART_clearRxBuff();

    stUart.txIndex = 0;

    UCA0IE |= UCTXIE;

    UCA0TXBUF = stUart.txBuff[stUart.txIndex++];

    stUart.bytesToSend--;
}

void UART_clearRxBuff( void ){
    UCA0IE &= ~UCRXIE;

    memset( stUart.rxBuff, 0, SIZE_UART_RX_BUFF );

    stUart.rxIndex = 0;

    UCA0IE |= UCRXIE;
}
