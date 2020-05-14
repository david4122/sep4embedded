#pragma once
#include <stdint.h>

typedef struct Sensor2* sensor2_t;
//Sensor simulation is created
sensor2_t sensor2_create(void);
//Sensor simulation is destroyed
sensor2_t sensor2_destroy(sensor2_t self);
//Sensor simulation assigns value from predefined data
void sensor2_measure(sensor2_t self);
//Returns last measured value
uint8_t sensor2_getData(sensor2_t self);