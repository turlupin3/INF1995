#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t boutonPoussoir = 0;

ISR(INT0_vect);
bool antiRebond();
void initialisation();
uint8_t getBouton();
