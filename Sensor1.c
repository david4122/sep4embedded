#include <stdlib.h>
#include "Sensor1.h"
#include "better_print.h"

int driver_initialized = 0;

void sensor1_create(void) {
	if(!driver_initialized) {
		mh_z19_create(ser_USART3, NULL);
		driver_initialized = 1;
	}
}

void sensor1_measure(uint16_t* data) {
	bprintf("BEFORE MEASURE: ");
	bprintf_int(data);
	mh_z19_return_code_t return_code_co2_measurement = mh_z19_take_meassuring();
	bprintf("MEASUREMENT TAKEN");
	vTaskDelay(1000);
	bprintf("AFTER DELAY");
	if(return_code_co2_measurement == MHZ19_OK) {
		bprintf("MEASUREMENT OK");
		uint16_t measurement_uint;
		mh_z19_get_co2_ppm(&measurement_uint);
		bprintf_int(measurement_uint);
		*data = measurement_uint;	// !!!
		bprintf_int(data);
	} else {
		bprintf("NO OK");
	}
	bprintf("MEASURE FINISHED");
}