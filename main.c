/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <hal_defs.h>
#include <ihal.h>

#include <ATMEGA_FreeRTOS.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>

#include<display_7seg.h>

#include "app_controller.h"


void initialiseSystem() {
	// Initialize the trace-driver to be used together with the R2R-Network
	trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0);

	// Initialise the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_create(LORA_USART, NULL);
	// Create LoRaWAN task and start it up with priority 3
	//lora_handler_create(3);
	display_7seg_init(NULL);
	display_7seg_power_up();
	display_7seg_display(69,2);
}

int main(void) {
	
	initialiseSystem();

	initialize();

	while(1) {}
}

