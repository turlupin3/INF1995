#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect);
void initialisation();
