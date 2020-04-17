#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay_basic.h>



class adjustmentPWM
{
private:
	/* data */
public:
	adjustmentPWM(/* args */){}
	// ~adjustmentPWM();
	void stopPWMSignal() volatile;
	void startMotor (uint8_t roueGauche, uint8_t roueDroite) volatile;
};




