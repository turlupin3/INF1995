////////////////////////////////////////////////////
//  state  //  input  //  output  //  next state  //
////////////////////////////////////////////////////
//    0    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    0    //    1   //     0    //       1       //
////////////////////////////////////////////////////
//    1    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    1    //    1   //     0    //       2       //
////////////////////////////////////////////////////
//    2    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    2    //    1   //     0    //       3       //
////////////////////////////////////////////////////
//    3    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    3    //    1   //     0    //       4       //
////////////////////////////////////////////////////
//    4    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    4    //    1   //     0    //       5       //
////////////////////////////////////////////////////
//    5    //    0   //     0    //       0       //
////////////////////////////////////////////////////
//    5    //    1   //     0    //       6       //
////////////////////////////////////////////////////
//    6    //   n/a  //     1    //       1       //

#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

void ambre();

bool antiRebond();

bool antiHold();

int main(){
	
	DDRA = 0xff; // PORT A est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree	
	int compteur = 0;
	for(;;){
		
		if(antiHold()){
		compteur++;
		}
		if(compteur == 5){
		PORTA =ROUGE;
		_delay_ms(1000.0);
		PORTA = OFF;
		compteur = 0;
		}
		
	}
	return 0; 
}

void ambre(){
	PORTA = VERT;
	_delay_ms(1.0);
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

bool antiHold(){
	if(antiRebond()){
		while(antiRebond()){
			//wait for falling edge
		}
	return true;
	}
	return false;
}
