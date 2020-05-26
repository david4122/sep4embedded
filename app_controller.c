#include<stdlib.h>
#include<stdbool.h>

#include "config.h"
#include "globals.h"
#include "app_controller.h"
#include "co2_sensor.h"
#include "temp_hum_sensor.h"
#include "lora_adapter.h"
#include "lora_task.h"
#include "control_task.h"

#ifdef VERBOSE
#include "safeprint.h"
#endif


int freeMem() {
	int size = 8 * 1024;
	uint8_t *b;
	while(size > 0 && (b = malloc(--size)) == NULL);
	free(b);
	return size;
}


bool create_tasks() {
#ifdef VERBOSE
	int mem = freeMem();
	safeprintln_int("FREE MEM: ", mem);
#endif

	EventGroupHandle_t egroup = xEventGroupCreate();
	if(!egroup) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not allocate event group");
#endif
		return false;
	}
	
#ifdef VERBOSE
	mem = freeMem();
	safeprintln_int("FREE MEM: ", mem);
#endif

	
	// TODO move
	lora_payload_t* payload = malloc(sizeof(lora_payload_t));
	if(!payload) {
		mem = freeMem();
		
		printf("ERROR");
		return false;
	}
	payload->len = LORA_PAYLOAD_LENGTH;
	payload->port_no = 1;
	payload->bytes[0] = 0;
	
	xTaskCreate(lora_init_task,
			"LORA INIT TASK",
			configMINIMAL_STACK_SIZE,
			egroup,
			4,
			NULL);


#ifdef VERBOSE
	mem = freeMem();
	safeprintln_int("FREE MEM: ", mem);
#endif			
			
	co2_t* co2 = co2_create(egroup, CO2_SENSOR_BIT);
	if(co2 == NULL) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not initialize co2 sensor");
#endif
		return false;
	}

	xTaskCreate(co2_task,
			(const portCHAR*) "CO2 sensor",
			configMINIMAL_STACK_SIZE,
			(void*) co2,
			1,
			NULL);
			
		
	tempHum_t* temphum = tempHum_create(egroup, TEMP_HUM_BIT);
	if(!temphum) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not initialize temp/hum sensor");
#endif
		return false;
	}
	
	xTaskCreate(tempHum_task,
			(const portCHAR*) "TempHum sensor",
			configMINIMAL_STACK_SIZE,
			(void*) temphum,
			1,
			NULL);
			
		
	bundle_t* readings = bundle_create(co2_get_data_pointer(co2), get_temp_pointer(temphum), get_hum_pointer(temphum));
	if(!readings) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not allocate packet");
#endif
		return false;
	}
		
	control_t* control = control_create(
			payload, readings, egroup, SENSORS_BITS, LORA_BIT);
	if(!control) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not create control task");
#endif
		return false;
	}
	
	xTaskCreate(control_task,
			 (const portCHAR*) "Control",
			 configMINIMAL_STACK_SIZE,
			 (void*) control,
			 3,
			 NULL);
			 

	lora_t* lora = lora_create(payload, egroup, LORA_BIT);
	if(!lora) {
#ifdef VERBOSE
		safeprintln("[!] [CONTROLLER] Could not initialize lora task");
#endif
		return false;
	}
	
	xTaskCreate(lora_task,
			(const portCHAR*) "Lora",
			configMINIMAL_STACK_SIZE,
			(void*) lora,
			1,
			NULL);

	puts("INIT DONE");

	return true;
}

void initialize(void) {
#ifdef VERBOSE
	int mem = freeMem();
	safeprintln_int("FREE MEM: ", mem);
#endif
	
	if(create_tasks())
		vTaskStartScheduler();
	else
#ifdef VERBOSE
		safeprintln("[F] create_tasks failed");
#endif
}


