/*
 * UTS_beacon.c
 *
 * Created: 29.5.2019 22:31:23
 * Author : kubas
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define UTS_POS (1<<3)
#define UTS_NEG (1<<4)
#define UTS_PINS (UTS_POS | UTS_NEG)

#define TRANSMIT 20
#define SILENCE (16000 - TRANSMIT)

#define FREQ 40000

volatile uint8_t pins_to_change = 0;
volatile uint16_t timer = 0;

ISR(TIM0_COMPA_vect) {
	PINB = pins_to_change;
	++timer;
	if (pins_to_change == 0) {
		if (timer == SILENCE) {
			PORTB |= UTS_POS;
			pins_to_change = UTS_PINS;
			timer = 0;
		}
	} else if (timer == TRANSMIT) {
		PORTB &= ~UTS_PINS;
		pins_to_change = 0;
		timer = 0;
	}
}

int main(void)
{
	DDRB = UTS_PINS;
	OCR0A = F_CPU / (FREQ * 2);
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS00);
	TIMSK0 = (1<<OCIE0A);
	sei();
    while (1) {}
}

