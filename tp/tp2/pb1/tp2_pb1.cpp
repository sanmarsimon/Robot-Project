/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Les compteurs sont une forme de machines à états.
 * On veut ici simplement que la DEL soit éteinte au départ.
 * On doit appuyer et relâcher 5 fois le bouton-poussoir avant que la DEL tourne au rouge pendant exactement 1 seconde.
 * Par la suite, on revient au départ pour pouvoir recommencer.
 * Version: 1.1
 */

/* Table 1: Tableau des états
---------------------------------------------------------------
 Etat present    Entree DDRD     Etat suivant    Sortie DDRB       
    INIT             0              INIT            0x00
    INIT             1               E1             0x00
    E1               0               E1             0x00
    E1               1               E2             0x00
    E2               0               E2             0x00
    E2               1               E3             0x00
    E3               0               E3             0x00
    E3               1               E4             0x00
    E4               0               E4             0x00
    E4               1               E5             0x00
    E5               0              INIT            0x01
--------------------------------------------------------------- */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h> 

const uint8_t RED = 0x01;
const uint8_t OFF = 0x00;

bool debounce()
{
  if(PIND & 0x04)
  {
    _delay_ms(50);
    while(PIND & 0x04) {}
    return true;
  }
  return false;
}

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree

  enum State {Init, E1, E2, E3, E4, E5};

  State currentState = Init; 
  
  for(;;)  // boucle sans fin
  {
    switch(currentState)
    {
      case Init : 
        if(debounce())
          currentState = E1; 
          _delay_ms(100);
        break;
          
      case E1 : 
        if(debounce())
          currentState = E2;
          _delay_ms(100);
        break;

      case E2 : 
        if(debounce())
          currentState = E3; 
          _delay_ms(100);
        break;

      case E3 : 
        if(debounce())
          currentState = E4;
          _delay_ms(100);
        break;

      case E4 : 
        if(debounce()) 
          currentState = E5;
          _delay_ms(100); 
        break;

      case E5 : 
        PORTB = RED;          //DEL est rouge
        _delay_ms(1000);      //Le DEL brille pendant 1 seconde
        PORTB = OFF;          //DEL est eteint
        currentState = Init;
        break;
    }
  }
  return 0; 
}

