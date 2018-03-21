
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <util/delay.h>


void setUpPWMoteur();
void arreterMoteur();
void avancerMoteur(unsigned char op);
void reculer(unsigned char op);
void tournerDroite();
void tournerGauche();
