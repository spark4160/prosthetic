#ifndef TIME_H_
#define TIME_H_

#include "macro.h"

#include <avr/interrupt.h>

#include "serial.h"
#include "adc.h"

// Variables
int32_t ticks;

// Methods
void time_init(void);

void time_start(void);

int32_t time_ticks(void);

int32_t time_ms(void);

// Interrupts
ISR(TIMER1_COMPB_vect);

#endif