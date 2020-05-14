#pragma once
#include "LoraDriver.h"
#include "Sensor1.h"
#include "Sensor2.h"

#include <stdio.h>
#include <time.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "timers.h"
#include <semphr.h>
#include "event_groups.h"

//Initializes and runs the simulation
void initialize(void);
//Creates sensors, event group, sempahore that controls print outs and tasks
void create_sensors_eventGroup_semaphore_and_tasks(void);
//Simulator of sensor 1, measures data in the specified period of time
void simulationOfSensor1(void* pvParameters);
//Simulator of sensor 2, measures data in the specified period of time
void simulationOfSensor2(void* pvParameters);
//Simulator of Lora prints out Data Package to the console
void loraTask(void* pvParameters);
//Controls and synchronizes all the tasks
void controlTask(void* pvParameters);
