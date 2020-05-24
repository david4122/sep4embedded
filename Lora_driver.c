#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>
#include<stdbool.h>

#include "LoraDriver.h"
#include "config.h"

void sent_upload_messages(lora_payload_t* lora_payload) {
	e_LoRa_return_code_t rc;
	if ((rc = lora_driver_sent_upload_message(false, lora_payload)) == LoRa_MAC_TX_OK )
	{
		puts("LORA message sent");
	} else {
		printf("LORA send message failed: %d\n", rc);
	}
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
	
	printf("LORA INIT eui: ");
	for(int i=0; i<17; i++)
		printf("%d ", dev_eui[i]);
	puts("");
	
	if (lora_driver_set_otaa_identity(LORA_APP_EUI, LORA_APP_KEY, dev_eui) != LoRA_OK)
	{
		puts("setting identity failed");
	}
	
	if (lora_driver_save_mac() != LoRA_OK)
	{
		puts("saving config failed");
	}
	
	e_LoRa_return_code_t ret = lora_driver_join(LoRa_OTAA);
	if (ret == LoRa_ACCEPTED)
	{
		puts("LORA INIT managed to join the network");
	} else {
		printf("LORA INIT failed to join network: %d\n", ret);
	}
	puts("INIT SUCESSFUL");
	
	xEventGroupSetBits(handle, LORA_READY_BIT);
	
	vTaskDelete(NULL);
}