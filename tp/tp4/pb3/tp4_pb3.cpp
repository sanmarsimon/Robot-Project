/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Il vous faudra donner un signal de direction au circuit du pont en H. 
 * Le seul détail qui change par rapport à la semaine précédente est la fréquence du signal PWM. 
 * On se contentera d'une seule fréquence (donnée plus bas), mais on continuera de générer des signaux PWM de 0%, 25%, 50% et 75% et 100% pour des durées de 2 secondes chacun. 
 * Le rapport a/b du PWM sera réglé en passant correctement un ou des arguments de la fonction. 
 * Version: 1.1
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay_basic.h>

void delay(uint16_t time) 
{
  for(uint16_t i = 0; i < time; i ++)
    _delay_ms(1);
}

void ajustmentPWM (uint8_t duration) 
{
	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)

	TCNT1 = 0;	//Timer a 0

	OCR1A = duration;

	OCR1B = duration;

	// division d'horloge par 8 - implique une frequence de PWM fixe

	TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B0) | (1 << WGM10);

	TCCR1B |= (1 << CS11); // Divise l'horloge par 8

	TCCR1C = 0;
}

void initialization() 
{
	cli();		 // cli est une routine qui bloque toutes les interruptions
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0xff; // PORT D est en mode entree

	EIMSK |= (1 << INT0);  // Activer les interruptions externes
	EICRA |= (1 << ISC01) | (1 << ISC00); // regler INT0 pour declencher n'importe quel changement de logique
	sei();				   // sei permet de recevoir à nouveau des interruptions.
}

int main()
{
	initialization();

	for(;;) 
	{    
		uint16_t nbReps = 2000; // 2 seconds
		for(uint8_t velocity = 0; velocity <= 255; velocity += 255/4)
		{
			ajustmentPWM(velocity);
			delay(nbReps);
		}
	}
	return 0;
}

