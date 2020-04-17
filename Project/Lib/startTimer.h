#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

class startTimer
{
private:
    
public:
    startTimer(){}
    ~startTimer();
    void Timer(const uint16_t duration);
};


