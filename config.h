/*
 * config.h
 *
 * Created: 5/15/2020 8:12:25 PM
 *  Author: cichy
 */ 
#define LORA_PAYLOAD_LENGTH 2
#define READ_DELAY 2000

#define EVENT_GROUP_DELAY (2100)
#define CO2_SENSOR_BIT (1 << 0)
#define TEMP_HUM_BIT (1 << 1)
#define SENSORS_BITS (CO2_SENSOR_BIT && TEMP_HUM_BIT)

#define LORA_BIT (1 << 2)