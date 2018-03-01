
#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

void ambre();

bool antiRebond();

volatile uint8_t minuterieExpiree = 0;
volatile uint8_t compteur = 0;
volatile uint8_t boutonPoussoir = 0;

void initialisation(){
	cli();
	DDRA = 0xff;
	DDRD = 0x00;
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (0 << ISC01) | (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request

	sei();
}

ISR(INT0_vect){

	if(antiRebond()){
		boutonPoussoir = 1;
		}
	
	EIFR |= (1 << INTF0);
}

ISR(TIMER1_COMPA_vect){
	
		minuterieExpiree = 1;	
}

void partirMinuterie(uint16_t duree){
	TCNT1 = 0x0;
	OCR1A = duree;
	TCCR1B |= (1 << CS10);	
	TCCR1B &= ~(1 << CS11);	
	TCCR1B |= (1 << CS12);	
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);

}
int main()
{

	initialisation();
	
	
	_delay_ms(10000.0);
	PORTA = ROUGE;
	_delay_ms(100.0);
	PORTA = OFF;
	partirMinuterie(8000);
	
	do{
		
	}while(minuterieExpiree == 0 && boutonPoussoir ==0);
	cli();
	
	
	if(boutonPoussoir == 1){
		PORTA = VERT; 
	}
	
	
  return 0; 
}

void ambre(){
	PORTA = VERT;
	_delay_ms(5.0);
	PORTA = ROUGE;
	_delay_ms(1.0);
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
