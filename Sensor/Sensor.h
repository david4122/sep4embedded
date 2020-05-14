#pragma once
#include "DataSensor.h"

//Memory is allocated and pointer to the sensor is returned
sensor_t sensor_create(void);
//Sensor is destroyed and memory is freed
void sensor_destroy(sensor_t self);
//Sensor updates its data in the memory
void sensor_measure(sensor_t self);
//Gets data from the sensor
uint8_t sensor_getData(sensor_t self);
//Task for sensor simulation
void simulationOfSensorTask(void* args);
