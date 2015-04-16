#ifndef SERIAL_H_
#define SERIAL_H_

#include "macro.h"

#include <stdio.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

#define BUFFER_LENGTH 512
#define FORMAT_LENGTH 128

uint8_t write_index, read_index;
char buffer[BUFFER_LENGTH];

void serial_init(void);

void serial_send(char *message, uint8_t length);

void serial_printf(const char *format, ...);

void serial_clear();

ISR(USART_TX_vect);

#endif