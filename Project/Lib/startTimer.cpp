#include "startTimer.h"

void startTimer::Timer(const uint16_t duration)
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