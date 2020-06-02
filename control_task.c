/*
 * control_task.c
 *
 * Created: 5/15/2020 8:24:11 PM
 *  Author: cichy
 */ 
#include<stdio.h>

#include "config.h"
#include "control_task.h"

#ifdef VERBOSE
#include "safeprint.h"
#endif

struct control_bundle {
	lora_payload_t* lora_payload;
	bundle_t* readings;

	EventGroupHandle_t egroup;
	EventBits_t wait_for;
	EventBits_t ready;
};

control_t* control_create(lora_payload_t* payload, bundle_t* readings, EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready) {
	control_t* res = malloc(sizeof(control_t));
	if(!res)
		return NULL;

	res->lora_payload = payload;
	res->readings = readings;
	res->egroup = egroup;
	res->wait_for = wait_for;
	res->ready = ready;

	return res;
}

void control_task(void* control_bundle) {
	control_t* self = (control_t*) control_bundle;

	xEventGroupWaitBits(self->egroup, LORA_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

	while(1) {
		xEventGroupClearBits(self->egroup, self->ready);

		EventBits_t bitsResult = xEventGroupWaitBits(self->egroup, self->wait_for, pdFALSE, pdTRUE, portMAX_DELAY);
		xEventGroupClearBits(self->egroup, LORA_BIT);
		xEventGroupClearBits(self->egroup, self->wait_for);

		if((bitsResult & self->wait_for) == self->wait_for) {

#ifdef VERBOSE
			safeprint_acquire();
			printf("[+] [CONTROL] CO2: %d, TEMP: %d, HUM: %d\n",
					get_co2(self->readings),
					(int) get_temperature(self->readings),
					(int) get_humidity(self->readings));
			safeprint_release();
#endif

			self->lora_payload->bytes[0] = co2_get_lower_bits(self->readings);
			self->lora_payload->bytes[1] = co2_get_higher_bits(self->readings);

			self->lora_payload->bytes[2] = (uint8_t) get_temperature(self->readings);
			self->lora_payload->bytes[3] = (uint8_t) get_humidity(self->readings);

			xEventGroupSetBits(self->egroup, self->ready);

#ifdef VERBOSE
		} else if(bitsResult & CO2_SENSOR_BIT) {
			safeprintln("[!] [CONTROL] Sensor 2 didn't measure data yet");
		} else if(bitsResult & TEMP_HUM_BIT) {
			safeprintln("[!] [CONTROL] CO2 didn't measure data yet");
		} else {
			safeprintln("[!] [CONTROL] Sensors not ready yet");
#endif
		}
	}
}