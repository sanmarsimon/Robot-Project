
#include <avr/io.h>
#include <util/delay.h>
#include "memoire_24.h"
#include "../Lib/uart.h"

#define F_CPU 8000000UL
#define BAUD 2400


uart a;

//#define DEBUG // à mettre en commentaire au besoin

#ifdef DEBUG 
 
# define DEBUG_PRINT(x) a.transmissionStringUART(x)  // ou par RS-232
# define DEBUG_PRINTCHAR(x) a.transmissionUART(x)  // ou par RS-232


#else

# define DEBUG_PRINT(x) do {} while (0) // code mort

#endif