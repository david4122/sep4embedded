#include "AppController.h"
#include "better_print.h"

const int Sensor1_Delay = 2000;
const int Sensor2_Delay = 2200;
const int Event_group_delay = 2100;

const uint32_t Sensor1_bit = (1 << 0);
const uint32_t Sensor2_bit = (1 << 1);
const uint32_t Both_sensors_bits = (1 << 0) | (1 << 1);
const uint32_t Lora_bit = (1 << 2);

EventGroupHandle_t eventGroupHandler;
SemaphoreHandle_t printOutsGateKeeper;

sensor1_t sensor1;
sensor2_t sensor2;

lora_payload_simulation_t messageLora;

void create_sensors_eventGroup_semaphore_and_tasks() {
	bprintf_init();
	sensor1 = sensor1_create();
	sensor2 = sensor2_create();
	eventGroupHandler = xEventGroupCreate();
	
	xTaskCreate(
		simulationOfSensor1
		, (const portCHAR*)"Simulation of sensor 1"
		, configMINIMAL_STACK_SIZE 
		, NULL
		, 0  // Priority 0 being the lowest and 7 being the highest 
		, NULL);
	xTaskCreate(
		simulationOfSensor2
		, (const portCHAR*)"Simulation of sensor 2"
		, configMINIMAL_STACK_SIZE
		, NULL
		, 0  // Priority 0 being the lowest and 7 being the highest 
		, NULL);
	xTaskCreate(
		controlTask
		, (const portCHAR*)"Control task"
		, configMINIMAL_STACK_SIZE
		, NULL
		, 0  // Priority 0 being the lowest and 7 being the highest 
		, NULL);
	xTaskCreate(
		loraTask
		, (const portCHAR*)"LoraDriver simulation"
		, configMINIMAL_STACK_SIZE
		, NULL
		, 0  // Priority 0 being the lowest and 7 being the highest 
		, NULL);

}

void initialize(void) {
	srand(time(NULL));
	create_sensors_eventGroup_semaphore_and_tasks();
	bprintf("ESW Sensor simulation started \n");
	vTaskStartScheduler();
}

void simulationOfSensor1(void* pvParameters) {
	for(;;) {
			sensor1_measure(sensor1);
			vTaskDelay(Sensor1_Delay);
			xEventGroupSetBits(eventGroupHandler, Sensor1_bit);
	}
}

void simulationOfSensor2(void* pvParameters) {
	for (;;) {
			sensor2_measure(sensor2);
			vTaskDelay(Sensor2_Delay);
			xEventGroupSetBits(eventGroupHandler, Sensor2_bit);
	}
}

void loraTask(void* pvParameters) {
	for (;;) {
		uint32_t loraBitResponse = xEventGroupWaitBits(eventGroupHandler, Lora_bit, pdTRUE, pdTRUE, Event_group_delay);
		if (loraBitResponse == Lora_bit) {
			bprintf("LORA BUILDING STRING");
			bprintCallback(sent_upload_messages, &messageLora);
			bprintf("LORA FINISHED BUILDING STRING");
		}
	}
}

void controlTask(void* pvParameters) {
	for(;;) {
		uint32_t bitsResult = xEventGroupWaitBits(eventGroupHandler, Both_sensors_bits, pdTRUE, pdTRUE, Event_group_delay);
		if (bitsResult == Both_sensors_bits) {
			uint16_t sensor1Data = sensor1_getData(sensor1);
			uint16_t sensor2Data = sensor2_getData(sensor2);
			messageLora = (lora_payload_simulation_t){ 2, {sensor1Data, sensor2Data} };
			xEventGroupSetBits(eventGroupHandler, Lora_bit);
		}
		else {
			if (bitsResult != Sensor1_bit) {
				bprintf("Sensor 1 didn't measure data yet\n");
			}
			else if (bitsResult != Sensor2_bit) {
				bprintf("Sensor 2 didn't measure data yet\n");
			}
			else {
				bprintf("Error");
			}
		}
	}
}

