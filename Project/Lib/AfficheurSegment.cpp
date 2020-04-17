#include "AfficheurSegment.h"
#define F_CPU 8000000UL


AfficheurSegment::AfficheurSegment()
{
}
AfficheurSegment::~AfficheurSegment()
{
}

void AfficheurSegment::changerValeur(uint8_t position, uint8_t valeur) {
    valeursAffichees_[position] =valeur;
}

void AfficheurSegment::afficherValeur(uint8_t position) {
    PORTC = 0;
    PORTA |= 0b11111000;
    uint8_t valeur = affichages[valeursAffichees_[position]];
    PORTA = AfficheurPositions[position];
    PORTC = valeur;
    
    
}

void AfficheurSegment::etablirPourcentages(uint8_t pourcentageGauche, uint8_t pourcentageDroite) {
    changerValeur(0,(pourcentageGauche/10));
    changerValeur(1,(pourcentageGauche%10));
    changerValeur(2,10);
    changerValeur(3,(pourcentageDroite/10));
    changerValeur(4,(pourcentageDroite%10));
}

void AfficheurSegment::off() {
    for (uint8_t i = 0; i < 5; i++)
    {
        changerValeur(i,11);
    }
    
}