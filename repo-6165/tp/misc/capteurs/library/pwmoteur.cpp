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

void controleMoteur(int8_t op) {
	//avancer
	
	
	if(op > 0) {
		PORTB =0;
	}
	else if(op < 0) {
		PORTB = 0b00100100;
		op *= (-1); // negate op
	}
	
	uint8_t tmp = op *2.55;
	OCR0B = tmp;
	OCR0A = tmp;
	
}

void controleMoteurG(int8_t op) {
	//avancer
	if(op > 0) {
		PORTB &= ~(1 << 5);
	}
	else if(op < 0) {
		//PORTB = 0b00100100;
		PORTB |= (1 << 5);
		op *= (-1); // negate op
	}
	
	uint8_t tmp = op *2.55;
	OCR0B = tmp;
	
}
void controleMoteurD(int8_t op) {
	//avancer
	
	
	if(op > 0) {
		PORTB &= ~(1 << 2);
	}
	else if(op < 0) {
		PORTB |= (1 << 2);
		op *= (-1); // negate op
	}
	
	uint8_t tmp = op *2.55;
	OCR0A = tmp;
	
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
	_delay_ms(1250); // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE
	arreterMoteur();
}
void tournerGauche() {
	//tourner a gauche
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = 150; 
	OCR0A = 150;
	PORTB = 0b100;
	_delay_ms(1250); // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE
	arreterMoteur();
	
}
