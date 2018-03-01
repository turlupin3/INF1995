/////////////////////////////////////////////////////////////////////////////
//INF 1995 TD7 pb1
//auteurs : Francis Lavigueur 1906953
//			Rostan Lord-Belleville
////////////////////////////////////////////////////////////////////////////

#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

bool buttonPress();
void delay_ms(double ms);

void initialisation();
void initializeTimer();
void stopTimer();

void clignoterA0(uint16_t duree_ds, double frequence, uint8_t couleur); //fonction qui fait clignoter une couleur sur la pin 0 du port A

volatile uint8_t boutonEstEnfonce= 0;
volatile uint16_t tempsEcoule_ds; // temps ecoule en dixiemes de seconde


int main()
{
	initialisation();
	uint16_t compteur = 0;
	
	for(;;){
	
	while(boutonEstEnfonce == 0){
		//wait

	}
	initializeTimer();
	do{
		//wait some moar et incremente compteur 10 fois par secondes
		compteur = tempsEcoule_ds;	
	}while(boutonEstEnfonce == 1 && compteur <= 120);
	stopTimer();
		
	clignoterA0(5, 4, VERT); //clignotements demandes, (ici il nous paraissait overkill d'utiliser un timer car le processeur ne fait rien dautre)
	delay_ms(2000);
	clignoterA0(compteur/2, 2, ROUGE);
	PORTA = VERT;
	delay_ms(1000);
	PORTA = OFF;
	
	}
  return 0; 
}

bool buttonPress(){
	bool estEnfonce = false;
	if(PIND & 0x04){
		_delay_ms(10.0);
		if(PIND & 0x04){
		estEnfonce = true;
		}
	}
	return estEnfonce;
}
//fonction qui remplace _delay_ms car elle ne prend pas de variable en parametres
void delay_ms(double ms){ 
	for(; ms > 0 ; ms--){
		_delay_ms(1);
	}
}

void initialisation(){
	cli();
	DDRA = 0xff;
	DDRD = 0x00;
	
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (0 << ISC01) | (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request
	sei();
}

ISR(INT0_vect){
	boutonEstEnfonce = 0;
	delay_ms(30.0);
	if(buttonPress()){ //TODO verfier la necessite du delay
		boutonEstEnfonce = 1;
	}
	EIFR |= (1 << INTF0);
}
//initialisation du compteur pour qu'il genere un interupt a chaque dixieme de seconde
void initializeTimer(){ 
	cli();
	tempsEcoule_ds = 0;
	TCNT1 = 0x0;
	OCR1A = 781; // clock/prescaler/10 = 8000000/1024/10 = 781.25
	TCCR1B |= (1 << CS10);	
	TCCR1B &= ~(1 << CS11);	
	TCCR1B |= (1 << CS12);	
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	sei();
}
// arrete de compter
void stopTimer(){
	TCCR1B &= ~(1 << CS10);	
	TCCR1B &= ~(1 << CS11);	
	TCCR1B &= (~1 << CS12);
	TCNT1 = 0x0;
}
//routine dinteteruption du compteur
ISR(TIMER1_COMPA_vect){
		tempsEcoule_ds++;
}
//fonction qui fait clignoter une couleur a une certaine frequence sur la pin 0 du port A
void clignoterA0(uint16_t duree_ds, double frequence, uint8_t couleur){
	initializeTimer();
	while(tempsEcoule_ds <= duree_ds){
		PORTA = couleur;
		delay_ms(500/frequence);
		PORTA = OFF;
		delay_ms(500/frequence);
	}
	stopTimer();
}
