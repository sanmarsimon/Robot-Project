/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: L'exercice demandé ici consiste à faire fonctionner le PWM pour un rapport a/b de 0%, 25%, 50% et 100% pendant 2 secondes chaque.
 * La roue tournera donc 20 secondes au total (les 10 premières à 60 Hz et à 400 Hz pour les 10 autres).
 * Version: 1.1
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <util/delay_basic.h>

const uint8_t ON = 0x01;
const uint8_t OFF = 0x00;
const uint8_t frequency60 = 60;
const uint16_t frequency400 = 400;

void delay(int time) 
{
  for(int i = 0; i < time; i ++)
    _delay_ms(1);
}

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree

  for(;;) 
  {    
    // CASE 60Hz
    int nbReps = 120;
    uint8_t b = (1/frequency60) * 1000;

    for(uint8_t a = 0; a <= b; a += b/4)
    {
      uint8_t difference = b - a;
      for(uint16_t time_counter = 0; time_counter < nbReps; time_counter++)
      {
        PORTB = ON;
        delay(a);     
        PORTB = OFF;
        delay(difference);
      }
    }

    // CASE 400Hz
    nbReps = 800;
    b = (1/frequency400) * 1000;

    for(uint8_t a = 0; a <= b; a += b/4) 
    {
      uint8_t difference = b - a;
      for(uint16_t time_counter = 0; time_counter < nbReps; time_counter++)
      {
        PORTB = ON;
        delay(a);     
        PORTB = OFF;
        delay(difference);
      }
    } 
  }
  return 0;
}
