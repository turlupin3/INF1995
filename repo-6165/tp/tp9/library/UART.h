#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

void initialisationUART();
void transmissionUART ( uint8_t donnee );
unsigned char receptionUART ();
