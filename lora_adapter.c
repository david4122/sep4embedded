#include<stdbool.h>
#include<stdio.h>

#include<ATMEGA_FreeRTOS.h>
#include<event_groups.h>

#include "lora_driver.h"
#include "config.h"

#ifdef DEBUG_MODE

void sent_upload_messages(lora_payload_t* payload) {
	printf("[>] Lora payload [%d] {", payload->len);
		for(int i=0; i<payload->len; i++)
		printf("%d ", payload->bytes[i]);
	puts("}");
}

#else

void sent_upload_messages(lora_payload_t* lora_payload) {	
	e_LoRa_return_code_t rc;
	if((rc = lora_driver_sent_upload_message(false, lora_payload)) == LoRa_MAC_TX_OK)
	{
		puts("[*] [LORA] message sent");
	} else {
		printf("[*] [LORA] send message failed: %d\n", rc);
	}
}

#endif

void lora_init_task(void* arg) {
	EventGroupHandle_t handle = (EventGroupHandle_t) arg;

	while(1) {

#ifdef DEBUG_MODE

		puts("[*] [LORA INIT] DEBUGGING MODE");

#else

		lora_driver_reset_rn2483(1); // Activate reset line
		vTaskDelay(2);
		lora_driver_reset_rn2483(0); // Release reset line
		vTaskDelay(150);
		lora_driver_rn2483_factory_reset();
		lora_driver_flush_buffers(); // get rid of first version string from module after reset!
	
		if(lora_driver_rn2483_factory_reset() != LoRA_OK)
		{
			puts("[!] [LORA INIT] Factory reset failed");
			continue;
		}
	
		if(lora_driver_configure_to_eu868() != LoRA_OK)
		{
			puts("[!] [LORA INIT] eu686 configuration failed");
			continue;
		}
	
		static char dev_eui[17]; // It is static to avoid it to occupy stack space in the task
		if(lora_driver_get_rn2483_hweui(dev_eui) != LoRA_OK)
		{
			puts("[!] [LORA INIT] Getting hweui failed");
			continue;
		}
	
		printf("[*] [LORA INIT] Lora eui: ");
		for(int i=0; i<17; i++)
			printf("%d ", dev_eui[i]);
		puts("");
	
		if(lora_driver_set_otaa_identity(LORA_APP_EUI, LORA_APP_KEY, dev_eui) != LoRA_OK)
		{
			puts("[!] [LORA INIT] Setting identity failed");
			continue;
		}
	
		e_LoRa_return_code_t ret;
		if((ret = lora_driver_join(LoRa_OTAA)) != LoRa_ACCEPTED) {
			printf("[!] [LORA INIT] failed to join network: %d\n", ret);
			continue;
		}
		puts("[*] [LORA INIT] managed to join the network");

#endif

		xEventGroupSetBits(handle, LORA_READY_BIT);
		
		puts("LORA INIT DONE");
	
		vTaskDelete(NULL);
	}
}