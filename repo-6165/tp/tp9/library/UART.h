#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> // Interrupts
#include "memoire_24.h"


void initialisationUART();
void transmissionUART ( uint8_t donnee );
