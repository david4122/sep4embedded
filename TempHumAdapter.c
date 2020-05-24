
/*
 * TempHumAdapter.c
 *
 * Created: 5/24/2020 10:11:36 PM
 *  Author: cichy
 */ 
#include "TempHumAdapter.h"

struct tempHum_sensor {
	uint16_t data;
	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

tempHum_t* tempHum_create(EventGroupHandle_t egroup, EventBits_t bit) {
	
}
void tempHum_task(void *param);
