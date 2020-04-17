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
#include <avr/interrupt.h>

bool debounce()
{
  if(PIND & 0x04)
  {
    _delay_ms(5);
    if(PIND & 0x04) {
      return true;
    }
  }
  return false;
}

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

volatile uint8_t state = 0; // like static variable but more intense

void initialisation()
{
	cli(); // cli est une routine qui bloque toutes les interruptions
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	EIMSK |= (1 << INT0);  // Activer les interruptions externes
	EICRA |= (1 << ISC00); // regler INT0 pour declencher n'importe quel changement de logique
	sei(); //sei permet de recevoir à nouveau des interruptions.
}

ISR(INT0_vect)  // ISR stops everything the motherboard does to run what is inside it.
{
  _delay_ms(30); //Debounce
  state++;
  if(state == 5)
  {
      state = 0;
  }
  EIFR |= (1 << INTF0); //effacez toute interruption en INT0
}

int main()
{
  initialisation();
  for(;;)  // boucle sans fin
  {
   switch(state)
   {
      case 0 :
      PORTB = RED;  
      if(debounce())
      {
        state = 1;
      } 
      break;

      case 1 : 
        while(PIND & 0x04)      //alternance des couleurs rouge et vert rapidement pour produire la
        {                     //couleur ambre dans le DEL pendant que le bouton poussoir est pese
          PORTB = RED;
          _delay_ms(1);
          PORTB = GREEN;
          _delay_ms(1);
        }
        state = 2;
        break;

      case 2 :
        PORTB = GREEN;
        if (debounce())
        {
          state = 3;
        }
        break;

      case 3 :
          while(PIND & 0x04)
          { 
            PORTB = RED; 
          }    //DEL brille rouge pendant que le bouton poussoir est pese
        state = 4;
        break;

      case 4: 
        PORTB = OFF;
        if(debounce())
        {
          while(PIND & 0x04)
          { 
            PORTB = GREEN; 
          }   //DEL brille vert pendant que le bouton poussoir est pese
          state = 0;
        } 
        break;
    }
  }     
  return 0; 
}
