/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Quand le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé. 
 * Quand le bouton est relâché ou lorsque le compteur atteint 120, la lumière clignote vert pendant 1/2 seconde. 
 * Ensuite, la carte mère ne fait rien. 
 * Puis, deux secondes plus tard, la lumière rouge s'allume. 
 * Elle devra clignoter (compteur / 2) fois au rythme de 2 fois par seconde. 
 * Ensuite, la lumière tourne au vert pendant une seconde. 
 * Finalement, elle s'éteint et le robot revient à son état original.
 * Version: 1.1
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

const uint16_t F_CPU_DIVIDED_BY_1024 = 7812; // Ca prend une seconde au TCNT1 pour compter jusqua 7812
const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

const uint8_t QUARTER_SEC_DELAY = 250;
const uint16_t HALF_SEC_DELAY = 500;
const uint16_t ONE_SEC_DELAY = 1000;

const uint8_t COUNTER_LIMIT = 120;

volatile uint8_t counter = 0;
volatile bool timerExpired = 0;
volatile bool buttonPressed = 0;

void startTimer(const uint16_t duration)
{
	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée
	TCNT1 = 0; // initialise le timer a 0

	OCR1A = duration; // La valeur TOP jusque laquelle le TCNT1 doit compter

	TCCR1A = (1<< WGM12); // Timer/Counter 1 control register A en mode CTC

	TCCR1B = (1 << CS12) | (1 << CS10); // divise l'horloge par 1024 

	TCCR1C = 0;

	TIMSK1 |= (1 << OCIE1A); 	// On active ici le bit output compare interrupts enable A
}								// du time/counter interrupt mask register

ISR(INT0_vect) 
{
	// Bouton poussoir Breadbord enfoncé signifie valeur de la broche D2 = 0
	// Relaché signifie valeur broche D2 = 1
	_delay_ms(30);	// anti-rebond	
	if(PIND & 0x04)
	{
		buttonPressed = 0;
	}
	else
	{
		buttonPressed = 1;
		startTimer(F_CPU_DIVIDED_BY_1024);
	}
	EIFR |= (1 << INTF0); // On met le INTF0 bit à 0. ainsi ca efface le flag de l'interruption
}

ISR(TIMER1_COMPA_vect)
{
	// Chaque seconde il y'a une interruption du timer et donc on incrémente le compteur de 10
	counter += 10;
	if (counter == COUNTER_LIMIT)
	{
		timerExpired = 1;
	}
	else
	{
		startTimer(F_CPU_DIVIDED_BY_1024);
	}
}

void initialization() 
{
	cli();		 // cli est une routine qui bloque toutes les interruptions

	DDRB = 0xff; // PORT B est en mode sortie (PORT B0-B1 connecté à la LED)
	DDRD = 0x00; // PORT D est en mode entree (bouton poussoir connecté à PORTD2)

	counter = 0;

	EIMSK |= (1 << INT0);  // Activer les interruptions externes
	EICRA |= (1 << ISC00); // regler INT0 pour declencher n'importe quel changement de logique

	sei();				   // sei permet de recevoir à nouveau des interruptions.
}

int main()
{
	for(;;)
	{
		initialization();
		while (buttonPressed == 0)
		{
			// On ne fait rien au début tant que le bouton n'est pas appuyé pour commencer
			PORTB = OFF;
		}

		do {
			PORTB = OFF;
			// Tant que bouton est appuyé et 
			// que compteur n'est pas 120 on ne fait rien
		} while (timerExpired == 0 && buttonPressed == 1);
		
		// Une interruption s'est produite. Arrêter toute
		// forme d'interruption. Une seule réponse suffit.
		cli();

		// Séquence couleur ici :

		PORTB = GREEN;
		_delay_ms(HALF_SEC_DELAY);
		PORTB = OFF;
		_delay_ms(2*ONE_SEC_DELAY);

		for(uint8_t i = 0; i < (counter/2); i++) 	// Boucle de 2 secondes qu'on repète (compteur/2) fois
		{
			PORTB = RED;
			_delay_ms(QUARTER_SEC_DELAY);
			PORTB = OFF;
			_delay_ms(QUARTER_SEC_DELAY);
			PORTB = RED;
			_delay_ms(QUARTER_SEC_DELAY);
			PORTB = OFF;
			_delay_ms(QUARTER_SEC_DELAY);
		}

		PORTB = GREEN;
		_delay_ms(ONE_SEC_DELAY);
		PORTB = OFF;
		TCNT1 = 0;					// Reinitialise le compteur de la minuterie pour
}									// Remettre le robot a son etat initial
}

