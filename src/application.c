#include "inc/main.h"

stAPPLICATION stApplication;

void APP_applicationRun( void )
{
    while(1)
    {
        WDT_refresh();
        INTERFACE_interfaceRun();
        APP_controlBatteryHeater();

        /* Realiza leitura dos valores de corrente a cada minuto */
        if (UTILS_checkTimeout(&stApplication.tmt, 60*__VAL_1_SEGUNDO__ ) == HAL_OK)
        {
            SENSORS_readCurrent( PANEL_CURRENT_ADDR );
            SENSORS_readCurrent( BATT_CURRENT_ADDR );
            SENSORS_readCurrent( LOAD_CURRENT_ADDR );
            stApplication.tmt = MAIN_getTick();
        }
    }
}

void APP_controlBatteryHeater ( void )
{
    if(stMeasures.battTemp < stInterface.battTempConfig) BATTERY_HEATER_PORT |= BATTERY_HEATER;
    else BATTERY_HEATER_PORT &= ~BATTERY_HEATER;
}
