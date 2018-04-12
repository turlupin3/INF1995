#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>

const uint8_t VERTDEL = 0b10000000;
const uint8_t ROUGEDEL = 0b00100000; 
const uint8_t OFFDEL = 0b00000000;


void ambre();
void delVert();
void delRouge();
void delOff();
void delSwitcher(unsigned char order);
