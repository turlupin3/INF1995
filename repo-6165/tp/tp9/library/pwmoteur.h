
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <util/delay.h>


void setUpPWMoteur();
void arreterMoteur();
void arreterMoteur2();
void avancerMoteur(unsigned char op);
void avancerMoteur2(unsigned char op);
void reculerMoteur(unsigned char op);
void tournerDroite();
void tournerGauche();
