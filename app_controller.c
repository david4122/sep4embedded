#include<stdbool.h>

#include "config.h"
#include "globals.h"
#include "app_controller.h"
#include "better_print.h"
#include "co2_sensor.h"
#include "temp_hum_sensor.h"
#include "lora_adapter.h"
#include "lora_task.h"
#include "control_task.h"


void create_tasks() {

	EventGroupHandle_t egroup = xEventGroupCreate();
	if(!egroup) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}
	
	// TODO move
	lora_payload_t* payload = malloc(sizeof(lora_payload_t));
	payload->len = LORA_PAYLOAD_LENGTH;
	payload->port_no = 1;
	payload->bytes[0] = 0;
	
	xTaskCreate(lora_init_task,
			"LORA INIT TASK",
			configMINIMAL_STACK_SIZE,
			egroup,
			4,
			NULL);
			
			
	co2_t* co2 = co2_create(egroup, CO2_SENSOR_BIT);
	if(!co2) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}

	xTaskCreate(co2_task,
			(const portCHAR*) "CO2 sensor",
			configMINIMAL_STACK_SIZE,
			(void*) co2,
			0,
			NULL);
			
		
	tempHum_t* temphum = tempHum_create(egroup, TEMP_HUM_BIT);
	if(!temphum) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}
	
	xTaskCreate(tempHum_task,
			(const portCHAR*) "TempHum sensor",
			configMINIMAL_STACK_SIZE,
			(void*) temphum,
			0,
			NULL);
			
		
	bundle_t* readings = bundle_create(co2_get_data_pointer(co2), get_temp_pointer(temphum), get_hum_pointer(temphum));
	if(!readings) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}
		
	control_t* control = control_create(
			payload, readings, egroup, SENSORS_BITS, LORA_BIT);
	if(!control) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}
	
	xTaskCreate(control_task,
			 (const portCHAR*) "Control",
			 configMINIMAL_STACK_SIZE,
			 (void*) control,
			 3,
			 NULL);
			 

	lora_t* lora = lora_create(payload, egroup, LORA_BIT);
	if(!lora) {
		puts("[!] [CONTROLLER] Could not allocate event group");
		return;
	}
	
	xTaskCreate(lora_task,
			(const portCHAR*) "Lora",
			configMINIMAL_STACK_SIZE,
			(void*) lora,
			0,
			NULL);
}

void initialize(void) {
	bprintf_init();
	
	create_tasks();
	
	vTaskStartScheduler();
}


