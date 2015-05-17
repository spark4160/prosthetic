NAME=prosthetic
INCLUDE=serial.c time.c adc.c
EXTINCLUDE=-I/usr/include
EXTLINK=-L/usr/lib/libfftw3.so -lfftw3
CFLAGS=-mmcu=atmega328 -O3 -std=c99 -Wno-int-to-pointer-cast -Wno-aggressive-loop-optimizations
PROGRAMMER=-c arduino
FAST=-B 1
SLOW=-B 16
DEVICE=-P /dev/ttyACM0
MICRO=-p ATmega328P

all: o elf install
o:
	avr-gcc ${CFLAGS} -o ${NAME}.o ${NAME}.c ${INCLUDE} ${EXTINCLUDE}
elf:
	avr-ld -mavr3 -o ${NAME}.elf ${NAME}.o
install:
	sudo avrdude ${FAST} ${PROGRAMMER} ${DEVICE} ${MICRO} -U flash:w:${NAME}.elf
slowinstall:
	sudo avrdude ${SLOW} ${PROGRAMMER} ${DEVICE} ${MICRO} -U flash:w:${NAME}.elf

# fuses:
# 	sudo avrdude ${SLOW} ${PROGRAMMER} ${DEVICE} ${MICRO} -U lfuse:w:0xc7:m -U hfuse:w:0xd9:m

# bluetooth:
# 	sudo rfcomm bind 0 20:14:02:18:10:58
# 	screen /dev/rfcomm0 57600
