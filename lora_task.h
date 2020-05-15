/*
 * lora_task.h
 *
 * Created: 5/15/2020 8:00:20 PM
 *  Author: cichy
 */ 
#include<stdlib.h>

typedef struct lora_bundle lora_t;

lora_t* lora_create(lora_payload_t* payload, EventGroupHandle_t egroup, EventBits_t bits);

//Simulator of Lora prints out Data Package to the console
void lora_task(void* lora_bundle);