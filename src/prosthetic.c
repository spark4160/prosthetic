#include <stdio.h>

#include "macro.h"

#include "serial.h"
#include "time.h"
#include "adc.h"

#define PULSES 4
#define PULSE_THRESHOLD 512
#define PULSE_COOLDOWN 100
#define PULSE_DOUBLETIME 500

#define HAND_MIN 0
#define HAND_MAX 180
#define HAND_SCALE 180/(1024 - PULSE_THRESHOLD)
#define HAND_TIMEOUT 1000

void main(void) {
	
	// Disable interrupts during setup
	cli();

	// Initialise
	serial_init();
	adc_init();
	time_init();
	
	// Set tick LED pin to output
	DDRB |= (1<<PB0);

	// Loop count
	uint16_t loop_l = 0;
	uint16_t loop_h = 0;

	// Pulse info
	uint8_t pulsing = 0;            // Is a pulse currently being recorded?
	uint8_t pulse = 0;              // ID of current or next pulse
	uint16_t pulse_max[PULSES];     // Max sample from ADC
	uint16_t pulse_start[PULSES];   // Start time, millis
	uint16_t pulse_end[PULSES];     // End time, millis

	// Action info
	uint8_t hand_squeezed = FALSE;  // Is the hand squeezing now?
	uint8_t hand_angle = 0;         // Finger servo angles

	// Enable interrupts
	sei();

	// Zero time
	time_start();

	// Start ADC sample interrupt loop
	adc_sample();
	
	while(1) {
		// Update serial periodically
		if(loop_h%(1<<1) == 0 && loop_l == 0) {
			serial_clear();
			serial_printf("LOOP %08d*2^16\r\n", loop_h);
			serial_printf("TIME %08ds\r\n", time_s());
			// ADC
			serial_printf("MODE AVG %04d\r\n", adc_get(SENSOR_MODE));
			serial_printf("ACT  AVG %04d\r\n", adc_get(SENSOR_ACT));
			// Pulses
			serial_printf("PULSING     %01d\r\n", pulsing);
			serial_printf("PULSE INDEX %01d\r\n", pulse);
			serial_printf("PULSE MAX   %04d\r\n", pulse_max[pulse]);
			serial_printf("PULSE START %04d\r\n", pulse_start[pulse]);
			serial_printf("PULSE END   %04d\r\n", pulse_end[pulse]);
			// Output
			serial_printf("HAND ANGLE %04d\r\n", hand_angle);
			serial_printf("HAND SQUZD %d\r\n", hand_squeezed);
		}

		// Do processing more often
		if(loop_l == 0) {
			// Get input
			uint16_t act = adc_get(SENSOR_ACT);
			
			// Get time
			uint16_t now = time_ms();
			
			// Check for pulse
			if(act > PULSE_THRESHOLD) {
				// Currently a pulse
				// If a pulse is already being measured
				if(pulsing) {
					// Adjust max if exceeded
					if(act > pulse_max[pulse]) {
						pulse_max[pulse] = act;
					}
					// Set end to now
					pulse_end[pulse] = now;
				} else {
					// Check if cooldown has passed
					if(now - pulse_end[pulse] > PULSE_COOLDOWN) {
						// Increment pulse index
						pulse++;
						if(pulse > PULSES) {
							pulse = 0;
						}
						// Start measuring a pulse
						pulse_start[pulse] = now;
						pulse_max[pulse] = act;
						pulsing = TRUE;
					}
				}
			} else {
				// Currently below threshold
				// If a pulse is being measured
				if(pulsing) {
					// Check if cooldown has passed
					if(now - pulse_start[pulse] > PULSE_COOLDOWN) {
						// End pulse
						pulsing = FALSE;
					}
				}
			}

			// Do things
			// If last 2 pulses were in quick succession
			if(pulse_start[pulse] - pulse_end[(pulse - 1)%PULSES] < PULSE_DOUBLETIME) {
				// Set squeezing force relative to the average max level of the pulses
				hand_angle = (pulse_max[pulse] + pulse_max[(pulse - 1)%PULSES])/2;
			} else {
				uint8_t timed_scale = (now - pulse_end[pulse])/HAND_TIMEOUT;
				// If pulsing
				if(pulsing || timed_scale == 0) {
					// Set relative to current max
					hand_angle = pulse_max[pulse];
				} else {
					// Set relative to the last pulse max divided by time since
					hand_angle = pulse_max[pulse]/timed_scale;
				}
			}
		}

		// Increment loop count
		loop_l++;
		if(loop_l == 0) {
			loop_h++;
		}
	}
}