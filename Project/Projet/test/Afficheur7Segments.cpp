#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../../Lib/adjustmentPWM.h"
#include "../../Lib/amber.h"
#include "../../Lib/can.h"
#include "../../Lib/InitInterrupt.h"
#include "../../Lib/memoire_24.h"
#include "../../Lib/startTimer.h"
#include "../../Lib/tmr2.h"
#include "../../Lib/AfficheurSegment.h"
#include "../../Lib/Debug.h"

 


//Bloc Parametres AFficheur 7 Segments
////////////////////////////////////////////////
AfficheurSegment afficheur = AfficheurSegment();
volatile uint8_t positionAfficheur = 0;
tmr2 timer2;
volatile uint8_t compteur2 =0;
ISR(TIMER2_COMPA_vect){
   cli();
   positionAfficheur++;
   if (positionAfficheur >= 5)
   {
      positionAfficheur = 0;
   }
   afficheur.afficherValeur(positionAfficheur);

   sei();
}
////////////////////////////////////////////////


int main(){
   DDRA = 0xFF;
   DDRB = 0xFF;
   DDRC = 0xFF;
   DDRD = 0x00;
   timer2.demarrerTimer();
   
   for(;;){

      //Exemple de code d'afficheur
      afficheur.off();//Met des tirets sur tous les affichages
      
      _delay_ms(3000);
      for (uint8_t i = 0; i < 100; i++)
      {
         for (uint8_t j = 0; j < 100; j++)
         {
            afficheur.etablirPourcentages(i,j);
            _delay_ms(10);
         }
      }
      


   } 
};
    
