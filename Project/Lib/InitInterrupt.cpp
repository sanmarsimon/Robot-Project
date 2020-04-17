#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../Lib/InitInterrupt.h"

void InitInterrupt::initialization() 
{
	cli();		 // cli est une routine qui bloque toutes les interruptions
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	EIMSK |= (1 << INT0);  // Activer les interruptions externes
	EICRA |= (1 << ISC01) | (1 << ISC00); // regler INT0 pour declencher n'importe quel changement de logique
	sei();				   // sei permet de recevoir à nouveau des interruptions.
}
