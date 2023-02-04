#ifndef INC_UART_H_
#define INC_UART_H_

/* Defines */

#define SIZE_UART_RX_BUFF 200
#define SIZE_UART_TX_BUFF 500

/* Structs */

typedef struct stUART{
    uint8_t rxBuff[SIZE_UART_RX_BUFF];
    uint8_t txBuff[SIZE_UART_TX_BUFF];
    uint16_t rxIndex;
    uint16_t txIndex;
    uint16_t bytesToSend;
    uint32_t tmt;
}stUART;

extern stUART stUart;

/* Prototipos */

void UART_init( void );
void UART_write( char * );
void UART_clearRxBuff( void );
uint32_t MAIN_getTick( void );
void INTERFACE_clearTerminal( void );


#endif /* INC_UART_H_ */
