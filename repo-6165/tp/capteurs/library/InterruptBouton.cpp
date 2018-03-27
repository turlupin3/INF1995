#include <InterruptBouton.h> 

volatile uint8_t boutonPoussoir = 0;

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
