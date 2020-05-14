
#include <stdlib.h>
#include "Sensor2.h"

typedef struct Sensor2 {
	uint8_t data;
} sensor;

const uint8_t example_values_sensor2[7] = { 3, 6, 7, 10, 12, 13, 14 };

sensor2_t sensor2_create(void) {
	sensor2_t newSensor = calloc(sizeof(sensor), 1);
	if (newSensor == NULL) {
		return NULL;
	}
	newSensor->data = 0;
	return newSensor;
}
sensor2_t sensor2_destroy(sensor2_t self) {
	free(self);
}
void sensor2_measure(sensor2_t self) {
	self->data = example_values_sensor2[rand() % 7 + 0];
}
uint8_t sensor2_getData(sensor2_t self) {
	return self->data;
}