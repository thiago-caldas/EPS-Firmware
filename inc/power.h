#ifndef INC_POWER_H_
#define INC_POWER_H_

/* Defines */

#define SUBSYSTEMS_NUMBER 4

/* Enums */

typedef enum en_SUBSYSTEM{
    COMMUNICATION_SUBSYSTEM,
    ATTITUDE_CONTROL_SUBSYSTEM,
    COMMAND_SUBSYSTEM,
    THERMAL_CONTROL_SUBSYSTEM,
}en_SUBSYSTEM;

typedef enum en_POWER_STATE{
    OFF,
    ON,
}en_POWER_STATE;

/* Structs */

typedef struct{
    uint8_t powerStatus[SUBSYSTEMS_NUMBER];
    uint8_t subsystemInConfig;
}stSUBSYSTEMS;

extern stSUBSYSTEMS stSubsystems;

/* Prototipos */

void POWER_initSubsystemsPowerStatus( void );
void POWER_updateSubsystemsPowerStatus( uint8_t subsystemInConfig );

#endif /* INC_POWER_H_ */
