
//Écrit par Nicholas Legrand
#include <avr/io.h>
#include <util/delay.h>

#include "../Lib/uart.h"
#define F_CPU 8000000UL
#define BAUD 2400



void uart::initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux

// premier registres pour vous éviter des complications

UBRR0H = 0;

UBRR0L = 0xCF;

// permettre la réception et la transmission par le UART0

UCSR0A |= (1<<RXC0) | (1<<TXC0);

UCSR0B = (1<<RXEN0)|(1<<TXEN0) ;

// Format des trames: 8 bits, 1 stop bits, none parity

UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

}
void uart::transmissionUART ( uint8_t donnee ) {

/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) );
/* Put data into buffer, sends the data */
UDR0 = donnee;

}
void uart::transmissionStringUART ( char donnee[] ){

   


for (uint8_t i=0;donnee[i]!='\0';i++){
    
        transmissionUART(donnee[i]);
    }
}