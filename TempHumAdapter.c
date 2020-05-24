
/*
 * TempHumAdapter.c
 *
 * Created: 5/24/2020 10:11:36 PM
 *  Author: cichy
 */ 
#include<stdio.h>
#include "config.h"
#include "TempHumAdapter.h"

struct tempHum_sensor {
	float hum;
	float temp;

	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

static int driver_initialized = 0;

tempHum_t* tempHum_create(EventGroupHandle_t egroup, EventBits_t bit) {
	if(!driver_initialized) {
		if ( HIH8120_OK != hih8120Create() ) {
			puts("[!] HIH820 could not be initialized");
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
	return &self->hum;
}

float* get_temp_pointer(tempHum_t* self) {
	return &self->temp;
}

void tempHum_task(void *param) {
	tempHum_t *self = (tempHum_t*) param;
	xEventGroupWaitBits(self->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	
	while(1) {
		puts("[*] HUMTEMP loop starts");
		if ( HIH8120_OK != hih8120Wakeup() )
		{
			puts("[!] HUMTEMP failed to wakeup");
			break;
		}

		vTaskDelay(1000);

		if(HIH8120_OK != hih8120Meassure()) {
			puts("[!] HUMTEMP measure failed");
			break;
		}
		
		vTaskDelay(1000);
		
		self->hum = hih8120GetHumidity();
		self->temp = hih8120GetTemperature();
		
		printf("[*] HUMTEMP measurement done: %f, %f\n", self->hum, self->temp);
		
		xEventGroupSetBits(self->egroup, self->ready_bit);
	}
}
