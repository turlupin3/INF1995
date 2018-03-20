#include <pwm.h>


void ajustementPWM(uint16_t valeur){
	TCNT1 = 0x0;
	
	TCCR1A &= ~(1 << WGM10);	//mode CTC
	TCCR1A &= ~(1 << WGM11);
	TCCR1B |=  (1 << WGM12);
	TCCR1B &= ~(1 << WGM13);

	TCCR1B |= (1 << CS10); //set prescaler to 1024
	TCCR1B &= ~(1 << CS11);
	TCCR1B |= (1 << CS12);
	
	TCCR1A &= ~(1 << COM1A1); //mode de compare
	TCCR1A |= (1 << COM1A0);
	TCCR1A &= ~(1 << COM1B1);
	TCCR1A |= (1 << COM1B0);
	
	OCR1B = valeur;
	OCR1A = valeur;
	
	TIMSK1 |= (1<< OCR1A);
	TIMSK1 |= (1<< OCR1B);

}
