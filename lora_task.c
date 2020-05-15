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
	for (;;) {
		EventBits_t loraBitResponse = xEventGroupWaitBits(bundle->egroup, bundle->ready_bit, pdTRUE, pdTRUE, EVENT_GROUP_DELAY);
		if (loraBitResponse == bundle->ready_bit) {
			bprintf("LORA BUILDING STRING");
			bprintCallback(sent_upload_messages, bundle->payload);
			bprintf("LORA FINISHED BUILDING STRING");
		}
	}
}