#include <stdlib.h>
#include "Sensor1.h"

typedef struct Sensor1 {
	uint16_t data;
} sensor;

const uint8_t example_values_sensor1[7] = { 2, 8, 9, 11, 15, 17, 21 };

sensor1_t sensor1_create(void) {
	sensor1_t newSensor = calloc(sizeof(sensor), 1);
	if (newSensor == NULL) {
		return NULL;
	}
	newSensor->data = 0;
	return newSensor;
}
void sensor1_destroy(sensor1_t self) {
	free(self);
}
void sensor1_measure(sensor1_t self) {
	self->data = example_values_sensor1[rand() % 7 + 0];
}
uint16_t sensor1_getData(sensor1_t self) {
	return self->data;
}