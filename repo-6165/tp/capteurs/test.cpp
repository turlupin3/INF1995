#define F_CPU 8000000


#include <./library/ambre.h>
#include <./library/can.h>
#include <./library/UART.h>



uint8_t lecture8Bit(can& conv, uint8_t pos);

int main()
{	
	DDRA = 0x00;
	//DDRD = 0xff;
	
	initialisationUART();
	
	can convertisseurD = can();
	uint8_t lectureDonneeD = 0;
	
	for(;;){
		
		lectureDonneeD = lecture8Bit(convertisseurD, 5);

		transmissionUART (lectureDonneeD);
		
		//~ if(lectureDonneeD <= 25){ 
			//~ PORTD = ROUGE;
		//~ }
		//~ else if(lectureDonneeD >= 83){ 
			//~ ambre();
		//~ }
		//~ else {
			//~ PORTD = VERT;
			
		//~ }
	}

	return 0; 
}


uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}


	//~ //lectureDonneeG = lecture8Bit(convertisseurG, 1);

	
	//~ for(;;){
		//~ lectureDonneeD = lecture8Bit(convertisseurD, 5);

		
		
		//~ if(lectureDonneeD <= 25){ //ambre
			//~ ambre();
		//~ }
		//~ else if(lectureDonneeD >= 100){ //rouge
			//~ PORTD = ROUGE;
		//~ }
		//~ else {
			//~ PORTD = VERT;
			
		//~ }
	//~ }
