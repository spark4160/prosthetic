#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#include "serial.h"

#define ADC0 0b00000000
#define ADC1 0b00000001

#define SENSOR_BICEP ADC0
#define SENSOR_WRIST ADC1

#define SENSORS 2
#define SAMPLE_COUNT 128
#define SPARSE_SAMPLE_INTERVAL (1<<12)
#define SPARSE_SAMPLE_COUNT 32

#define AVG_LONG 128
#define AVG_SHORT 16

uint8_t sampling;
volatile uint8_t sensor;
volatile uint32_t samples_count[SENSORS];

volatile uint16_t samples[SENSORS][SAMPLE_COUNT];
volatile uint8_t samples_indices[SENSORS];

volatile uint16_t samples_sparse[SENSORS][SPARSE_SAMPLE_COUNT];
volatile uint8_t samples_sparse_indices[SENSORS];

// Sum
volatile uint32_t sum_full[SENSORS];
volatile uint32_t sum_sparse[SENSORS];

void adc_init(void);

void adc_sample(void);

uint16_t adc_get(uint8_t pin);

ISR(ADC_vect);

#endif