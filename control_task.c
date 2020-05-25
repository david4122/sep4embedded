/*
 * control_task.c
 *
 * Created: 5/15/2020 8:24:11 PM
 *  Author: cichy
 */ 
#include<stdio.h>

#include "config.h"
#include "control_task.h"
#include "better_print.h"

struct control_bundle {
	lora_payload_t* lora_payload;
	bundle_t* readings;
	EventGroupHandle_t egroup;
	EventBits_t read_done;
	EventBits_t meassage_done;
};

control_t* control_create(lora_payload_t* payload, bundle_t* readings, EventGroupHandle_t egroup, EventBits_t read_done, EventBits_t message_done) {
	control_t* res = malloc(sizeof(control_t));
	if(!res)
		return NULL;

	res->lora_payload = payload;
	res->readings = readings;
	res->egroup = egroup;
	res->read_done = read_done;
	res->meassage_done = message_done;

	return res;
}

void control_task(void* control_bundle) {
	control_t* bundle = (control_t*) control_bundle;
	
	xEventGroupWaitBits(bundle->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	
	while(1) {
		EventBits_t bitsResult = xEventGroupWaitBits(bundle->egroup, bundle->read_done, pdFALSE, pdTRUE, portMAX_DELAY);
		
		if((bitsResult & bundle->read_done) == bundle->read_done) {
			
			printf("[+] [CONTROL] CO2: %d, TEMP: %d, HUM: %d\n", get_co2(bundle->readings), (int) get_temperature(bundle->readings), (int) get_humidity(bundle->readings));
			
			bundle->lora_payload->bytes[0] = co2_get_lower_bits(bundle->readings);
			bundle->lora_payload->bytes[1] = co2_get_higher_bits(bundle->readings);
			
			bundle->lora_payload->bytes[2] = (uint8_t) get_temperature(bundle->readings);
			bundle->lora_payload->bytes[3] = (uint8_t) get_humidity(bundle->readings);

			xEventGroupSetBits(bundle->egroup, bundle->meassage_done);
			
			xEventGroupClearBits(bundle->egroup, bundle->read_done);
			
		} else if(bitsResult & CO2_SENSOR_BIT) {
			bprintf("[!] [CONTROL] Sensor 2 didn't measure data yet\n");
		} else if(bitsResult & TEMP_HUM_BIT) {
			bprintf("[!] [CONTROL] CO2 didn't measure data yet\n");
		} else {
			bprintf("[!] [CONTROL] Sensors not ready yet\n");
		}
	}
}