/*
 * better_printf.c
 *
 * Created: 5/14/2020 10:17:51 PM
 *  Author: cichy
 */ 
#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>

#include <ATMEGA_FreeRTOS.h>
#include "semphr.h"
#include "better_print.h"

SemaphoreHandle_t printSemaphore;

void bprintf_init() {
	if(printSemaphore == NULL) {
		printSemaphore = xSemaphoreCreateMutex();
		if(printSemaphore != NULL) {
			xSemaphoreGive(printSemaphore);
		}
	}
}

void bprintf_int(uint16_t n) {
	if(xSemaphoreTake(printSemaphore, portMAX_DELAY)) {
		vTaskDelay(100);
		printf("%d\n", n);
		fflush(stdout); 
		vTaskDelay(100);
		xSemaphoreGive(printSemaphore);
	}
}

void bprintf(const char* str) {
	if(xSemaphoreTake(printSemaphore, portMAX_DELAY)) {
		puts(str);
		fflush(stdout); 
		xSemaphoreGive(printSemaphore);
	}
}

void bprintCallback(void(*callback_function_example)(lora_payload_t*), lora_payload_t* arg) {
	if(xSemaphoreTake(printSemaphore, portMAX_DELAY)) {
		(*callback_function_example)(arg);
		xSemaphoreGive(printSemaphore);
	}
}