//Author: Krystian Goluch
//Date: 13.05.2020

#include "AppController.h"
#include "../better_print.h"

//Bits for corresponding sensors
const uint8_t bit_for_sensor1 = BIT_FOR_SENSOR1;
const uint8_t bit_for_sensor2 = BIT_FOR_SENSOR2;
const uint8_t bit_for_LoRa = BIT_FOR_LORA;

//-------------------------------------------------------------
//DECLARATIONS OF THE VARIABLES

//Declaration of the event group
EventGroupHandle_t eventGroup;
//Declarations of sensor 1 simulations
sensor_t sensor1;


task_sensor_data_container_t task_sensor_data_container1;
task_sensor_data_container_t task_sensor_data_container2;
task_lora_data_container_t task_lora_data_container;
task_controller_data_container_t task_controller_data_container;

void initialize_tasks_and_sensors() {
	// Init bprintf semaphore
	bprintf_init();
	//-------------------------------------------------------------------
	//Creating a semaphore to protect print outs
	// if (printOutsSemaphore == NULL) {
	//	printOutsSemaphore = xSemaphoreCreateMutex();
	//	if (printOutsSemaphore != NULL) {
	//		xSemaphoreGive(printOutsSemaphore);
	//	}
	// }
	//-------------------------------------------------------------------
	//Crating sensors 
	sensor1 = sensor_create();
	if (sensor1 == NULL) {
		bprintf(" @@ OUT OF MEMORY SENSOR ONE COULDN'T BE CREATED\n");
	}
	//-------------------------------------------------------------------
	//Creating a group
	eventGroup = xEventGroupCreate();
	if (eventGroup == NULL) {
		bprintf(" @@ OUT OF MEMORY EVENT GROUP COULDN'T BE CREATED\n");
	}
	//-------------------------------------------------------------------
	task_sensor_data_container1 = (task_sensor_data_container_t){ sensor1, eventGroup, bit_for_sensor1, SENSOR1_FREQUENCY};
	task_lora_data_container = (task_lora_data_container_t){ eventGroup, bit_for_LoRa, NULL };
	task_controller_data_container = (task_controller_data_container_t){ eventGroup, sensor1, bit_for_sensor1, &task_lora_data_container };
	//Creating tasks
	xTaskCreate(
		simulationOfSensorTask
		, (const portCHAR*)"Sensor 1" //User friendly name of the task
		, configMINIMAL_STACK_SIZE // Minimal stack size from FreeRTOS configuration header
		, &task_sensor_data_container1 // Parameter
		, 0  // Available priority from 0 (the lowest) to 10 (the highest)
		, NULL);
	//xTaskCreate(
		//simulationOfSensorTask
		//, (const portCHAR*)"Sensor 2" //User friendly name of the task
		//, configMINIMAL_STACK_SIZE // Minimal stack size from FreeRTOS configuration header
		//, &task_sensor_data_container2 // Parameter
		//, 0  // Available priority from 0 (the lowest) to 10 (the highest)
		//, NULL);
	xTaskCreate(
		taskController
		, (const portCHAR*)"Task controller" //User friendly name of the task
		, configMINIMAL_STACK_SIZE // Minimal stack size from FreeRTOS configuration header
		, &task_controller_data_container // Parameter
		, 2  // Available priority from 0 (the lowest) to 10 (the highest)
		, NULL);
	xTaskCreate(
		send_upload_message
		, (const portCHAR*)"LoRaDriver simulation" //User friendly name of the task
		, configMINIMAL_STACK_SIZE // Minimal stack size from FreeRTOS configuration header
		, &task_lora_data_container // Parameter
		, 1  // Available priority from 0 (the lowest) to 10 (the highest)
		, NULL);

}

void start_simulation(void) {
	srand((unsigned)time(NULL));
	initialize_tasks_and_sensors();
	bprintf("^^^^^^^^^\n");
	bprintf("Program started\n");
	bprintf("^^^^^^^^^\n");
	vTaskStartScheduler();
}

