/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: L'exercice demandé ici consiste à faire atténuer progressivement la luminosité d'une DEL.
 * Autrement dit, de faire comme si la DEL s'éteignait en 3 secondes plutôt qu'abruptement.
 * Le problème consiste à faire une atténuation de la couleur rouge dans un premier temps et de la couleur verte dans un deuxième temps.
 * Version: 1.1
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <util/delay_basic.h>

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

void delay(int time) 
{
  for(int i = 0; i < time; i++)
    _delay_us(1);
}

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree

  const uint16_t b = 1000; // us freq = 1mhz <=> b = 1/1000 <=> b = 1ms
  const uint16_t nbReps = 3000;

  uint8_t difference = 0;

  for(int a = b; a > 0; a--)
  {
    for(int i = 0; i < nbReps; i++)
    {
      difference = b - a;
      PORTB = RED;
      delay(a);
      PORTB = OFF;
      delay(difference++);
    }
    difference = 0;
  }
  return 0;
}
