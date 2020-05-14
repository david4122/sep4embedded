#pragma once
#include "../LoRa/LoraDriver.h"
#include "../Sensor/Sensor.h"
#include "../TaskController/TaskController.h"

#include <stdio.h>
#include <time.h>

#include "task.h"
#include "timers.h"


//Call method for creating everything and starts task scheduler
void start_simulation(void);
//Initializes all tasks, sensors, event group and semaphore
void initialize_tasks_and_sensors(void);
