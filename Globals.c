/*
 * Globals.c
 *
 * Created: 5/15/2020 8:31:19 PM
 *  Author: cichy
 */ 
#include "globals.h"

struct readings_bundle {
	uint16_t* co2;
	// TOOD rest of readings
};

bundle_t* bundle_create(uint16_t* co2) {
	bundle_t* res = malloc(sizeof(bundle_t));
	if(!res)
		return NULL;
	
	res->co2 = co2;
	
	return res;
}

uint8_t co2_get_lower_bits(bundle_t* self) {
	return *self->co2 >> 8;
}


uint8_t co2_get_higher_bits(bundle_t* self) {
	return *self->co2 & 0xFF;
}