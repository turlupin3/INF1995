#include <interruptBouton.h> 

bool antiRebond(){
	DDRD = 0x00;
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

	while(antiRebond()){
		boutonPoussoir = 1;
		}
	
	boutonPoussoir = 0;
	EIFR |= (1 << INTF0);
}

uint8_t getBouton(){
	
	return boutonPoussoir;
}
