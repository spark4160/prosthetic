#include "time.h"

// Methods
void time_init(void) {
	// Set vairables
	ticks = 0;
	millis_l = 0;
	millis_h = 0;

	// Registers
	// Set up timer
	// Clock select: no prescaler (16*10^6 Hz)
	// TCCR2B = (0<<CS22)|(0<<CS21)|(1<<CS20);
	// Clock select: /8 prescaler (2*10^6 Hz)
	TCCR2B = (0<<CS22)|(1<<CS21)|(0<<CS20);
	// Clock select: /32 prescaler (500*10^3 Hz)
	// TCCR2B = (0<<CS22)|(1<<CS21)|(1<<CS20);
	// Clock select: /64 prescaler (250*10^3 Hz)
	// TCCR2B = (1<<CS22)|(0<<CS21)|(0<<CS20);
	TIMSK2 |= (1<<OCIE2B);
}

void time_start(void) {
	ticks = 0;
	millis_l = 0;
	millis_h = 0;
}

uint16_t time_ticks(void) {
	return ticks;
}

uint16_t time_ms(void) {
	return millis_l;
}

uint16_t time_s(void) {
	return (millis_l/1000) + (millis_h/1000)*65536;
}

// Interrupts
ISR(TIMER2_COMPB_vect) {
	// Increments at 255
	// /1  Tick frequency is  16*10^6/256 = 62500 > 31250
	// /8  Tick frequency is   2*10^6/256 =  7812 >  3906
	// /32 Tick frequency is 500*10^3/256 =  1953 >   977
	// /64 Tick frequency is 250*10^3/256 =   977 >   488
	ticks++;
	// Every millisecond
	if(ticks%8 == 0) {
		// Add another millisecond
		millis_l++;
		if(millis_l == 0) {
			// Tick on LED
			PORTB ^= (1<<PB0);
			millis_h++;
		}
	}
}