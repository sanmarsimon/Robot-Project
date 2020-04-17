#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay_basic.h>
#include "../../Lib/tmr2.h"
#include "../../Lib/AfficheurSegment.h"
#include "../../Lib/customprocs.h"
#include "../../Lib/lcm_so1602dtr_m_fw.h"
#include "../../Lib/adjustmentPWM.h"

#define DEMO_DDR	DDRB // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTB // Port AVR occup� par l'afficheur

// CONSTANTES GLOBALES UTILISES POUR LES DEL
const uint8_t RED_LEFT = (1 << PIND1);
const uint8_t GREEN_LEFT = (1 << PIND0);
const uint8_t RED_RIGHT = (1 << PIND7);
const uint8_t GREEN_RIGHT = (1 << PIND2);
const uint8_t OFF = 0x00;       

// VARIABLES GLOBALES UTILISES POUR LA DETECTION ET L'AFFICHAGE DES DISTANCES
volatile double distance_gauche ;
volatile double distance_devant;
volatile double distance_droite;
char affichageGauche[5];
char affichageDevant[5];
char affichageDroite[5];

// Les deux modes de fonctionnement du robot
enum Mode{detection, manoeuvres};
volatile Mode currentMode = detection;

// Objet gerant la vitesse des roues
volatile adjustmentPWM pwm;

// Ecran LCD
LCM ecranLCD(&DEMO_DDR, &DEMO_PORT);

// Afficheur 7 segment pour la diffusion des vitesses
AfficheurSegment seg_7 = AfficheurSegment();
volatile uint8_t positionAfficheur = 0;
tmr2 timer2;
volatile uint8_t compteur2 =0;

ISR(TIMER2_COMPB_vect){
   cli();
   positionAfficheur++;
   if (positionAfficheur >= 5)
   {
      positionAfficheur = 0;
   }
   seg_7.afficherValeur(positionAfficheur);
   sei();
   TIFR2 |= (1 << OCF2B);       // Puttin th OCF2B bit at 0. so it clear the interruption flag
}

// Fonction qui initialise la direction des ports (entrée/sortie) et ajustes les registres nécessaires
void initialization() 
{
    cli();		                            // cli est une routine qui bloque toutes les interruptions
    DDRB = 0xFF;                            // PORT B est en mode sortie
    DDRA = 0xF8;                            // PA0-PA2 en entrée le reste en sortie    
    DDRC = 0xFF;                            // PORT C en sortie

    /* 0b1111 0111 PD3(bouton poussoir)est en mode entree, PD4 et PD5(PWM), 
    PD7 et PD2(LED droite), PD0 et PD1(LED gauche) sont en sorties */
    DDRD = 0xF7;  

    EIMSK |= (1 << INT1) ;                   // Activer les interruptions externes sur INT1
    EICRA |= (1 << ISC11) | (1 << ISC10) ;   // regler INT1 pour declencher au front montant uniquement

    sei();				                    // sei permet de recevoir à nouveau des interruptions.
}

// Fonction qui retourne le message à afficher en fonction de la distance
const char* messageDistance(double distance)
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

void manoeuvre1()
{
    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 1 ", 0);

    // manoeuvre
    pwm.startMotor((255*90)/100, (255*90)/100);  // 90% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(90,90);
    // LES DEUX LED en verte
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    for(uint8_t i = 90; i>=52; i--)
    {
        pwm.startMotor((255*i)/100, (255*90)/100);
        seg_7.etablirPourcentages(i,90);
        _delay_ms(100);
    }
    _delay_ms(1000);
    for(uint8_t i = 52; i<=90; i++)
    {
        pwm.startMotor((255*i)/100, (255*90)/100);   
        seg_7.etablirPourcentages(i,90);     
        _delay_ms(100);
    }
    _delay_ms(2000);

    // FIN DE MANOEUVRE
}

void manoeuvre2()
{
    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 2 ", 0);

    // Début manoeuvre
    pwm.startMotor((255*90)/100, (255*90)/100);  // 90% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(90, 90);
    // LES DEUX LED en verte
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    for(uint8_t i = 90; i>=52; --i)
    {
        pwm.startMotor((255*90)/100, (255*i)/100);
        seg_7.etablirPourcentages(90, i);
        _delay_ms(100);
    }
    _delay_ms(1000);
    for(uint8_t i = 52; i<=90; i++)
    {
        pwm.startMotor((255*90)/100, (255*i)/100);
        seg_7.etablirPourcentages(90, i);
        _delay_ms(100);
    }
    _delay_ms(2000);
    PORTD &= OFF;
    // FIN DE MANOEUVRE
}

void manoeuvre3()
{

    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 3 ", 0);

    // Début manoeuvre
    pwm.startMotor(255/2, 255/2);  // 50% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(50, 50);
    // LED GAUCHE ROUGE
    PORTD |= RED_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(1000);

    // DEUX LED VERTES
    pwm.startMotor((255*66)/100, (255*66)/100);
    seg_7.etablirPourcentages(66, 66);
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(2000);

    // LED DROITE ROUGE
    pwm.startMotor(255/2, 255/2);
    seg_7.etablirPourcentages(50, 50);
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= RED_RIGHT;
    _delay_ms(1000);

    // DEUX LED VERTES
    pwm.startMotor((255*78)/100, (255*78)/100);
    seg_7.etablirPourcentages(78, 78);
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(2000);
    // FIN DE MANOEUVRE
}

