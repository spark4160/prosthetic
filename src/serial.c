#include "serial.h"

void serial_init(void) {
	// Set vairables
	write_index = 0;
	read_index = 0;

	// Registers
	// Set baud rate
	UBRR0 = MYUBRR;
	// Clear transmit complete register
	// UCSR0A |= (1<<TXC0);
	// Enable transmitter
	UCSR0B |= (1<<TXCIE0)|(1<<TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C |= (1<<USBS0)|(3<<UCSZ00);
}

void serial_send(char *message, uint8_t length) {
	char *char_pointer = message;
	char char_value;
	for(char_value = (char)*(char_pointer); char_value != '\0';) {
		buffer[write_index] = char_value;
		write_index++;
		if(write_index == BUFFER_LENGTH) {
			write_index = 0;
		}
		if(length > 0 && char_pointer - message >= length) {
			char_value = '\0';
		} else {
			char_value = (char)*(++char_pointer);
		}
	}
	if(UCSR0A & (1<<UDRE0)) {
		UDR0 = buffer[read_index];
		read_index++;
	}
}

void serial_printf(const char *format, ...) {
	// Declare formatted array
	char string[FORMAT_LENGTH];
	// Get arguments
	va_list args;
	va_start(args, format);
		// Do formatting
		vsprintf(string, format, args);
	va_end(args);
	// Buffer the result
	serial_send(string, FORMAT_LENGTH);
}

void serial_clear() {
	serial_send("\033[2J\033[1;1H", 10);
}

ISR(USART_TX_vect) {
	if(read_index != write_index) {
		UDR0 = buffer[read_index];
		read_index++;
		if(read_index == BUFFER_LENGTH) {
			read_index = 0;
		}
	}
}