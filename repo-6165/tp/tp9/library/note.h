#define F_CPU 8000000

#include <avr/interrupt.h>


void setupPWMNote();
void jouerNote(uint8_t note);
uint16_t calculOCR1A(double frequence);
