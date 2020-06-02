/*
 * safeprint.c
 *
 * Created: 25/05/2020 19:48:24
 *  Author: sheep
 */ 
#include<stdio.h>
#include<stdarg.h>

#include<ATMEGA_FreeRTOS.h>
#include<semphr.h>

#include "safeprint.h"

static SemaphoreHandle_t printMutex = NULL;

bool safeprint_acquire() {
	if(!printMutex) {
		printMutex = xSemaphoreCreateMutex();
		
		if(!printMutex) {
			return false;
		}
	}
	
	if(xSemaphoreTake(printMutex, (TickType_t) 10) == pdTRUE)
		return true;
	return false;
}

bool safeprint_release() {
	if(!printMutex)
		return false;
		
	if(xSemaphoreGive(printMutex) == pdTRUE)
		return true;
	return false;
}

bool safeprint(const char* str) {
	if(safeprint_acquire()) {
		printf("%s", str);
		
		if(safeprint_release())
			return true;
	}
	return false;
}

bool safeprint_int(const char* prefix, int i) {
	if(safeprint_acquire()) {
		printf("%s%d", prefix, i);
		
		if(safeprint_release())
			return true;
	}
	return false;
}

bool safeprintln(const char* str) {
	if(safeprint_acquire()) {
		puts(str);
		
		if(safeprint_release())
			return true;
	}
	return false;
}

bool safeprintln_int(const char* prefix, int i) {
	if(safeprint_acquire()) {
		printf("%s%d\n", prefix, i);
		
		if(safeprint_release())
			return true;
	}
	return false;
}

bool safeprintln_ints(const char* prefix, int nums, ...) {
	if(safeprint_acquire()) {
		
		va_list args;
		va_start(args, nums);
		
		printf("%s", prefix);
		
		int n;
		for(int i=0; i<nums; i++) {
			n = va_arg(args, int);
			printf("%d ", n);
		}
		puts("");

		if(safeprint_release())
			return true;
	}
	return false;
}

bool safeprint_callbk(void (*func)(void)) {
	if(safeprint_acquire()) {

		func();
		
		if(safeprint_release())
			return true;
	}
	return false;
}