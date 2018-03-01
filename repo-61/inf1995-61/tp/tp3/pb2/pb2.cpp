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

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;

void ambre();

bool antiRebond();

bool antiHold();


int main()
{
	DDRA = 0xff;
	DDRD = 0x00;
	
	enum state {INIT, S1, S2, S3, S4, S5};
	
	for(;;){
	//on utilise la propriete "cascade" du switch case pour eviter de devoir faire: state++; break;
	switch(INIT){
			case INIT : PORTA = ROUGE;
						while(!antiRebond()){
							//wait around
						}
			case S1 :  	while(antiRebond()){
							ambre();
							//wait around
						}
			case S2 : 	PORTA = VERT;
						while(!antiRebond()){
							//wait around
						}
			case S3 : 	PORTA = ROUGE;
						while(antiRebond()){
							//wait around
						}
			case S4: 	PORTA = OFF;
						while(!antiRebond()){
							//wait around
						}
			case S5 : 	PORTA = VERT;
						while(antiRebond()){
							//wait around
						}
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
bool antiHold(){
	if(antiRebond()){
		while(antiRebond()){
			//wait for falling edge
		}
	return true;
	}
	return false;
}
