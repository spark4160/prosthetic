#include "adc.h"

void adc_init(void) {
	// Set vairables

	// Registers
	// DDR
	// DDRC &= (0<<PC5)|
	// 	(0<<PC4)|
	// 	(0<<PC3)|
	// 	(0<<PC2)|
	// 	(0<<PC1)|
	// 	(0<<PC0);
	// ADC multiplexer
	// REFS = reference (00 = AREF pin)
	// MUX = ADCn select (0000 = ADC0)
	ADMUX	=	(0<<REFS1)|
	        (1<<REFS0);

	// ADC control
	// ADEN = enable ADC
	// ADSC = start conversion
	// ADPS = clock prescaler select (111 = 128)
	ADCSRA = (1<<ADEN)|
	         (0<<ADSC)|
	         (0<<ADATE)|
	         (0<<ADIF)|
	         (1<<ADIE)|
	         (1<<ADPS2)|
	         (1<<ADPS1)|
	         (1<<ADPS0);
}

void adc_sample(void) {
	// Registers
	// ADC multiplexer
	// MUX = ADCn select (0000 = ADC0)
	// ADCn: ADMUX 3:2:1:0

	// Conversion complete, get reading
	uint16_t reading = ADC;

	// Remove old sample from sum
	sum_full[sensor] -= samples[sensor][samples_indices[sensor]];

	// Store new sample
	samples[sensor][samples_indices[sensor]] = reading;

	// Add new sample to sum
	sum_full[sensor] += reading;

	// Check for sparse sample
	if(samples_count[sensor]%SPARSE_SAMPLE_INTERVAL == 0) {
		uint16_t sparse_value = sum_full[sensor]/SAMPLE_COUNT;

		// Remove old sample from sum
		sum_sparse[sensor] -= samples_sparse[sensor][samples_sparse_indices[sensor]];

		// Store new sample
		samples_sparse[sensor][samples_sparse_indices[sensor]] = sparse_value;

		// Add new sample to sum
		sum_sparse[sensor] += sparse_value;

		// Increment index
		samples_sparse_indices[sensor]++;
		if(samples_sparse_indices[sensor] == SPARSE_SAMPLE_COUNT) {
			samples_sparse_indices[sensor] = 0;
		}
	}

	// Increment indices
	samples_indices[sensor]++;
	if(samples_indices[sensor] == SAMPLE_COUNT) {
		samples_indices[sensor] = 0;
	}
	sensor++;
	if(sensor == SENSORS) {
		sensor = 0;
	}
	samples_count[sensor]++;

	// Set ADCn
	ADMUX	= (ADMUX & 0xF0) |
	        (sensor & 0x0F);
	// Start ADC
	ADCSRA |= (1<<ADSC);
}

uint16_t adc_get(uint8_t pin){
	// uint32_t sum = 0;
	// for(uint8_t i = 0; i < SAMPLE_COUNT; i++) {
	// 	sum += samples[sensor][i];
	// }
	return (uint16_t) sum_full[pin]/SAMPLE_COUNT;
}

ISR(ADC_vect) {
	adc_sample();
}