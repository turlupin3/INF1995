#define F_CPU 8000000

#include <avr/io.h> 

void initialisationUART();
void transmissionUART ( uint8_t donnee );
unsigned char receptionUART ();
