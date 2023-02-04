#include "inc/main.h"

stINTERFACE stInterface;

void INTERFACE_interfaceRun( void ){
    switch(stInterface.maq){
        char *str;
        char message[400];
        char panelVStr[10], battVStr[10], loadVStr[10], panelCStr[10], battCStr[10], loadCStr[10];

        case enMAQ_INTERFACE_MENU:
            UART_write("Digite o numero correspondente a operacao desejada:\r\n\n"
                       "1 - Configurar temperatura do aquecedor da bateria;\r\n"
                       "2 - Solicitar dados de telemetria;\r\n"
                       "3 - Configurar a alimentacao dos subsistemas.\r\n\n");
            INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_MENU);
            break;
        case enMAQ_INTERFACE_WAIT_MENU:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_1_SEGUNDO__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str == NULL) break;
            else
            {
                str = strtok((char *) stUart.rxBuff, "\r" );
                if (!strcmp(str, "1")) stInterface.maq = enMAQ_INTERFACE_CONFIG_BATT_TEMP;
                else if (!strcmp(str, "2")) stInterface.maq = enMAQ_INTERFACE_REQUEST_TELEMETRY_DATA;
                else if (!strcmp(str, "3")) INTERFACE_changeMaqState(enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_MENU);
                else
                {
                    UART_write("Operacao invalida.\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_RESTART);
                }
            }
            break;
        case enMAQ_INTERFACE_CONFIG_BATT_TEMP:
            UART_write("Digite a temperatura minima na qual se deseja manter a bateria:\r\n");
            INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_BATT_TEMP);
            break;
        case enMAQ_INTERFACE_WAIT_CONFIG_BATT_TEMP:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str != NULL)
            {
                str = strtok((char *) stUart.rxBuff, "\r" );
                stInterface.battTempConfig = atof(str);
                INTERFACE_changeMaqState(enMAQ_INTERFACE_RESTART);
            }
            break;
        case enMAQ_INTERFACE_REQUEST_TELEMETRY_DATA:
            SENSORS_readCurrent( PANEL_CURRENT_ADDR );
            SENSORS_readCurrent( BATT_CURRENT_ADDR );
            SENSORS_readCurrent( LOAD_CURRENT_ADDR );

            SENSORS_convertFtoStr( &stMeasures.panelVoltage, panelVStr );
            SENSORS_convertFtoStr( &stMeasures.battVoltage,  battVStr  );
            SENSORS_convertFtoStr( &stMeasures.loadVoltage,  loadVStr  );
            SENSORS_convertFtoStr( &stMeasures.panelCurrent, panelCStr );
            SENSORS_convertFtoStr( &stMeasures.battCurrent,  battCStr  );
            SENSORS_convertFtoStr( &stMeasures.loadCurrent,  loadCStr  );

            sprintf(message, "Tensao do painel: %s V\r\n"
                             "Tensao da bateria: %s V\r\n"
                             "Tensao da carga: %s V\r\n"
                             "Corrente do painel: %s mA\r\n"
                             "Corrente da bateria: %s mA\r\n"
                             "Corrente da carga: %s mA\r\n\n",
                             panelVStr ,
                             battVStr  ,
                             loadVStr  ,
                             panelCStr ,
                             battCStr  ,
                             loadCStr  );

            UART_write(message);
            INTERFACE_changeMaqState(enMAQ_INTERFACE_RESTART);
            break;
        case enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_MENU:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            sprintf(message, "Digite o numero correspondente ao subsistema para configuracao:\r\n\n"
                       "1 - Subsistema de comunicacao - %s\r\n"
                       "2 - Subsistema de controle e determinacao de atitude - %s\r\n"
                       "3 - Subsistema de comando - %s\r\n"
                       "4 - Subsistema de controle de temperatura - %s\r\n\n",
                       stSubsystems.powerStatus[COMMUNICATION_SUBSYSTEM] ? "LIGADO" : "DESLIGADO",
                       stSubsystems.powerStatus[ATTITUDE_CONTROL_SUBSYSTEM] ? "LIGADO" : "DESLIGADO",
                       stSubsystems.powerStatus[COMMAND_SUBSYSTEM] ? "LIGADO" : "DESLIGADO",
                       stSubsystems.powerStatus[THERMAL_CONTROL_SUBSYSTEM] ? "LIGADO" : "DESLIGADO");
            UART_write(message);
            INTERFACE_changeMaqState(enMAQ_INTERFACE_CONFIG_POWER_SUPPLY);
            break;
        case enMAQ_INTERFACE_CONFIG_POWER_SUPPLY:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str == NULL) break;
            else
            {
                str = strtok((char *) stUart.rxBuff, "\r" );
                if (!strcmp(str, "1"))
                {
                    stSubsystems.subsystemInConfig = COMMUNICATION_SUBSYSTEM;
                    if (stSubsystems.powerStatus[COMMUNICATION_SUBSYSTEM] == OFF)
                        UART_write("Deseja ligar o subsistema de comunicacao?(s/n)\r\n");
                    else UART_write("Deseja desligar o subsistema de comunicacao?(s/n)\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY);
                }
                else if (!strcmp(str, "2"))
                {
                    stSubsystems.subsystemInConfig = ATTITUDE_CONTROL_SUBSYSTEM;
                    if (stSubsystems.powerStatus[ATTITUDE_CONTROL_SUBSYSTEM] == OFF)
                        UART_write("Deseja ligar o subsistema de controle e determinacao de atitude?(s/n)\r\n");
                    else UART_write("Deseja desligar o subsistema de controle e determinacao de atitude?(s/n)\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY);
                }
                else if (!strcmp(str, "3"))
                {
                    stSubsystems.subsystemInConfig = COMMAND_SUBSYSTEM;
                    if (stSubsystems.powerStatus[COMMAND_SUBSYSTEM] == OFF)
                        UART_write("Deseja ligar o subsistema de comando?(s/n)\r\n");
                    else UART_write("Deseja desligar o subsistema de comando?(s/n)\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY);
                }
                else if (!strcmp(str, "4"))
                {
                    stSubsystems.subsystemInConfig = THERMAL_CONTROL_SUBSYSTEM;
                    if (stSubsystems.powerStatus[THERMAL_CONTROL_SUBSYSTEM] == OFF)
                        UART_write("Deseja ligar o subsistema de controle de temperatura?(s/n)\r\n");
                    else UART_write("Deseja desligar o subsistema de controle de temperatura?(s/n)\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY);
                }
                else
                {
                    UART_write("Operacao invalida.\r\n");
                    INTERFACE_changeMaqState(enMAQ_INTERFACE_RESTART);
                }
            }
            break;
        case enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str == NULL) break;
            else
            {
                str = strtok((char *) stUart.rxBuff, "\r" );
                if (!strcmp(str, "s"))
                {
                    if(stSubsystems.powerStatus[stSubsystems.subsystemInConfig] == OFF)
                        stSubsystems.powerStatus[stSubsystems.subsystemInConfig] = ON;
                    else stSubsystems.powerStatus[stSubsystems.subsystemInConfig] = OFF;
                }
            }
            POWER_updateSubsystemsPowerStatus(stSubsystems.subsystemInConfig);
            stInterface.maq = enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_AGAIN;
            break;
        case enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_AGAIN:
            UART_write("Deseja configurar a alimentacao de outro subsistema?(s/n)\r\n");
            INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY_AGAIN);
            break;
        case enMAQ_INTERFACE_WAIT_CONFIG_POWER_SUPPLY_AGAIN:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str == NULL) break;
            else
            {
                str = strtok((char *) stUart.rxBuff, "\r" );
                if (!strcmp(str, "s"))stInterface.maq = enMAQ_INTERFACE_CONFIG_POWER_SUPPLY_MENU;
                else stInterface.maq = enMAQ_INTERFACE_RESTART;
            }
            break;
        case enMAQ_INTERFACE_RESTART:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            UART_write("Pressione o ENTER para abrir o menu de configuracao.\r\n");
            INTERFACE_changeMaqState(enMAQ_INTERFACE_WAIT_RESTART);
            break;
        case enMAQ_INTERFACE_WAIT_RESTART:
            if (UTILS_checkTimeout(&stInterface.tmt, __VAL_500_MS__ ) != HAL_OK) break;
            str =  strstr((char *) stUart.rxBuff, "\r" );
            if(str != NULL)  stInterface.maq = enMAQ_INTERFACE_MENU;
            break;
    }
}

void INTERFACE_changeMaqState( enMAQ_INTERFACE nextState )
{
    stInterface.tmt = MAIN_getTick();
    stInterface.maq = nextState;
}

void INTERFACE_clearTerminal( void )
{
    stUart.tmt = MAIN_getTick();
    UART_write("\033[2J\r");
    while(UTILS_checkTimeout(&stUart.tmt, __VAL_500_MS__) != HAL_OK);
    stUart.tmt = MAIN_getTick();
    UART_write("\033[H\r");
    while(UTILS_checkTimeout(&stUart.tmt, __VAL_500_MS__) != HAL_OK);
}
