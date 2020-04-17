#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 8000000UL

#ifndef TIMER2
#define TIMER2

class tmr2
{
private:

public:
    tmr2(){}
    ~tmr2(){}
    void demarrerTimer();
};


#endif