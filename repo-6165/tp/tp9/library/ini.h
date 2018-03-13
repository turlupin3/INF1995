#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t OFF = 0b00000000;
const uint32_t PORT_SORTIE  = 0xff;

void demarrage();
