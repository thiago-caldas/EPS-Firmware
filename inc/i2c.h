#ifndef INC_I2C_H_
#define INC_I2C_H_

/* Defines */

#define SIZE_I2C_TX_BUFF 128
#define SIZE_I2C_RX_BUFF 128

/* Structs */

typedef struct stI2C{
    uint8_t txBuff[SIZE_I2C_TX_BUFF];
    uint8_t rxBuff[SIZE_I2C_RX_BUFF];
    uint8_t txIndex;
    uint8_t rxIndex;
    uint8_t bytesToSend;
    uint8_t sendingData:1;
}stI2C;

extern stI2C stI2CMaster;
extern stI2C stI2CSlave;


/* Prototipos */

void I2C_configMaster(uint32_t baudrate_Hz);
void I2C_configSlave(uint8_t ownAddr);
int8_t I2C_masterWrite(uint8_t slaveAddr, uint8_t *data, uint16_t nBytes);
int8_t I2C_masterRead(uint8_t slaveAddr, uint16_t nBytes);
void I2C_clearMasterRxBuff ( void );
void I2C_slaveWrite( uint8_t *data, uint16_t nBytes );

#endif /* INC_I2C_H_ */
