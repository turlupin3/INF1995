#include <pwmoteur.h>

void setUpPWMoteur(){
	DDRB = 0xff;
	PORTB = 0x00;
	TCNT0 = 0x0;
	
	TCCR0A |= (1 << WGM00);	//mode phase correct
	TCCR0A &= ~(1 << WGM01);
	TCCR0B &= ~(1 << WGM02);

	TCCR0B &= ~(1 << CS00); //set prescaler to 8
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);
	
	TCCR0A |= (1 << COM0A1); //mode de compare
	TCCR0A &= ~(1 << COM0A0);
	TCCR0A |= (1 << COM0B1);
	TCCR0A &= ~(1 << COM0B0);
	

	
	OCR0B = 0;
	OCR0A = 0;
	
	TIMSK0 |= (1 << OCIE0A);
	TIMSK0 |= (1 << OCIE0B);
	//TIMSK0 |= (1 << OCR0A);
	//TIMSK0 |= (1 << OCR0B);

}

void arreterMoteur(){
	DDRB = 0xff;
	TCCR0A &= ~(1 << COM0A1); //mode de compare
	TCCR0A &= ~(1 << COM0A0);
	TCCR0A &= ~(1 << COM0B1);
	TCCR0A &= ~(1 << COM0B0);
	
	TCCR0B &= ~(1 << CS00); //set prescaler to 0
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS02);
	
	OCR0B = 0;
	OCR0A = 0;
	PORTB = 0x00;
	
}

//~ void arreterMoteur2(){
	//~ //avancerMoteur(0);
	//~ //DDRB = 0xff;
	//~ //PORTB = 0;

	//~ cli ();
	//~ EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001
	//~ EICRA |= (1 << ISC00);
	//~ EICRA |= (1 << ISC01);
	//~ sei ();

	//~ //OCR1B = 0;
	//~ //OCR1A = 0;
	//~ OCR0A = 0;
	//~ OCR0B = 0;

	//~ TCCR0A |= (1 << WGM10);
	//~ TCCR0A |= (1 << COM1A1);
	//~ TCCR0A |= (1 << COM1B1);
	//~ TCCR0B |= (1 << CS11);
	//~ TCCR1C = 0;
	//~ //DDRB=0x00;
	//~ }

void avancerMoteur(unsigned char op) {
	//avancer
	setUpPWMoteur();
	OCR0B = op;
	OCR0A = op;
   // PORTB |= 0b101000;
	
}
void avancerMoteurG(unsigned char op) {
	//avancer
	setUpPWMoteur();
	OCR0B = op;
   // PORTB |= 0b101000;
	
}
void avancerMoteurD(unsigned char op) {
	//avancer
	setUpPWMoteur();
	OCR0A = op;
   // PORTB |= 0b101000;
	
}
void reculerMoteur(unsigned char op) {
	//reculer
	
	setUpPWMoteur();
	DDRB = 0xff;
	PORTB = 0b00100100;
	OCR0B = op;
	OCR0A = op;
	
	
}
void tournerDroite() {
	//tourner a droite
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = 150; 
	OCR0A = 150;
	PORTB = 0b100000;
	//~ _delay_ms(2500); // 1250 = 90degree
	//~ arreterMoteur();
}
void tournerGauche() {
	//tourner a gauche
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = 150; 
	OCR0A = 150;
	PORTB = 0b100;
	//~ _delay_ms(2500); // 1250 = 90degree
	//~ arreterMoteur();
	
}
