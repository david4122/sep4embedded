
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

typedef struct tempHum_sensor tempHum_t;

tempHum_t *tempHum_create(EventGroupHandle_t egroup, EventBits_t bit);
void tempHum_task(void *param);