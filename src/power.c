#include "inc/main.h"

stSUBSYSTEMS stSubsystems;

void POWER_initSubsystemsPowerStatus ( void )
{
    int i = 0;
    for(i = 0; i < SUBSYSTEMS_NUMBER; i++) stSubsystems.powerStatus[i] = 0;
}

void POWER_updateSubsystemsPowerStatus( uint8_t subsystemInConfig )
{
    switch(subsystemInConfig){
    case COMMUNICATION_SUBSYSTEM:
        if(stSubsystems.powerStatus[COMMUNICATION_SUBSYSTEM] == ON)
            POWER_SUBSYSTEMS_PORT |= COMMUNICATION_POWER;
        else POWER_SUBSYSTEMS_PORT &= ~COMMUNICATION_POWER;
        break;
    case ATTITUDE_CONTROL_SUBSYSTEM:
        if(stSubsystems.powerStatus[ATTITUDE_CONTROL_SUBSYSTEM] == ON)
            POWER_SUBSYSTEMS_PORT |= ATTITUDE_POWER;
        else POWER_SUBSYSTEMS_PORT &= ~ATTITUDE_POWER;
        break;
    case COMMAND_SUBSYSTEM:
        if(stSubsystems.powerStatus[COMMAND_SUBSYSTEM] == ON)
            POWER_SUBSYSTEMS_PORT |= COMMAND_POWER;
        else POWER_SUBSYSTEMS_PORT &= ~COMMAND_POWER;
        break;
    case THERMAL_CONTROL_SUBSYSTEM:
        if(stSubsystems.powerStatus[THERMAL_CONTROL_SUBSYSTEM] == ON)
            POWER_SUBSYSTEMS_PORT |= THERMAL_POWER;
        else POWER_SUBSYSTEMS_PORT &= ~THERMAL_POWER;
        break;
    }
}
