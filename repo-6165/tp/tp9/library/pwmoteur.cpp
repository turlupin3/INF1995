#include <pwmoteur.h>

void setUpPWMoteur(){
	TCNT0 = 0x0;
	
	TCCR0A |= (1 << WGM00);	//mode PWM, Phase corerct
	TCCR0A |= (0 << WGM01);
	TCCR0B |= (1 << WGM02);

	TCCR0B |= (0 << CS10); //set prescaler to 8
	TCCR0B |= (1 << CS11);
	TCCR0B |= (0 << CS12);
	
	TCCR0A |= (1 << COM0A1); //mode de compare
	TCCR0A |= (1 << COM0B1);


	
	OCR0B = 0;
	OCR0A = 0;
	
	TIMSK0 |= (0<< OCR1A);
	TIMSK0 |= (0<< OCR1B);

}

void arreterMoteur(){
	TCCR0B |= (0 << CS10); //set prescaler to 0
	TCCR0B |= (0 << CS11);
	TCCR0B |= (0 << CS12);
}

void avancerMoteur(unsigned char op) {
	//avancer
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = op;
	OCR0A = op;
	PORTB |= 0b100100;
	
}
void reculer(unsigned char op) {
	//reculer
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = op;
	OCR0A = op;
	PORTB &= 0b011011;
	
}
void tournerDroite() {
	//tourner a droite
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = 64; 
	OCR0A = 64;
	PORTB |= 0b100;
	PORTB &= 0b011111;
	_delay_ms(1500); // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE
	arreterMoteur();
}
void tournerGauche() {
	//tourner a gauche
	DDRB = 0xff;
	setUpPWMoteur();
	OCR0B = 64; 
	OCR0A = 64;
	PORTB |= 0b100000;
	PORTB &= 0b111011;
	_delay_ms(1500); // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE
	arreterMoteur();
	
}
