#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

/* Defines */

#define INA_ADDR         0x40
#define CONFIG_REG_ADDR  0X00
#define CH1_REG_ADDR     0X01
#define CH2_REG_ADDR     0X03
#define CH3_REG_ADDR     0X05

#define PANEL_CURRENT_ADDR    CH1_REG_ADDR
#define BATT_CURRENT_ADDR     CH2_REG_ADDR
#define LOAD_CURRENT_ADDR     CH3_REG_ADDR

/* Structs */

typedef struct{
    float panelVoltage;
    float battVoltage;
    float loadVoltage;
    float panelCurrent;
    float battCurrent;
    float loadCurrent;
    float battTemp;
}stMEASURES;

extern stMEASURES stMeasures;

/* Prototipos */

void SENSORS_answerRequest( void );
void SENSORS_readCurrent( uint8_t regAddr );
float SENSORS_convertInaValue ( uint16_t inaValue );
void SENSORS_configIna ( void );
void SENSORS_sendByteToIna ( uint8_t regAddr, uint16_t byte );
void SENSORS_convertFtoStr (float *data, char *string);
uint16_t SENSORS_convertValueToSend( float value );
float SENSORS_convertReceivedValue( uint16_t value );
void SENSORS_sendAnswer( uint16_t byte );

#endif /* INC_SENSORS_H_ */
