
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <util/delay.h>


void setUpPWMoteur();
void arreterMoteur();
void avancerMoteur(unsigned char op);
void avancerMoteurG(unsigned char op);
void avancerMoteurD(unsigned char op);
void reculerMoteur(unsigned char op);
void tournerDroite();
void tournerGauche();
