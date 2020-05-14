#include <stdlib.h>
#include <stdio.h>
#include "Sensor.h"

typedef struct Sensor1 {
	uint8_t data;
} sensor;
//Memory is allocated and pointer to the sensor is returned
sensor_t sensor_create(void) {
	sensor_t newSensor = calloc(sizeof(sensor), 1);
	if (newSensor == NULL) {
		return NULL;
	}
	newSensor->data = 0;
	return newSensor;
}
//Sensor is destroyed and memory is freed
void sensor_destroy(sensor_t self) {
	free(self);
	self = NULL;
}
//Sensor updates its data in the memory
void sensor_measure(sensor_t self) {
	self->data = rand() % 255 + 0;
}
//Gets data from the sensor
uint8_t sensor_getData(sensor_t self) {
	return self->data;
}
//Task for sensor simulation
void simulationOfSensorTask(void* args) {
	task_sensor_data_container_t* task_sensor_data_container = (task_sensor_data_container_t*)args;
	while (1) {
		//Delays task by custom value specified in CustomConfig.h
		vTaskDelay(task_sensor_data_container->frequency);
		//Performs measurement in the sensor and saves the data in it
		sensor_measure(task_sensor_data_container->sensor);
		//Sets a bit assigned to the sensor to inform about successful operation
		xEventGroupSetBits(task_sensor_data_container->event_group, task_sensor_data_container->bit_for_sensor);
	}
}

