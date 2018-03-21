#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

void debut(bool & boolDebut);
void fin(bool & boolFin);
uint16_t dbc(unsigned char op, uint16_t instructionCounter,uint16_t & counter);
uint16_t fbc(unsigned char op, uint16_t instructionCounter, uint16_t currentInstruction,uint16_t & counter);
void att(unsigned char op);
void det(unsigned char op);
void dal(unsigned char op);
