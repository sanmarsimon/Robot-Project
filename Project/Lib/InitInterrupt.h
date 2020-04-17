#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

class InitInterrupt
{
private:
    /* data */
public:
    InitInterrupt(/* args */){}
    ~InitInterrupt();
    void initialization();
};



