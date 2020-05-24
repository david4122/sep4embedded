
/*
 * TempHumAdapter.c
 *
 * Created: 5/24/2020 10:11:36 PM
 *  Author: cichy
 */ 
#include "TempHumAdapter.h"

struct tempHum_sensor {
	float data_humidity;
	float data_temperature;
	EventGroupHandle_t egroup;
	EventBits_t ready_bit;
};

tempHum_t* tempHum_create(EventGroupHandle_t egroup, EventBits_t bit) {
	if ( HIH8120_OK == hih8120Create() )
	{
		// Driver created OK
		// Always check what hih8120Create() returns
		
	}
}
void tempHum_task(void *param);
