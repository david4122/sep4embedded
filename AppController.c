
#include "config.h"
#include "globals.h"
#include "AppController.h"
#include "better_print.h"
#include "co2_sensor.h"
#include "lora_task.h"
#include "control_task.h"

const int Sensor1_Delay = 2000;
const int Sensor2_Delay = 2200;
const int Event_group_delay = 2100;

void create_sensors_eventGroup_semaphore_and_tasks() {
	bprintf_init();
	
	EventGroupHandle_t eventGroupHandler = xEventGroupCreate(); // TODO ifs
	
	lora_payload_t* payload = malloc(sizeof(lora_payload_t));
	payload->len = LORA_PAYLOAD_LENGTH;
	payload->port_no = 1;
	payload->bytes[0] = 0;
	
	xTaskCreate(lora_init_task, "LORA INIT TASK", configMINIMAL_STACK_SIZE, eventGroupHandler, 4, NULL);

	co2_t* co2 = co2_create(eventGroupHandler, CO2_SENSOR_BIT);
	xTaskCreate(
		co2_task, (const portCHAR*) "CO2 sensor", configMINIMAL_STACK_SIZE, (void*) co2, 0, NULL);
	//xTaskCreate(
	//	simulationOfSensor2, (const portCHAR*) "Simulation of sensor 2", configMINIMAL_STACK_SIZE, (void*) eventGroupHandler, 0, NULL);
		
	bundle_t* readings = bundle_create(co2_get_data_pointer(co2));
	control_t* control_bundle = control_create(
			payload, readings, eventGroupHandler, SENSORS_BITS, LORA_BIT);
	 xTaskCreate(
		control_task, (const portCHAR*) "Control task", configMINIMAL_STACK_SIZE, (void*) control_bundle, 3, NULL);
		
	lora_t* lora = lora_create(payload, eventGroupHandler, LORA_BIT);
	xTaskCreate(
		lora_task, (const portCHAR*) "LoraDriver simulation", configMINIMAL_STACK_SIZE, (void*) lora, 0, NULL);

}

void initialize(void) {
	create_sensors_eventGroup_semaphore_and_tasks();
	vTaskStartScheduler();
}

void simulationOfSensor2(void* pvParameters) {
	EventGroupHandle_t eventGroupHandler2 = (EventGroupHandle_t) pvParameters;
	
	xEventGroupWaitBits(eventGroupHandler2, LORA_READY_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	puts("SENSOR2 READY");

	for (;;) {
		vTaskDelay(READ_DELAY);
		puts("SENSOR 2 AFTER DELAY");
		//EventBits_t bits = xEventGroupSetBits(eventGroupHandler2, TEMP_HUM_BIT);
		//printf("SENSOR2 SET BITS: %d\n", bits);
	}
}


