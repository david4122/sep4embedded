
#include<stdlib.h>

#include "LoraDriver.h"
#include "../better_print.h"

void print_message(lora_payload_simulation_t* lora_payload) {
	for (uint8_t i = 0; i < LORA_PAYLOAD_LENGTH; i++)
	{
		bprintf_int("+++%d+++", lora_payload->bytes[i]);
	}
}

void send_upload_message(void* args) {
	task_lora_data_container_t* task_lora_data_container = (task_lora_data_container_t*)args;
	uint32_t loraBitResponse;
	while (1) {
		//Read the bit responsible for LoRa to check if data is already collected and ready for the printout
		loraBitResponse = xEventGroupWaitBits(task_lora_data_container->event_group, task_lora_data_container->bit_for_lora, pdTRUE, pdTRUE, EVENT_GROUP_WAITING_TIME);
		if (loraBitResponse == task_lora_data_container->bit_for_lora) {
			
			

			lora_payload_simulation_t* rumun = malloc(sizeof(lora_payload_simulation_t));
			rumun->len = LORA_PAYLOAD_LENGTH;
			rumun->bytes[0] = 2;
			rumun->bytes[1] = 5;
			bprintf_int(">>>>> %d\n", rumun->bytes[0]);
			bprintf_int(">>>>> %d\n", rumun->bytes[1]);
			print_message(rumun);
			bprintf_int("<<<< %d\n", rumun->bytes[0]);
			bprintf_int("<<<< %d\n", rumun->bytes[1]);
			free(rumun);
			
			
			
			
			// print_message(&(task_lora_data_container->message));
		}
	}
}