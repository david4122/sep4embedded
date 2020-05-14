#pragma once

#include <stdio.h>
#include <stdint.h>


struct LoraPayload {
	uint8_t		len;
	uint8_t		bytes[2];
} typedef lora_payload_simulation_t;

//Updates console with the package data
void sent_upload_messages(lora_payload_simulation_t* lora_payload);