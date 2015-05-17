#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#include "serial.h"

#define ADC0 0b00000000
#define ADC1 0b00000001

#define SENSOR_MODE ADC0
#define SENSOR_ACT ADC1

#define SENSORS 2
#define SAMPLE_COUNT 64

uint8_t sampling;
volatile uint8_t sensor;

volatile uint16_t samples[SENSORS][SAMPLE_COUNT];
volatile uint8_t samples_indices[SENSORS];

// Sum
// 16 bit sum = 2^16 max
// 10 bit adc = 2^10 max
// sample count = 2^16/2^10 = 2^6 = 64 = SAMPLE_COUNT
volatile uint16_t sum[SENSORS];

void adc_init(void);

void adc_sample(void);

uint16_t adc_get(uint8_t pin);

ISR(ADC_vect);

#endif