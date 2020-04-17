/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Faire un robot pouvant se faire bronzer...
 * Si la lumière est basse (en cachant la photoresistance), la DEL prendra la couleur verte.
 * Si la lumière est à un bon niveau (lumière ambiante), la DEL tournera à l'ambre.
 * Si la lumière est trop forte (photoresistance sous une lampe de poche), la DEL devient rouge.
 * Version: 1.1
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;


void initialization() 
{
	DDRB = 0xff; // PORT B est en mode sortie (On connecte la DEL au ports PB0 et PB1)
	DDRA = 0x00; // PORT A est en mode entree (On connecte la photoresistance au port PA0)
	
	PORTB = OFF; 				// On commence eteint
}

int main()
{
	initialization();

	const uint8_t luminositeForte = 240 ;  	// Valeurs donne par charge 
	const uint8_t luminositeFaible = 190 ;	// Entre ces deux valeurs on est à un eclairage ambiant

	can valeurPhotoresistance ; // On construit un objet de la classe can sur lequel on peut appeller la fonction lecture
	
	
	for (;;)
	{
		uint8_t luminosite = (valeurPhotoresistance.lecture(0) >> 2); // On mets à 0 les bits 8 et 9 car inutiles
		
		// While eclairage faible LED verte
		if (luminosite < luminositeFaible )
		{
			PORTB = GREEN;
		}

		// While eclairage moyen LED ambre
		if ((luminosite >= luminositeFaible) && (luminosite <= luminositeForte))
		{                     
			PORTB = RED;		//alternance des couleurs rouge et vert rapidement pour produire la
			_delay_ms(1);		//couleur sur la DEL 
			PORTB = GREEN;
			_delay_ms(5);
		
		}

		// While eclairage fort LED rouge
		if (luminosite > luminositeForte )
		{
			PORTB = RED;
		}
	}

}

