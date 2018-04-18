#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

void debut(bool & boolDebut);
void fin(bool & boolFin);
uint16_t dbc(unsigned char op);
uint16_t fbc(unsigned char & counter, uint16_t & debutBoucle, uint16_t & nextInstruction);
void att(unsigned char op);
void det(unsigned char op);
void dal(unsigned char op);
