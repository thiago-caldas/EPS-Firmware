#ifndef INTERFACE_H_
#define INTERFACE_H_

/* Enums */

#define ENUM_STEP_PAIR(maq, step) enMAQ_##maq##_##step, enMAQ_##maq##_WAIT_##step
#define ENUM_STEP_PAIR_INTERFACE(step) ENUM_STEP_PAIR(INTERFACE, step)

typedef enum enMAQ_INTERFACE{
    ENUM_STEP_PAIR_INTERFACE(MENU),
    ENUM_STEP_PAIR_INTERFACE(CONFIG_BATT_TEMP),
    enMAQ_INTERFACE_REQUEST_TELEMETRY_DATA,
    enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_MENU,
    ENUM_STEP_PAIR_INTERFACE(CONFIG_POWER_SUPPLY),
    ENUM_STEP_PAIR_INTERFACE(CONFIG_POWER_SUPPLY_AGAIN),
    ENUM_STEP_PAIR_INTERFACE(RESTART),
}enMAQ_INTERFACE;

/* Structs */

typedef struct{
    enMAQ_INTERFACE maq;
    uint32_t tmt;
    float battTempConfig;
}stINTERFACE;

extern stINTERFACE stInterface;

/* Prototipos */

void INTERFACE_interfaceRun( void );
void INTERFACE_changeMaqState( enMAQ_INTERFACE nextState );
void INTERFACE_clearTerminal( void );



#endif /* INTERFACE_H_ */
