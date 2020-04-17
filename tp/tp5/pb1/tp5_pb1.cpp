/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Il faut faire un programme qui écrit la chaîne de caractères «*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire externe.
 * La chaîne commencera à l'adresse 0x00.
 * Faire en sorte que le programme puisse aller relire la chaîne en question.
 * Comparez la chaîne de caractères envoyée à la mémoire et celle relue.
 * Il suffira de faire afficher la DEL en vert si elles sont identiques et en rouge dans le cas contraire. 
 * Version: 1.1
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include "memoire_24.h"

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

int main()
{
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie

	Memoire24CXXX externalMemory = Memoire24CXXX();

	const uint16_t address = 0x00;
	// If the size of the array is omitted, an array just big enough to hold the initialization is created.
	uint8_t textArray[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
	const uint8_t size = sizeof(textArray) - 1;
	uint8_t relectureArray[size] = {};

    // uint8_t ecriture(const uint16_t adresse, uint8_t *donnee, const uint8_t longueur);
	externalMemory.ecriture(address, &textArray, size);
	_delay_ms(10);

    //uint8_t lecture(const uint16_t adresse, uint8_t *donnee, const uint8_t longueur);
	externalMemory.lecture(address, &relectureArray, size);
	_delay_ms(10);

	bool equalCondition = false;
	for(uint8_t i = 0; i < size; i++)
	{
		if(textArray[i] == relectureArray[i])
		{
			equalCondition = true;
		}
		else
		{
			equalCondition = false;
			break;
		}
	}

	if(equalCondition)
	{
		PORTB = GREEN;
		_delay_ms(2000);
		PORTB = OFF;
		_delay_ms(2000);

	}
	else
	{
		PORTB = RED;
		_delay_ms(2000);
		PORTB = OFF;
		_delay_ms(2000);
	}
}
