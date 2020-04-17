/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Le défi est de concevoir un jeu de réflexe. 
 * Quand le microcontrôleur démarre, il attend 10 secondes et fait clignoter la lumière rouge pendant 1/10 de seconde. 
 * Quand la lumière est éteinte, le joueur doit peser sur le bouton aussitôt que possible. 
 * Si le joueur pèse sur le bouton à l'intérieur d'une seconde, la DEL devient verte. 
 * Si le joueur pèse sur le bouton passé une seconde ou ne pèse pas du tout, la lumière prend la couleur rouge. 
 * Dans un cas comme dans l'autre, la DEL conserve sa couleur indéfiniment. 
 * Il faut peser sur reset pour recommencer.
 * Version: 1.1
 */
#define F_CPU 8000000UL
#define F_CPU_DIVIDED_BY_1024 7813
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay_basic.h>

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

volatile uint8_t timerExpired = 0;
volatile uint8_t pushButton = 0;

ISR(TIMER1_COMPA_vect)
{
	timerExpired = 1;
}

ISR(INT0_vect)
{
	//anti-rebond
	_delay_ms(30);
	if (PIND & 0x04)
	{
		pushButton = 1;
	}
	else
	{
		pushButton = 0;
	}
	EIFR |= (1 << INTF0); // Puttin th INTF0 bit at 0. so it clear the interruption flag
}

void startTimer(uint16_t duration)
{
	timerExpired = 0;
	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée
	TCNT1 = 0; // initialise le timer a 0

	OCR1A = duration; // define the top value for the counter, hence also its resolution

	TCCR1A = 0; // Timer/Counter 1 control register A
	// TCCR1A |= (1 << COM1A0); Active OCnA/OCnB en Compare Match

	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); // divise l'horloge par 1024 VOIR MaxEmbedded section AVR Timers

	TCCR1C = 0;

	TIMSK1 |= (1 << OCIE1A); 	// On active ici le bit output compare interrupts enable A
}								// du time/counter interrupt mask register

void initialization() 
{
	cli();		 // cli est une routine qui bloque toutes les interruptions
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	EIMSK |= (1 << INT0);  // Activer les interruptions externes
	EICRA |= (1 << ISC01) | (1 << ISC00); // regler INT0 pour declencher n'importe quel changement de logique
	sei();				   // sei permet de recevoir à nouveau des interruptions.
}

int main()
{
	initialization();
	for(;;)
	{
		_delay_ms(10000); 		// delai de 10 seconde au demarage
		PORTB = RED;	
		_delay_ms(100);  		// del allumer en rouge pendant 1/10 seconde
		PORTB = OFF;   			// del s'eteint

		// On lance le timer ici appel à startTimer
		startTimer(F_CPU_DIVIDED_BY_1024); // L'énoncé dit de diviser par 1024
		
		do {
			// attendre qu'une des deux variables soit modifiée
			// par une ou l'autre des interruptions.
		} while ( timerExpired == 0 && pushButton == 0 );
			// Une interruption s'est produite. Arrêter toute
			// forme d'interruption. Une seule réponse suffit.
		
		cli();
		
		// Verifier la réponse
		while (timerExpired == 0 && pushButton == 1)
		{
			PORTB = GREEN;		// Si l'interruption viens du bouton alors VICTORE
		}
		
		while (timerExpired == 1 && pushButton == 0)
		{
			PORTB = RED;		// Si l'interruption viens du timer alors DEFAITE
		}
		return 0; 
	}
}

