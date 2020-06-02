/*
 * lora_task.c
 *
 * Created: 5/15/2020 8:00:55 PM
 *  Author: cichy
 */ 
#include "config.h"
#include "app_controller.h"
#include "lora_task.h"
#include "lora_adapter.h"


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
	lora_t* self = (lora_t*) lora_bundle;
	
	xEventGroupWaitBits(self->egroup, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	
	while(1) {
		EventBits_t loraBitResponse = xEventGroupWaitBits(self->egroup, self->ready_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		if((loraBitResponse & self->ready_bit) == self->ready_bit) {
			sent_upload_messages(self->payload);
			vTaskDelay(500);
		}
	}
}