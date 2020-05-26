
/*
 * TempHumAdapter.c
 *
 * Created: 5/24/2020 10:11:36 PM
 *  Author: cichy
 */ 
#include<stdio.h>

#include "config.h"
#include "temp_hum_sensor.h"

#ifdef VERBOSE
#include "safeprint.h"
#endif

struct tempHum_sensor {
	float hum;
	float temp;

	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

static int driver_initialized = 0;

tempHum_t* tempHum_create(EventGroupHandle_t egroup, EventBits_t bit) {
	if(!driver_initialized) {
		if(HIH8120_OK != hih8120Create()) {
#ifdef VERBOSE
			safeprintln("[!] HIH820 could not be initialized");
#endif
		} else {
			driver_initialized = 1;
		}
	}

	tempHum_t *res = malloc(sizeof(tempHum_t));
	if(!res)
		return NULL;
		
	res->hum = -1;
	res->temp = -1;
	res->egroup = egroup;
	res->ready_bit = bit;
	
	return res;
}

float* get_hum_pointer(tempHum_t* self) {
	return &(self->hum);
}

float* get_temp_pointer(tempHum_t* self) {
	return &(self->temp);
}

void tempHum_task(void *param) {
	tempHum_t *self = (tempHum_t*) param;
	xEventGroupWaitBits(self->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	
	while(1) {
		if(HIH8120_OK != hih8120Wakeup())
		{
#ifdef VERBOSE
			safeprintln("[!] HUMTEMP failed to wakeup");
#endif
			break;
		}

		vTaskDelay(500);

		if(HIH8120_OK != hih8120Meassure()) {

#ifdef VERBOSE
			safeprintln("[!] HUMTEMP measure failed");
#endif
			break;
		}

		vTaskDelay(500);

		self->hum = hih8120GetHumidity();
		self->temp = hih8120GetTemperature();

#ifdef VERBOSE
		safeprintln_ints("[<] HUMTEMP measurement done: ", 2, (uint8_t) (self->hum), (uint8_t) (self->temp));
#endif

		xEventGroupSetBits(self->egroup, self->ready_bit);
	}
}
