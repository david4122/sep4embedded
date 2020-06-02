/*
 * co2_sensor.c
 *
 * Created: 5/15/2020 6:26:56 PM
 *  Author: cichy
 */ 
#include<stdlib.h>
#include<stdio.h>

#ifdef VERBOSE
#include "safeprint.h"
#endif

#include "co2_sensor.h"
#include "config.h"

#include "safeprint.h"

struct co2_sensor {
	uint16_t data;
	
	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

static int driver_initialized = 0;

co2_t* co2_create(EventGroupHandle_t egroup, EventBits_t bit) {
	if(!driver_initialized) {
		mh_z19_create(ser_USART3, NULL);
		driver_initialized = 1;
	}

	co2_t* res = malloc(sizeof(co2_t));
	if(!res)
		return NULL;
	
	res->data = -1;
	res->egroup = egroup;
	res->ready_bit = bit;
	
	return res;
}

uint16_t* co2_get_data_pointer(co2_t* self) {
	return &(self->data);
}

void co2_task(void* pvParams) {
	co2_t* sensor = (co2_t*) pvParams;
	
	puts("CO2 w8");
	EventBits_t bits;
	while(((bits = xEventGroupWaitBits(sensor->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY)) & LORA_READY_BIT) == 0);
	puts("CO2 start");

	while(1) {
		mh_z19_return_code_t return_code_co2_measurement = mh_z19_take_meassuring();
		vTaskDelay(500);
		
		if(return_code_co2_measurement == MHZ19_OK) {
			mh_z19_get_co2_ppm(&sensor->data);
			safeprintln_int("[<] CO2: ", sensor->data);

			xEventGroupSetBits(sensor->egroup, sensor->ready_bit);

		} else {
#ifdef VERBOSE
			safeprintln("[!] CO2: measurement failed");
#endif
		}
	}
}