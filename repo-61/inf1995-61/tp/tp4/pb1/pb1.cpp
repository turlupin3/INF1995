#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>
#include <util/delay_basic.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

void delay_ms(float ms);
void ambre();
void pwm();
void fade(int s);
bool antiRebond();


int main(){
	
	DDRA = 0xff; // PORT A est en mode sortie
	DDRD = 0x00;
	
	
	for(;;){
		PORTA = ROUGE;
		while(antiRebond()){
			pwm();
		}
	
	}	

}

void delay_ms(float ms){

	for(; ms > 0; ms --){
	
		_delay_ms(1);
	}
}


void pwm(){
	
	PORTA = ROUGE;
	delay_ms(0.9);
	PORTA = OFF;
	delay_ms(0.1);
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
