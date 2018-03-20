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
	
	//TIMSK0 |= (1<< OCR1A);
	//TIMSK0 |= (1<< OCR1B);

}

void arreterMoteur(){
	TCCR0B |= (0 << CS10); //set prescaler to 0
	TCCR0B |= (0 << CS11);
	TCCR0B |= (0 << CS12);
}
