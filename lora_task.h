/*
 * lora_task.h
 *
 * Created: 5/15/2020 8:00:20 PM
 *  Author: cichy
 */ 
#include<stdlib.h>

#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>

typedef struct lora_bundle lora_t;

lora_t* lora_create(lora_payload_t* payload, EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready);

void lora_task(void* lora_bundle);

void lora_initialize(void);