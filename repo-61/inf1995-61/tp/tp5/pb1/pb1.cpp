////////////////////////////////////////////////////
//  state  //  input  //  output  //  next state  //
////////////////////////////////////////////////////
//   INIT  //    0   //   ROUGE  //       0       //
////////////////////////////////////////////////////
//   INIT  //    1   //   ROUGE  //       1       //
////////////////////////////////////////////////////
//    1    //    0   //   AMBRE  //       2       //
////////////////////////////////////////////////////
//    1    //    1   //   AMBRE  //       1       //
////////////////////////////////////////////////////
//    2    //    0   //   VERT   //       2       //
////////////////////////////////////////////////////
//    2    //    1   //   VERT   //       3       //
////////////////////////////////////////////////////
//    3    //    0   //   ROUGE  //       4       //
////////////////////////////////////////////////////
//    3    //    1   //   ROUGE  //       3       //
////////////////////////////////////////////////////
//    4    //    0   //    OFF   //       4       //
////////////////////////////////////////////////////
//    4    //    1   //    OFF   //       5       //
////////////////////////////////////////////////////
//    5    //    0   //   VERT   //     INIT      //
////////////////////////////////////////////////////
//    5    //    1   //   VERT   //       5       //
////////////////////////////////////////////////////

#define F_CPU 8000000


#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

void ambre();

bool antiRebond();

volatile uint8_t state = 0;

void initialisation(){
	cli();
	DDRA = 0xff;
	DDRD = 0x00;
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (0 << ISC01) | (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request
	sei();
}

ISR(INT0_vect){
	bool buttonPress = false;
	_delay_ms(30.0);
	if(antiRebond()){
		buttonPress = true;
		}
	
	if(state < 6){
		state++;
	}
	else{
		state = 0;
	}
	EIFR |= (1 << INTF0);
}



int main()
{

	initialisation();

	
	for(;;){
	
	switch(state){
			case 0 : 	PORTA = ROUGE;
						break;
						
			case 1 :  	ambre();
						break;
						
			case 2 : 	PORTA = VERT;
						break;		
										
			case 3 : 	PORTA = ROUGE;
						break;
						
			case 4: 	PORTA = OFF;
						break;
						
			case 5 : 	PORTA = VERT;
						break;
	}	
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
