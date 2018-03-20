#include <ini.h>

void demarrage(){
	
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	
	PORTB = ROUGE;
	_delay_ms(500);
	PORTB = OFF;
	_delay_ms(250);
	PORTB = ROUGE;
	_delay_ms(500);
	PORTB = OFF;
	_delay_ms(250);
	
	PORTB = VERT;
	_delay_ms(500);
	PORTB = OFF;
	_delay_ms(250);
		PORTB = VERT;
	_delay_ms(500);
	PORTB = OFF;
}

void test(){
	PORTB = ROUGE;
}
