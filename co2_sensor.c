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
	EventBits_t wait_for;
	EventBits_t ready;
};

static int driver_initialized = 0;

co2_t* co2_create(EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready) {
	if(!driver_initialized) {
		mh_z19_create(ser_USART3, NULL);
		driver_initialized = 1;
	}

	co2_t* res = malloc(sizeof(co2_t));
	if(!res)
		return NULL;

	res->data = -1;
	res->egroup = egroup;
	res->ready = ready;
	res->wait_for = wait_for;

	return res;
}

uint16_t* co2_get_data_pointer(co2_t* self) {
	return &(self->data);
}

void co2_task(void* pvParams) {
	co2_t* self = (co2_t*) pvParams;

	EventBits_t bits;
	while(1) {
		while(((bits = xEventGroupWaitBits(self->egroup, self->wait_for, pdFALSE, pdTRUE, portMAX_DELAY)) & self->wait_for) == 0
				|| (bits & self->ready) != 0);

		mh_z19_return_code_t return_code_co2_measurement = mh_z19_take_meassuring();
		vTaskDelay(500);

		if(return_code_co2_measurement == MHZ19_OK) {
			mh_z19_get_co2_ppm(&self->data);
			safeprintln_int("[<] CO2: ", self->data);

			xEventGroupSetBits(self->egroup, self->ready);

		} else {
#ifdef VERBOSE
			safeprintln("[!] CO2: measurement failed");
#endif
		}
	}
}