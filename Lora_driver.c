#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>

#include "LoraDriver.h"
#include "config.h"

void sent_upload_messages(lora_payload_t* lora_payload) {
	printf("\tPackage [ ");
	for (uint8_t i = 0; i < lora_payload->len; i++)
	{
		printf("%d ", lora_payload->bytes[i]);
	}
	printf("] is sent by LoRa\n");
}

void lora_init_task(void* arg) {
	EventGroupHandle_t handle = (EventGroupHandle_t) arg;

	puts("LORA INIT TASK");
	lora_driver_reset_rn2483(1); // Activate reset line
	vTaskDelay(2);
	lora_driver_reset_rn2483(0); // Release reset line
	vTaskDelay(150);
	lora_driver_rn2483_factory_reset();
	lora_driver_flush_buffers(); // get rid of first version string from module after reset!
	
	if (lora_driver_rn2483_factory_reset() != LoRA_OK)
	{
		puts("factory reset failed");
	}
	
	if (lora_driver_configure_to_eu868() != LoRA_OK)
	{
		puts("eu686 configuration failed");
	}
	
	static char dev_eui[17]; // It is static to avoid it to occupy stack space in the task
	
	if (lora_driver_get_rn2483_hweui(dev_eui) != LoRA_OK)
	{
		puts("getting hweui failed");
	}
	
	if (lora_driver_set_otaa_identity(LORA_APP_EUI, LORA_APP_KEY, dev_eui) != LoRA_OK)
	{
		puts("setting identity failed");
	}
	
	if (lora_driver_save_mac() != LoRA_OK)
	{
		puts("saving config failed");
	}
	
	if (lora_driver_join(LoRa_OTAA) == LoRa_ACCEPTED)
	{
		puts("managed to join the network");
	}
	puts("INIT SUCESSFUL");
	
	xEventGroupSetBits(handle, LORA_READY_BIT);
	
	vTaskDelete(NULL);
}