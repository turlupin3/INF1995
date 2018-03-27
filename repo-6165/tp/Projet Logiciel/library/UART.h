#define F_CPU 8000000

#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

void initialisationUART();
void transmissionUART ( uint8_t donnee );
unsigned char receptionUART ();
