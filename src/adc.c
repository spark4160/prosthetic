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

	// Remove old reading from sum
	sum[sensor] -= samples[sensor][samples_indices[sensor]];

	// Add new reading to sum
	sum[sensor] += reading;

	// Store new reading
	samples[sensor][samples_indices[sensor]] = reading;

	// Increment sample index
	samples_indices[sensor]++;
	if(samples_indices[sensor] == SAMPLE_COUNT) {
		samples_indices[sensor] = 0;
	}

	// Go to next sensor
	sensor++;
	if(sensor == SENSORS) {
		sensor = 0;
	}

	// Set ADCn
	ADMUX	= (ADMUX & 0xF0) |
	        (sensor & 0x0F);

	// Start ADC
	ADCSRA |= (1<<ADSC);
}

uint16_t adc_get(uint8_t pin){
	return sum[pin]/SAMPLE_COUNT;
}

ISR(ADC_vect) {
	adc_sample();
}