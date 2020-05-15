#include "AppController.h"
#include "better_print.h"
#include "co2_sensor.h"

const int Sensor1_Delay = 2000;
const int Sensor2_Delay = 2200;
const int Event_group_delay = 2100;

#define LORA_PAYLOAD_LENGTH 2

#define EVENT_GROUP_DELAY (2100)
#define CO2_SENSOR_BIT (1 << 0)
#define SENSOR2_BIT (1 << 1)
#define SENSORS_BITS (CO2_SENSOR_BIT && SENSOR2_BIT)

const uint32_t Sensor1_bit = (1 << 0);
const uint32_t Sensor2_bit = (1 << 1);
const uint32_t Both_sensors_bits = (1 << 0) | (1 << 1);
const uint32_t Lora_bit = (1 << 2);

EventGroupHandle_t eventGroupHandler;
SemaphoreHandle_t printOutsGateKeeper;

sensor2_t sensor2;

lora_payload_simulation_t messageLora;

struct lora_bundle {
	lora_payload_t* lora_payload;
	bundle_t* readings;
	EventGroupHandle_t egroup;
	uint32_t sbits;
};

lora_bundle_t* lora_bundle_create(lora_payload_t* payload, bundle_t* readings, EventGroupHandle_t egroup, uint32_t sbits) {
	lora_bundle_t* bundle = malloc(sizeof(lora_bundle_t));
	if(!bundle)
		return NULL;
		
	bundle->lora_payload = payload;
	bundle->readings = readings;
	bundle->egroup = egroup;
	bundle->sbits = sbits;
	
	return bundle;
}

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

void create_sensors_eventGroup_semaphore_and_tasks() {
	bprintf_init();
	
	sensor2 = sensor2_create();
	
	eventGroupHandler = xEventGroupCreate(); // TODO ifs
	
	lora_payload_t* payload = malloc(sizeof(lora_payload_t));
	payload->len = LORA_PAYLOAD_LENGTH;
	payload->port_no = 1;
	payload->bytes[0] = 0;


	co2_t* co2 = co2_create(eventGroupHandler, CO2_SENSOR_BIT);
	xTaskCreate(
		co2_task, (const portCHAR*)"CO2 sensor", configMINIMAL_STACK_SIZE, (void*) co2, 0, NULL);
	xTaskCreate(
		simulationOfSensor2, (const portCHAR*)"Simulation of sensor 2", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
		
	bundle_t* readings = bundle_create(co2_get_data_pointer(co2));
	lora_bundle_t* lora_bundle = lora_bundle_create(payload, readings, eventGroupHandler, SENSORS_BITS);
	xTaskCreate(
		controlTask, (const portCHAR*)"Control task", configMINIMAL_STACK_SIZE, (void*) lora_bundle, 0, NULL);
	xTaskCreate(
		loraTask, (const portCHAR*)"LoraDriver simulation", configMINIMAL_STACK_SIZE, NULL, 0, NULL);

}

void initialize(void) {
	srand(time(NULL));
	create_sensors_eventGroup_semaphore_and_tasks();
	bprintf("ESW Sensor simulation started \n");
	vTaskStartScheduler();
}

//void simulationOfSensor1(void* pvParameters) {
	//co2_t bundle = (co2_t) pvParameters;
//
	//mh_z19_create(ser_USART3, NULL);
//
	//for(;;) {
		//sensor1_measure(bundle->co2);
		//vTaskDelay(Sensor1_Delay);
		//xEventGroupSetBits(bundle->egroup, Sensor1_bit);
	//}
//}

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
	lora_bundle_t* bundle = (lora_bundle_t*) pvParameters;
	
	while(1) {
		uint32_t bitsResult = xEventGroupWaitBits(bundle->egroup, bundle->sbits, pdTRUE, pdTRUE, EVENT_GROUP_DELAY);
		if (bitsResult == bundle->sbits) {
			uint16_t sensor1Data = *bundle->readings->co2;
			
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

