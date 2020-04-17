#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL







uint16_t lireCapteur(){

PORTB |= (1<<0);
_delay_us(10);
PORTB |= (0<<0);
_delay_us(8/F_CPU);

//start timer somehow
TCNT0=0x00;
TCCR0B |= (1<<CS01);
//OCR0A = Valeur max ;
unsigned valmax=100;

while ((PINA & (1<<PINA0))&& (TCNT0<valmax)){

}

TCCR0B |= (0<<CS01);

return TCNT0;
}