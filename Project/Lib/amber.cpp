#include "amber.h"

 void amber::red()
{
  PORTC = 0xAA;
  PORTB = 0xAA;
  PORTA = 0xAA;
}

void amber::green()
{

  PORTC = 0x55;
  PORTB = 0x55;
  PORTA = 0x55;
}

void amber::off()
{

  PORTC = 0x00;
  PORTB = 0x00;
  PORTA = 0x00;
}

void amber::Amber(unsigned long s_count)
{
  for( unsigned long i =0; i < s_count; i++)
  {
  green();
  _delay_ms(9);
  red();
  _delay_ms(1);
  }
}

void amber::blink(){
    Amber(1);
    
    off();
}