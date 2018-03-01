
#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;


bool antiRebond();

volatile uint8_t minuterieExpiree = 0;
volatile uint8_t compteur = 0;
volatile uint8_t boutonPoussoir = 0;


void ajustementPWM(uint16_t valeur){
	TCNT1 = 0x0;
	
	TCCR1A |= (1 << WGM10);	//mode 8 bit PWM
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B &= ~(1 << WGM13);

	TCCR1B &= ~(1 << CS10); //set prescaler to 8
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	TCCR1A |= (1 << COM1A1); //mode de compare
	TCCR1A &= ~(1 << COM1A0);
	TCCR1A |= (1 << COM1B1);
	TCCR1A &= ~(1 << COM1B0);
	
	OCR1B = valeur;
	OCR1A = valeur;
	
	TIMSK1 |= (1<< OCR1A);
	TIMSK1 |= (1<< OCR1B);

}
int main()
{
	
	DDRD = 0xff;
	PORTD = 0;
	
	ajustementPWM(50);
	
	while(true)
	{}
  return 0; 
}


bool antiRebond(){
	bool estEnfonce = false;
	if(PIND & 0x04){
		_delay_ms(10.0);
		if(PIND & 0x04){
		estEnfonce = true;;
		}
	}
	return estEnfonce;
}
