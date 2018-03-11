#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;


void ambre();
