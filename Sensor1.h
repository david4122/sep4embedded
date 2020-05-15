#pragma once
#include <stdint.h>
#include<ATMEGA_FreeRTOS.h>
#include<task.h>
#include "mh_z19.h"

//Sensor simulation is created
void sensor1_create(void);

//Sensor simulation assigns value from predefined data
void sensor1_measure(uint16_t* data);