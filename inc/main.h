
#ifndef MAIN_H_
#define MAIN_H_

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "application.h"
#include "interface.h"
#include "i2c.h"
#include "libclock/clock.h"
#include "power.h"
#include "sensors.h"
#include "uart.h"
#include "utils.h"
#include "wdt.h"

extern uint32_t uwTick;


/* Defines */

#define POWER_SUBSYSTEMS_PORT    P3OUT
#define COMMUNICATION_POWER      BIT4    // P3.4
#define ATTITUDE_POWER           BIT5    // P3.5
#define COMMAND_POWER            BIT6    // P3.6
#define THERMAL_POWER            BIT7    // P3.7

#define BATTERY_HEATER_PORT      P1OUT
#define BATTERY_HEATER           BIT3    // P1.3

#define ADC_PORT                 P3OUT
#define TEMP_SENSOR              BIT0    // P3.0
#define PANEL_VOLTAGE            BIT1    // P3.1
#define BATT_VOLTAGE             BIT2    // P3.2
#define LOAD_VOLTAGE             BIT3    // P3.3


/* Prototipos */

void MAIN_gpioInit( void );
void MAIN_timerInit( void );
void MAIN_adcInit( void );
void UART_init( void );
void MAIN_mainInit( void );
uint32_t MAIN_getTick( void );
void INTERFACE_clearTerminal( void );

#endif /* MAIN_H_ */
