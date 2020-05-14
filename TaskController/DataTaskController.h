#pragma once

#include <stdio.h>
#include <stdint.h>
#include "../CustomConfig.h"
#include "../LoRa/LoraDriver.h"
#include "../Sensor/Sensor.h"

struct DataTaskController {
	EventGroupHandle_t				event_group;
	sensor_t						sensor1;
	uint8_t							bit_sensor_1;
	task_lora_data_container_t* loraContainer;
} typedef task_controller_data_container_t;