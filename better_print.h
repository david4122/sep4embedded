/*
 * better_print.h
 *
 * Created: 5/14/2020 10:16:54 PM
 *  Author: cichy
 */
#pragma once

#include "LoraDriver.h"

void bprintf_init();

void bprintf(const char* str);

void bprintf_int(uint16_t n);

void bprintCallback(void(*callback_function_example)(lora_payload_simulation_t*), lora_payload_simulation_t* arg);