#include "inc/main.h"

stI2C stI2CMaster;
stI2C stI2CSlave;

void I2C_configMaster(uint32_t baudrate_Hz)
{
    /* Interface configurada como mestre I2C */

    UCB1CTLW0 = UCSWRST;                      // Reseta a interface

    UCB1CTLW0 |= UCMODE_3 | UCSYNC;           // Configuracao - UCMODE_3 = I2C

    UCB1CTLW0 |= UCMST;

    UCB1CTLW0 |= UCSSEL__SMCLK;

    UCB1BRW = 1000000/baudrate_Hz;            // Bit clock -> 1 MHz/UCB1BRW

    UCB1CTLW0 &= ~UCSWRST;                    // Libera o reset
}

void I2C_configSlave(uint8_t ownAddr)
{
    /* Interface configurada como escravo I2C */

    UCB2CTLW0 = UCSWRST;                     // Reseta a interface

    UCB2CTLW0 |= UCMODE_3 | UCSYNC;          // Configuracao - UCMODE_3 = I2C

    UCB2I2COA0 = ownAddr | UCOAEN;

    UCB2CTLW0 &= ~UCSWRST;                   // Libera o reset

    UCB2IE |= UCTXIE0 | UCRXIE0;
}

int8_t I2C_masterWrite (uint8_t slaveAddr, uint8_t *data, uint16_t nBytes)
{
    UCB1I2CSA = slaveAddr;
    UCB1CTLW0_L |= (UCTR|UCTXSTT);

    // Destrava o ciclo de ACK
    while(!(UCB1IFG & UCTXIFG));
    UCB1TXBUF = *data++;  nBytes--;

    while( !(UCB1IFG & UCTXIFG) && !(UCB1IFG & UCNACKIFG) );
    if (UCB1IFG & UCNACKIFG)
    {
       UCB1CTLW0_L |= UCTXSTP;
       while (UCB1CTLW0 & UCTXSTP);
       return 1;
    }

    while(nBytes--){
       while(!(UCB1IFG & UCTXIFG));
       UCB1TXBUF = *data++;
    }

    while(!(UCB1IFG & UCTXIFG));
    UCB1IFG &= ~UCTXIFG;

    UCB1CTLW0 |= UCTXSTP;
    while(UCB1CTLW0 & UCTXSTP);
    return 0;
}

int8_t I2C_masterRead (uint8_t slaveAddr, uint16_t nBytes)
{
    UCB1I2CSA = slaveAddr;
    UCB1CTLW0_L &= ~UCTR;
    UCB1CTLW0_L |= UCTXSTT;

    while(!(UCB1IFG & UCRXIFG) && !(UCB1IFG & UCNACKIFG));
    if (UCB1IFG & UCNACKIFG) {
        UCB1CTLW0 |= UCTXSTP;
        while (UCB1CTLW0 & UCTXSTP);
        return 1;
    }

    while(nBytes--){
        while(!(UCB1IFG & UCRXIFG));
        stI2CMaster.rxBuff[stI2CMaster.rxIndex] = UCB1RXBUF;
        stI2CMaster.rxIndex++;
        if(nBytes == 1) UCB1CTLW0 |= UCTXSTP;
    }

    while(UCB1CTLW0 & UCTXSTP);
    return 0;
}

void I2C_clearMasterRxBuff ( void )
{
    stI2CMaster.rxIndex = 0;
    memset( stI2CMaster.rxBuff, 0, SIZE_I2C_RX_BUFF );
}

void I2C_slaveWrite( uint8_t *data, uint16_t nBytes )
{
    stI2CSlave.bytesToSend = nBytes;

    if(stI2CSlave.bytesToSend == 0 ) return;

    memset( stI2CSlave.txBuff, 0, SIZE_I2C_TX_BUFF );

    memcpy( stI2CSlave.txBuff, data, stI2CSlave.bytesToSend );

    stI2CSlave.txIndex = 0;

    UCB2TXBUF = stI2CSlave.txBuff[stI2CSlave.txIndex++];

    stI2CSlave.bytesToSend--;
}
