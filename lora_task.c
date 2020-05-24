/*
 * lora_task.c
 *
 * Created: 5/15/2020 8:00:55 PM
 *  Author: cichy
 */ 
#include "config.h"
#include "AppController.h"
#include "lora_task.h"

#include "better_print.h"

struct lora_bundle {
	lora_payload_t* payload;
	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

lora_t* lora_create(lora_payload_t* payload, EventGroupHandle_t egroup, EventBits_t bits) {
	lora_t* res = malloc(sizeof(lora_t));
	if(!res)
		return NULL;
	
	res->payload = payload;
	res->egroup = egroup;
	res->ready_bit = bits;
	
	return res;
}

void lora_task(void* lora_bundle) {
	lora_t* bundle = (lora_t*) lora_bundle;
	
	xEventGroupWaitBits(bundle->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	
	for (;;) {
		EventBits_t loraBitResponse = xEventGroupWaitBits(bundle->egroup, bundle->ready_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		if ((loraBitResponse & bundle->ready_bit) == bundle->ready_bit) {
			bprintCallback(sent_upload_messages, bundle->payload);
			vTaskDelay(5000);
			puts("LORA after delay");
		}
	}
}