#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../Lib/customprocs.h"
#include "../../Lib/lcm_so1602dtr_m_fw.h"


#define DEMO_DDR	DDRB // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTB // Port AVR occup� par l'afficheur

volatile double distance_gauche ;
volatile double distance_devant;
volatile double distance_droite;
char affichageGauche[5];
char affichageDevant[5];
char affichageDroite[5];

// Ecran LCD
LCM ecranLCD(&DEMO_DDR, &DEMO_PORT);

// Fonction qui retourne le message à afficher en fonction de la distance
char* messageDistance(double distance)
{
    if (distance < 1.00)
    {
        return "DNGR";
    }
    else if ((distance >= 1.00) && (distance < 3.00))
    {
        return "ATTN";
    }
    else
    {
        return " OK ";  // 2 espaces sont rajoutés pour avoir un affichage uniforme
    }
}

// Conversion double en string
void conversion(double a,char* b )
{
    uint8_t decimale =(uint8_t(a*10)%10);
    uint8_t entier = (uint8_t(a)%10);

    b[0]='0'+entier;
    b[1]='.';
    b[2]='0'+decimale;
}

void initialization() 
{
    cli();		                            // cli est une routine qui bloque toutes les interruptions
    DDRB = 0xFF;                            // PORT B est en mode sortie
    DDRA = 0xF8;                            // PA0-PA2 en entrée le reste en sortie    
    DDRC = 0xFF;                            // PORT C en sortie

    /* 0b1111 0111 PD3(bouton poussoir)est en mode entree, PD4 et PD5(PWM), 
    PD7 et PD2(LED droite), PD0 et PD1(LED gauche) sont en sorties */
    DDRD = 0xf7;  

    EIMSK |= (1 << INT1);     // Activer les interruptions externes sur INT1
    EICRA |= (1 << ISC11) | (1 << ISC10);   // regler INT1 pour declencher au front montant uniquement

    sei();				                    // sei permet de recevoir à nouveau des interruptions.
}

int main()
{   
    initialization();
    while(1)
    {
        /************************************************************************
        ************************** LECTURE DU SONAR GAUCHE **********************
        *************************************************************************/

        int timeCounter = 0;
        PORTB |= (1 << PINB0) ;         // On trigger le sonar
        _delay_us(10);
        PORTB &= ~(1 << PINB0);

        while(!(PINA & (1 << PA0))){
            /* On attends le début de la période haute */
        }

        while (PINA & (1 << PA0))
        {
            /* On attends la fin de la période haute. On compte le temps avant le retour de l'echo */
            timeCounter++;
            _delay_us(9);
            if (timeCounter > 3600)                         // 3600 = 400cm * 9 (400 cm est la valeur maximale)
            {
                timeCounter = 0;
                break;
            }
        }
        distance_gauche = (timeCounter/10)/58.0;
        conversion(distance_gauche, affichageGauche);
        ecranLCD.write(affichageGauche, 0);
        ecranLCD << "m";                                    // unité de distance en metre
        ecranLCD[16] = messageDistance(distance_gauche);    // OK ATTN DNGR en fonction de la distance

        /*************************************************************************
        ************************** LECTURE DU SONAR DE DEVANT ********************
        **************************************************************************/

        timeCounter = 0;
        PORTB |= (1 << PINB0) ;         // On trigger le sonar
        _delay_us(10);
        PORTB &= ~(1 << PINB0);

        while(!(PINA & (1 << PA1))){
            /* On attends le début de la période haute */
        }
        while (PINA & (1 << PA1))
        {
            /* On attends la fin de la période haute. On compte le temps avant le retour de l'echo */
            timeCounter++;
            _delay_us(9);
            if (timeCounter > 3600)                           // 3600 = 400cm * 9 (400 cm est la valeur maximale)
            {
                timeCounter = 0;
                break;
            }
        }
        distance_devant = (timeCounter/10)/58.0;
        conversion(distance_devant, affichageDevant);
        ecranLCD.write(affichageDevant, 6);
        ecranLCD << "m";                                      // unité de distance en metre              
        ecranLCD[22] = (messageDistance(distance_devant));    // OK ATTN DNGR en fonction de la distance

        /*************************************************************************
        ************************** LECTURE DU SONAR DROIT ************************
        **************************************************************************/

        timeCounter = 0;
        PORTB |= (1 << PINB0) ;         // On trigger le sonar
        _delay_us(10);
        PORTB &= ~(1 << PINB0);

        while(!(PINA & (1 << PA2))){
            /* On attends le début de la période haute */
        }

        while (PINA & (1 << PA2))
        {
            /* On attends la fin de la période haute. On compte le temps avant le retour de l'echo */
            timeCounter++;
            _delay_us(9);
            if (timeCounter > 3600)                         // 3600 = 400cm * 9 (400 cm est la valeur maximale)
            {
                timeCounter = 0;
                break;
            }
        }
        distance_droite = (timeCounter/10)/58.0;
        conversion(distance_droite, affichageDroite);
        ecranLCD.write(affichageDroite, 11);
        ecranLCD << "m";                                    //unité de distance en metre
        ecranLCD[28] = (messageDistance(distance_droite));  // OK ATTN DNGR en fonction de la distance
    }  
    return 0;
}




