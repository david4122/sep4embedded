/*
 * config.h
 *
 * Created: 5/15/2020 8:12:25 PM
 *  Author: cichy
 */ 
#pragma once

#define LORA_PAYLOAD_LENGTH 4
#define READ_DELAY 2000

// #define EVENT_GROUP_DELAY (2100)

#define LORA_READY_BIT	( 1 << 0 )
#define CO2_SENSOR_BIT	( 1 << 1 )
#define TEMP_HUM_BIT	( 1 << 2 )
#define SENSORS_BITS	( CO2_SENSOR_BIT | TEMP_HUM_BIT )
#define LORA_BIT		( 1 << 3 )

#define LORA_APP_EUI "80BE5102E3968E56"
#define LORA_APP_KEY "A9518CB9DBD95CA82F3C12F5D87AE2F6"

//#ifndef DEBUG_MODE
//#define DEBUG_MODE
//#endif

#define VERBOSE

int freeMem();