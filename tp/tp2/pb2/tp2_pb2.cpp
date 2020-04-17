/*
 * Nom: Vy-Phuong Nguyen 1989674 et Sanmar Simon 1938126
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Quand la carte mère démarre, la DEL libre doit s'allumer en rouge.
 * Si le bouton-poussoir noir est pesé, la DEL affiche la couleur ambre.
 * Quand le bouton-poussoir est relâché, la DEL devient verte.
 * Si le bouton est de nouveau pesé, la DEL prend la couleur rouge encore.
 * Quand il est relâché, la DEL s'éteint.
 * Si le bouton est de nouveau pesé, la DEL affiche la couleur verte.
 * Quand il est relâché, la DEL tourne au rouge ce qui fait que la carte mère est de retour à son état initial et tout peut recommencer. 
 * Version: 1.1
 */

/* Table 1: Tableau des états
Legende:
-------------------------------------------------------------------
 Etat present    Entree DDRD     Etat suivant     Sortie DDRB
    INIT             0              INIT             0x01
    INIT             1               E1           0x01 & 0x02
    E1               0               E1              0x02 
    E1               1               E2              0x01
    E2               0               E2              0x00
    E2               1              INIT             0x02
------------------------------------------------------------------- */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>


bool debounce()
{
  if(PIND & 0x04)
  {
    _delay_ms(5);
    if(PIND & 0x04)
    {
      return true;
    }
  }
  return false;
}

const uint8_t RED = 0x01;
const uint8_t GREEN = 0x02;
const uint8_t OFF = 0x00;

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree

  enum State {Init, E1, E2};

  State currentState = Init; 
  
  for(;;)  // boucle sans fin
  {
    switch(currentState)
    {
      case Init : 
        PORTB = RED;
        if(debounce())
        {
          currentState = E1;
        }
        break;
          
      case E1 : 
        while(PIND & 0x04)      //alternance des couleurs rouge et vert rapidement pour produire la
          {                     //couleur ambre dans le DEL pendant que le bouton poussoir est pese
            PORTB = RED;
            _delay_ms(1);
            PORTB = GREEN;
            _delay_ms(1);
          }
        PORTB = GREEN;

        if(debounce())
        {
          while(PIND & 0x04) { PORTB = RED; }    //DEL brille rouge pendant que le bouton poussoir est pese
          currentState = E2;
        }             
        break;

      case E2 : 
        PORTB = OFF;
        if(debounce())
        {
          while(PIND & 0x04) { PORTB = GREEN; }   //DEL brille vert pendant que le bouton poussoir est pese
          currentState = Init;
        } 
        break;
    }
  }
  return 0; 
}
