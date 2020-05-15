/*
 * control_task.h
 *
 * Created: 5/15/2020 8:21:59 PM
 *  Author: cichy
 */
 #include<ATMEGA_FreeRTOS.h>
 #include<event_groups.h>
#include<lora_driver.h>

#include "globals.h"

typedef struct control_bundle control_t;

control_t* control_create(lora_payload_t* payload, bundle_t* readings, EventGroupHandle_t egroup, EventBits_t read_done, EventBits_t message_done);

void control_task(void* control_bundle);