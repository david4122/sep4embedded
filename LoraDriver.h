#pragma once

#include <stdio.h>
#include <stdint.h>


struct LoraPayload {
	uint16_t		len;
	uint16_t		bytes[2];
} typedef lora_payload_simulation_t;

//Updates console with the package data
void sent_upload_messages(lora_payload_simulation_t* lora_payload);