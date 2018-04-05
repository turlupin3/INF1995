
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <util/delay.h>


void setUpPWMoteur();
void arreterMoteur();
void controleMoteur(int8_t op);
void controleMoteurG(int8_t op);
void controleMoteurD(int8_t op);
void reculerMoteur(unsigned char op);
void tournerDroite();
void tournerGauche();
