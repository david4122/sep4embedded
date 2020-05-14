#pragma once
#include <stdint.h>
#include "../CustomConfig.h"

//Data holder for sensor includes length and an array of bytes
typedef struct Sensor1* sensor_t;

//Data container for the task parameters
struct DataSensor {
	sensor_t				sensor;
	EventGroupHandle_t		event_group;
	uint8_t					bit_for_sensor;
	int						frequency;
} typedef task_sensor_data_container_t;