#pragma once

#include <stdio.h>
#include <time.h>

#include<ATMEGA_FreeRTOS.h>

#include "lora_driver.h"

typedef struct lora_bundle lora_bundle_t;
typedef struct readings_bundle bundle_t;


void initialize(void);

//Controls and synchronizes all the tasks
void controlTask(void* pvParameters);
