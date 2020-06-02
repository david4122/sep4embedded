
/*
 * TempHumAdapter.h
 *
 * Created: 5/24/2020 10:11:19 PM
 *  Author: cichy
 */ 
#include<stdlib.h>

#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>
#include<semphr.h>
#include<hih8120.h>

typedef struct tempHum_sensor tempHum_t;

tempHum_t* tempHum_create(EventGroupHandle_t egroup, EventBits_t wait_for, EventBits_t ready);

float* get_hum_pointer(tempHum_t* self);

float* get_temp_pointer(tempHum_t* self);

void tempHum_task(void *param);