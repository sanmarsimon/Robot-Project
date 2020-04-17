#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

class amber
{
private:

public:
    amber(){}
  
    void red();
    void green();
    void Amber(unsigned long s_count);
    void off();
    void blink();
};
