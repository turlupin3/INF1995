#include <pwm.h>


void setupPWMNote(){
	TCNT1 = 0x0;
	
	TCCR1A |= (1 << WGM10);	//mode PWM top ocr1a
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B |= (1 << WGM13);

	TCCR1B |= (1 << CS10); //set prescaler to 1
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	//~ TCCR1A |= (1 << COM1A1); //mode de compare
	//~ TCCR1A &= ~(1 << COM1A0);
	TCCR1A |= (1 << COM1B1);
	TCCR1A &= ~(1 << COM1B0);
	
	OCR1B = 0;
	OCR1A = 0;
	
	//~ TIMSK1 |= (1<< OCIE1A);
	TIMSK1 |= (1<< OCIE1B);

}
