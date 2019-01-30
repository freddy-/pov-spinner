//make clean
//make
//make program

#define F_CPU 8000000L

#include "avr/interrupt.h"
#include "avr/pgmspace.h"
#include "util/delay.h"
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#define ON_TIME 80
#define OFF_TIME 350

const uint8_t data[] PROGMEM = {
	0x00, 0x7f, 0x09, 0x09, 0x09, 0x01, 0x00, // F
	0x00, 0x7f, 0x09, 0x19, 0x29, 0x46, 0x00, // R
	0x00, 0x7f, 0x49, 0x49, 0x49, 0x41, 0x00, // E
	0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00, // D
	0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00, // D
	0x00, 0x07, 0x08, 0x70, 0x08, 0x07, 0x00	// Y
};

ISR(INT1_vect){
	GIMSK &= ~(1 << INT1);
	MCUCR &= ~(1 << SE);
	cli();
}

void draw(void) {
	uint8_t i;
	for (i = sizeof(data); i > 0; i--) {
		PORTB = ~pgm_read_byte(data + i - 1);
		_delay_us(ON_TIME);
		PORTB = 0xFF;
		_delay_us(OFF_TIME);
	}
}

void sleep(void) {
	GIMSK |= (1 << INT1);
	MCUCR |= (1 << SM0);
	MCUCR |= (1 << SE);

	sei();
	asm("SLEEP");
}

int main(void) {
	//disable watch dog timer
	//disable USI

	DDRB = 0xFF;
	DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD4);
	DDRD &= ~(1 << PD3);

	PORTB = 0xFF;
	PORTD |= (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3);

	while(1) {
		draw();

		if ((PIND & (1 << PD3)) == 0) {
			_delay_ms(200);
			PORTB = 0xFF;
			PORTD |= (1 << PD6) | (1 << PD5) | (1 << PD4);
			sleep();
			_delay_ms(200);
		}

		_delay_us(20 * OFF_TIME);
	}
}






