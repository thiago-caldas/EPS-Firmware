#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

/* Structs */

typedef struct{
    uint32_t tmt;
}stAPPLICATION;

extern stAPPLICATION stApplication;

/* Prototipos */

void APP_applicationRun( void );
void APP_controlBatteryHeater ( void );

#endif /* INC_APPLICATION_H_ */
