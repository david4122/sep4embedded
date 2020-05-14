#include<stdlib.h>

#include "TaskController.h"
#include "../better_print.h"

//Bits for corresponding sensors
const uint8_t task_controller_bit_for_sensor1 = BIT_FOR_SENSOR1;
const uint8_t task_controller_bit_for_sensor2 = BIT_FOR_SENSOR2;
const uint8_t task_controller_bit_for_LoRa = BIT_FOR_LORA;

void taskController(void* args) {
	uint32_t bit_from_sensor_1;
	
	uint8_t sensor1Data;
	uint8_t sensor2Data;
	
	lora_payload_simulation_t* messageLora;
	
	task_controller_data_container_t* task_controller_data_container = (task_controller_data_container_t*)args;
	messageLora = (lora_payload_simulation_t*) pvPortMalloc(sizeof(lora_payload_simulation_t));
	while (1) {
		//Read bits from both sensors to check
		bit_from_sensor_1 = xEventGroupWaitBits(task_controller_data_container->event_group, task_controller_data_container->bit_sensor_1, pdTRUE, pdTRUE, EVENT_GROUP_WAITING_TIME);
		

		//Log a proper information depending on the response
		if (bit_from_sensor_1 != task_controller_bit_for_sensor1) {
			bprintf(" Log: Sensor one is still measuring\n");
		}
		else {
			bprintf(" Log: Sensor one is finished with the measurement and waiting for upload of the data\n");
		}
		

		//If both sensors are ready to send the data set a bit for LoRa to activate
		if ((bit_from_sensor_1) == (unsigned)(task_controller_bit_for_sensor1)) {
			bprintf("Bit set");
			//sensor1Data = sensor_getData(task_controller_data_container->sensor1);
			sensor1Data = 2;
			sensor2Data = 4;
			//Declaration of data holder for LoRa package
			messageLora->len = 2;
			messageLora->bytes[0] = sensor1Data;
			messageLora->bytes[1] = sensor2Data;
			
			task_controller_data_container->loraContainer->message = messageLora;
			xEventGroupSetBits(task_controller_data_container->event_group, task_controller_data_container->loraContainer->bit_for_lora);
		}
	}
}