#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#ifndef SEG7
#define SEG7




class AfficheurSegment
{
    int AfficheurPositions[5] = {
        0b11110000, //Dizaine Gauche
        0b11101000, //Unite Gauche
        0b11011000, //Millieu
        0b10111000, //Dizaine Droite
        0b01111000, //Unite Droite
    };


private:
    uint8_t valeursAffichees_[5] = {0,0,10,0,0};
    const uint8_t affichages[12]= {
        0b11111100, //0
        0b01100000, //1
        0b11011010, //2
        0b11110010, //3
        0b01100110, //4
        0b10110110, //5
        0b10111110, //6
        0b11100000, //7
        0b11111110, //8
        0b11110110, //9
        0b00000010, //-
        0b00000000,
        };
public:
    AfficheurSegment();
    ~AfficheurSegment();
    void afficherValeur(uint8_t position);
    void changerValeur(uint8_t position, uint8_t valeur);
    void etablirPourcentages(uint8_t pourcentageGauche, uint8_t pourcentageDroite);
    void off();
};

#endif