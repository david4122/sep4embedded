#include "LoraDriver.h"

void sent_upload_messages(lora_payload_simulation_t* lora_payload) {
	printf("\tPackage [ ");
	for (uint8_t i = 0; i < lora_payload->len; i++)
	{
		printf("%d ", lora_payload->bytes[i]);
	}
	printf("] is sent by LoRa\n");
}