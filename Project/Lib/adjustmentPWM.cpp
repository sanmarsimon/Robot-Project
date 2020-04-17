#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay_basic.h>
#include "adjustmentPWM.h"


void adjustmentPWM::startMotor (uint8_t roueGauche, uint8_t roueDroite) volatile
{
	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF

	TCNT1 = 0;	//Timer a 0

	OCR1A = roueDroite;	// PD5 ne peux pas etre utilisé il est controlé par OCR1A

	OCR1B = roueGauche;	// PD4 ne peux pas etre utilisé il est controlé par OCR1B

	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);

	// division d'horloge par 8 - implique une frequence de PWM fixe
	TCCR1B |= (1 << CS11) ; // Divise l'horloge par 8

	TCCR1C = 0;


}

void adjustmentPWM::stopPWMSignal() volatile
{
	TCNT1 = 0;	//Counter a 0

	TCCR1A &= (0<< COM1A1) | (0 << COM1B1);		// Les ports PD4 et PD5 redeviennent disponibles à l'utilisation
}