/*
 * co2_sensor.h
 *
 * Created: 5/15/2020 6:21:29 PM
 *  Author: cichy
 */ 
#include<stdlib.h>

#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>
#include<semphr.h>

#include "mh_z19.h"

typedef struct co2_sensor co2_t;

co2_t *co2_create(EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready);

uint16_t* co2_get_data_pointer(co2_t* self);

void co2_task(void* param);