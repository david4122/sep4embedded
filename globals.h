/*
 * Globals.h
 *
 * Created: 5/15/2020 8:30:15 PM
 *  Author: cichy
 */ 
#include<stdint.h>
#include<stdlib.h>

typedef struct readings_bundle bundle_t;

bundle_t* bundle_create(uint16_t* co2, float* temp, float* hum);

uint16_t get_co2(bundle_t* self);

uint8_t co2_get_lower_bits(bundle_t* self);
uint8_t co2_get_higher_bits(bundle_t* self);

float get_humidity(bundle_t* self);
float get_temperature(bundle_t* self);