#include "AppController.h"
#include "better_print.h"
#include "co2_sensor.h"

const int Sensor1_Delay = 2000;
const int Sensor2_Delay = 2200;
const int Event_group_delay = 2100;

#define LORA_PAYLOAD_LENGTH 2
#define READ_DELAY 2000

#define EVENT_GROUP_DELAY (2100)
#define CO2_SENSOR_BIT (1 << 0)
#define TEMP_HUM_BIT (1 << 1)
#define SENSORS_BITS (CO2_SENSOR_BIT && TEMP_HUM_BIT)

#define LORA_BIT (1 << 2)

struct lora_bundle {
	lora_payload_t* lora_payload;
	bundle_t* readings;
	EventGroupHandle_t egroup;
	EventBits_t read_done;
	EventBits_t meassage_done;
};

lora_bundle_t* lora_bundle_create(lora_payload_t* payload, bundle_t* readings, EventGroupHandle_t egroup, EventBits_t read_done, EventBits_t message_done) {
	lora_bundle_t* bundle = malloc(sizeof(lora_bundle_t));
	if(!bundle)
		return NULL;
		
	bundle->lora_payload = payload;
	bundle->readings = readings;
	bundle->egroup = egroup;
	bundle->read_done = read_done;
	bundle->meassage_done = message_done;
	
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
	
	EventGroupHandle_t eventGroupHandler = xEventGroupCreate(); // TODO ifs
	
	lora_payload_t* payload = malloc(sizeof(lora_payload_t));
	payload->len = LORA_PAYLOAD_LENGTH;
	payload->port_no = 1;
	payload->bytes[0] = 0;


	co2_t* co2 = co2_create(eventGroupHandler, CO2_SENSOR_BIT);
	xTaskCreate(
		co2_task, (const portCHAR*) "CO2 sensor", configMINIMAL_STACK_SIZE, (void*) co2, 0, NULL);
	xTaskCreate(
		simulationOfSensor2, (const portCHAR*) "Simulation of sensor 2", configMINIMAL_STACK_SIZE, (void*) eventGroupHandler, 0, NULL);
		
	bundle_t* readings = bundle_create(co2_get_data_pointer(co2));
	lora_bundle_t* lora_bundle = lora_bundle_create(payload, readings, eventGroupHandler, SENSORS_BITS, LORA_BIT);
	
	xTaskCreate(
		controlTask, (const portCHAR*) "Control task", configMINIMAL_STACK_SIZE, (void*) lora_bundle, 0, NULL);
	xTaskCreate(
		loraTask, (const portCHAR*) "LoraDriver simulation", configMINIMAL_STACK_SIZE, (void*) lora_bundle, 0, NULL);

}

void initialize(void) {
	srand(time(NULL));
	create_sensors_eventGroup_semaphore_and_tasks();
	bprintf("ESW Sensor simulation started \n");
	vTaskStartScheduler();
}

void simulationOfSensor2(void* pvParameters) {
	EventGroupHandle_t eventGroupHandler = (EventGroupHandle_t) pvParameters;
	for (;;) {
		vTaskDelay(READ_DELAY);
		xEventGroupSetBits(eventGroupHandler, TEMP_HUM_BIT);
	}
}

void loraTask(void* pvParameters) {
	lora_bundle_t* bundle = (lora_bundle_t*) pvParameters;
	for (;;) {
		uint32_t loraBitResponse = xEventGroupWaitBits(bundle->egroup, bundle->meassage_done, pdTRUE, pdTRUE, EVENT_GROUP_DELAY);
		if (loraBitResponse == bundle->meassage_done) {
			bprintf("LORA BUILDING STRING");
			bprintCallback(sent_upload_messages, bundle->lora_payload);
			bprintf("LORA FINISHED BUILDING STRING");
		}
	}
}

void controlTask(void* pvParameters) {
	lora_bundle_t* bundle = (lora_bundle_t*) pvParameters;
	
	while(1) {
		uint32_t bitsResult = xEventGroupWaitBits(bundle->egroup, bundle->read_done, pdTRUE, pdTRUE, EVENT_GROUP_DELAY);
		if (bitsResult == bundle->read_done) {
			
			bundle->lora_payload->bytes[0] = *bundle->readings->co2 >> 8;
			bundle->lora_payload->bytes[1] = *bundle->readings->co2 & 0xFF;
			
			xEventGroupSetBits(bundle->egroup, bundle->meassage_done);
		}
		else {
			if (bitsResult != CO2_SENSOR_BIT) {
				bprintf("Sensor 1 didn't measure data yet\n");
			}
			else if (bitsResult != TEMP_HUM_BIT) {
				bprintf("Sensor 2 didn't measure data yet\n");
			}
			else {
				bprintf("Error");
			}
		}
	}
}