void manoeuvre4()
{
    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 4 ", 0);

    // Début manoeuvre
    pwm.startMotor(255/2, 255/2);  // 50% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(50, 50);
    // LED DROITE ROUGE
    PORTD |= GREEN_LEFT;
    PORTD |= RED_RIGHT;
    
    _delay_ms(1000);

    // DEUX LED VERTES
    pwm.startMotor((255*66)/100, (255*66)/100);
    seg_7.etablirPourcentages(66, 66);
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(2000);

    // LED GAUCHE ROUGE
    pwm.startMotor(255/2, 255/2);
    seg_7.etablirPourcentages(50, 50);
    PORTD &= OFF;
    PORTD |= RED_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(1000);

    // DEUX LED VERTES
    pwm.startMotor((255*78)/100, (255*78)/100);
    seg_7.etablirPourcentages(78, 78);
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    _delay_ms(2000);
    // FIN DE MANOEUVRE
}

void manoeuvre5()
{
    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 5 ", 0);

    // Début manoeuvre
    pwm.startMotor(255/2, 255/2); // 50% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(50, 50);
    // LED DROITE ROUGE
    PORTD |= GREEN_LEFT;
    PORTD |= RED_RIGHT;
    _delay_ms(2000);

    pwm.stopPWMSignal();
    PORTD &= OFF;
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    for(uint8_t i = 0 ; i<=63; i+=3)
    {
        seg_7.etablirPourcentages(i, i);
        pwm.startMotor((255*i)/100, (255*i)/100);
        _delay_ms(125);
    }
    _delay_ms(2000);
    // FIN DE MANOEUVRE
}

void manoeuvre6()
{
    // Affichage numéro manoeuvre sur LCD
    ecranLCD.write("Manoeuvre 6 ", 0);

    // Début manoeuvre
    pwm.startMotor((255*90)/100, (255*90)/100);  // 90% du TOP qui est 255 (uint8_t)
    seg_7.etablirPourcentages(90, 90);
    // LES DEUX LED en verte
    PORTD |= GREEN_LEFT;
    PORTD |= GREEN_RIGHT;
    for(uint8_t i = 90; i>=43; i-=7)
    {
        pwm.startMotor((255*i)/100, (255*i)/100);
        seg_7.etablirPourcentages(i, i);
        _delay_ms(500);
    }
    _delay_ms(2000);
    // FIN DE MANOEUVRE
}

ISR(INT1_vect)
{
    // Lorsque on appuie sur le bouton poussoir on passe alors en mode manoeuvre
    currentMode = manoeuvres;
    EIFR |= (1 << INTF1);       // Puttin th INTF1 bit at 0. so it clear the interruption flag
}

int main()
{
    initialization();
    timer2.demarrerTimer();
    seg_7.off();
    while(1)
    {
        switch(currentMode)
        {
            case detection :
                {
                    PORTD &= OFF;
                    /************************************************************************************************************************
                    ******************************************** LECTURE DU SONAR GAUCHE ****************************************************
                    *************************************************************************************************************************/

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

                    /************************************************************************************************************************
                    ******************************************* LECTURE DU SONAR DE DEVANT **************************************************
                    *************************************************************************************************************************/

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

                    /************************************************************************************************************************
                    ******************************************** LECTURE DU SONAR DROIT *****************************************************
                    *************************************************************************************************************************/

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
                    break;
                }

            case manoeuvres :
                {
                    // On passe au mode manoeuvre alors on nettoie l'affichage
                    ecranLCD.clear();
                    // On choisis quelle anoeuvre faire en fonction de la combinaison de distance
                    if ((distance_gauche > 3.00) && (distance_devant > 3.00) && (distance_droite >= 1.00) && (distance_droite < 3.00))
                    {
                        manoeuvre1();
                    }
                    else if ((distance_gauche >= 1.00) && (distance_gauche < 3.00) && (distance_devant > 3.00) && (distance_droite > 3.00))
                    {
                        manoeuvre2();
                    }
                    else if ((distance_gauche > 3.00) && (distance_devant < 1.00) && (distance_droite < 1.00))
                    {
                        manoeuvre3();
                    }
                    else if ((distance_gauche < 1.00) && (distance_devant < 1.00) && (distance_droite > 3.00))
                    {
                        manoeuvre4();
                    }
                    else if ((distance_gauche < 1.00) && (distance_devant < 1.00) && (distance_droite < 1.00))
                    {
                        manoeuvre5();
                    }
                    else if ((distance_gauche >= 1.00) && (distance_gauche < 3.00) && (distance_devant > 3.00) 
                    && (distance_droite >= 1.00) && (distance_droite < 3.00))
                    {
                        manoeuvre6();
                    }
                    else 
                    {
                        // Affiche "combinaison non evaluer" sur le LCD pendant 2 secondes
                        ecranLCD.write("Combinaison ", 0);
                        ecranLCD.write("non evaluee", 16);
                        _delay_ms(2000);
                    }
                    seg_7.off();                // On éteint le 7 segments
                    ecranLCD.clear();           // On efface l'ecran LCD
                    pwm.stopPWMSignal();
                    PORTD &= OFF;                // On éteint les LED ET OSCILLOSCOPE
                    currentMode = detection;
                    break;
                }
        }

    }
    return 0;
}
