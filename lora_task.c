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
	EventBits_t wait_for;
	EventBits_t ready;
};

lora_t* lora_create(lora_payload_t* payload, EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready) {
	lora_t* res = malloc(sizeof(lora_t));
	if(!res)
		return NULL;

	res->payload = payload;
	res->egroup = egroup;
	res->wait_for = wait_for;
	res->ready = ready;

	return res;
}

void lora_task(void* lora_bundle) {
	lora_t* self = (lora_t*) lora_bundle;

	xEventGroupWaitBits(self->egroup, LORA_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

	while(1) {
		EventBits_t loraBitResponse = xEventGroupWaitBits(self->egroup, self->wait_for, pdTRUE, pdTRUE, portMAX_DELAY);

		if((loraBitResponse & self->wait_for) == self->wait_for) {
			sent_upload_messages(self->payload);
			vTaskDelay(5000);

			xEventGroupSetBits(self->egroup, self->ready);
		}
	}
}