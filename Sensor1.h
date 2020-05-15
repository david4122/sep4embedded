#pragma once
#include <stdint.h>
#include "mh_z19.h"

typedef struct Sensor1* sensor1_t;
//Sensor simulation is created
sensor1_t sensor1_create(void);
//Sensor simulation is destroyed
void sensor1_destroy(sensor1_t self);
//Sensor simulation assigns value from predefined data
void sensor1_measure(sensor1_t self);
//Returns last measured value
uint16_t sensor1_getData(sensor1_t self);