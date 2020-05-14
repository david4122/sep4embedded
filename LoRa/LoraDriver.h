#pragma once

#include <stdio.h>
#include <stdint.h>
#include "../CustomConfig.h"
#include "LoraPayloadSimulation.h"
#include "DataLora.h"

//Simulates sending message by printing out package to the console
void print_message(lora_payload_simulation_t* lora_payload);
//Task for LoRa Simulator
void send_upload_message(void* args);