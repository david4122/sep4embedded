/*
 * better_printf.c
 *
 * Created: 5/14/2020 10:17:51 PM
 *  Author: cichy
 */ 
#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>

#include "CustomConfig.h"
#include "better_print.h"

SemaphoreHandle_t printSemaphore;

void bprintf_init() {
	if (printSemaphore == NULL) {
		printSemaphore = xSemaphoreCreateMutex();
		if (printSemaphore != NULL) {
			xSemaphoreGive(printSemaphore);
		}
	}
}

void bprintf_int(const char* str, uint8_t n) {
	if(xSemaphoreTake(printSemaphore, portMAX_DELAY)) {
		vTaskDelay(100);
		puts(str);
		// printf("%d\n", 10);
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