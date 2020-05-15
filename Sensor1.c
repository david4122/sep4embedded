#include <stdlib.h>
#include "Sensor1.h"
#include "better_print.h"

typedef struct Sensor1 {
	uint16_t data;
} sensor;

int driver_initialized = 0;

sensor1_t sensor1_create(void) {
	sensor1_t newSensor = calloc(sizeof(sensor), 1);
	if (newSensor == NULL) {
		return NULL;
	}
	newSensor->data = -9;
	if(!driver_initialized) {
		mh_z19_create(ser_USART3, NULL);
		driver_initialized = 1;
		puts("DRIVER INITIALIZED");
	}
	return newSensor;
}
void sensor1_destroy(sensor1_t self) {
	free(self);
}
void sensor1_measure(sensor1_t self) {
	bprintf("BEFORE MEASURE: ");
	bprintf_int(self->data);
	mh_z19_return_code_t return_code_co2_measurement = mh_z19_take_meassuring();
	bprintf("MEASUREMENT TAKEN");
	vTaskDelay(1000);
	bprintf("AFTER DELAY");
	if(return_code_co2_measurement == MHZ19_OK) {
		bprintf("MEASUREMENT OK");
		uint16_t measurement_uint;
		mh_z19_get_co2_ppm(&measurement_uint);
		bprintf_int(measurement_uint);
		self->data = measurement_uint;
		bprintf_int(self->data);
	} else {
		bprintf("NO OK");
	}
	bprintf("MEASURE FINISHED");
}
uint16_t sensor1_getData(sensor1_t self) {
	return self->data;
}