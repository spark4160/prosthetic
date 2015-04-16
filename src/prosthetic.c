#include <stdio.h>

#include "serial.h"
#include "time.h"
#include "adc.h"

void main(void) {
	
	cli();

	serial_init();
	adc_init();
	time_init();
	
	DDRB |= (1<<PB0);
	uint32_t loop = 0;
	
	sei();

	time_start();
	adc_sample();
	
	while(1) {
		if(loop%5000 == 0) {
			serial_clear();
			serial_printf("   LP %08ld\n\r", loop);
			serial_printf(" TIME %08ldms\r\n", time_ms());
			// ADC
			serial_printf("BICEP\r\n");
			serial_printf(" AVG SAMPLE %04d\r\n", (uint16_t) (sum_full[SENSOR_BICEP]/SAMPLE_COUNT));
			serial_printf(" AVG SPARSE %04d\r\n", (uint16_t) (sum_sparse[SENSOR_BICEP]/SPARSE_SAMPLE_COUNT));
		}
		loop++;
	}
}