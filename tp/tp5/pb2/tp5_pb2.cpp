/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ce programme redirige les caractères ASCII affichables à l'écran.
 * Version: 1.1
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "memoire_24.h"

void initializationUART()
{
    // 2400 bauds. Nous vous donnons la valeur des deux premier registres pour vous éviter des complications
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    UCSR0A |= (1 << UDRE0);
    // UCSR0A |= (1 << RXC0 ) | (1 << TXC0 );
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // Format des trames: 8 bits, 1 stop bits, none parity
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // p.187
}

// De l'USART vers le PC
void transmissionUART(uint8_t data)
{
    // Wait for the transmit buffer to empty
    while(!(UCSR0A & (1 << UDRE0))) {}     
    // Put the data into the buffer and the send the data. p. 174
    UDR0 = data;       
}

int main()
{
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie

    initializationUART();
    char mots[21] = "Le robot en INF1900\n";
    uint8_t i, j;
    for(i = 0; i < 100; i++)
    {
        for(j=0; j < 20; j++) 
        {
            transmissionUART (mots[j]);
        }
    }
}
