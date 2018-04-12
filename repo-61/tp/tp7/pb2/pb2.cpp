/////////////////////////////////////////////////////////////////////////////
//INF 1995 TD7 pb2
//auteurs : Francis Lavigueur 1906953
//			Rostan Lord-Belleville
////////////////////////////////////////////////////////////////////////////

#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <can.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

const uint8_t SEUILVERT = 120;
const uint8_t SEUILROUGE = 220;

void ambre();

uint8_t lecture8Bit(can& conv, uint8_t pos);

int main()
{	
	DDRA = 0x00;
	DDRB = 0xff;
	
	can convertisseur = can();
	uint8_t intensiteLumineuse = 0;
	
	for(;;){
		intensiteLumineuse = lecture8Bit(convertisseur, 5);
		
		if(intensiteLumineuse <= SEUILVERT){ //vert
			PORTB = VERT;
		}
		else if(intensiteLumineuse >= SEUILROUGE){ //rouge
			PORTB = ROUGE;
		}
		else {
			ambre();
		}
	}
	return 0; 
}
//fonction qui met la del a ambre sur le portB
void ambre(){
	PORTB = VERT;
	_delay_ms(10);
	PORTB = ROUGE;
	_delay_ms(10);
}

uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}
