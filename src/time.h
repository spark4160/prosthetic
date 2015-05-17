#ifndef TIME_H_
#define TIME_H_

#include "macro.h"

#include <avr/interrupt.h>

#include "serial.h"
#include "adc.h"

// Variables
uint16_t ticks;
uint16_t millis_l;
uint16_t millis_h;

// Methods
void time_init(void);

void time_start(void);

uint16_t time_ticks(void);

uint16_t time_ms(void);

uint16_t time_s(void);

// Interrupts
ISR(TIMER1_COMPB_vect);

#endif