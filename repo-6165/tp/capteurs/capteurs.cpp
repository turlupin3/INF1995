// 25 = 60cm
// 90 = 15cm
// OCRA = roue droite
// OCRB = roue gauche

#define F_CPU 8000000

#include <./library/ambre.h>
#include <./library/can.h>
#include <./library/UART.h>

enum  {
	      proche, ok, loin // proche < 15cm, 15<= ok >= 60cm, loin > 60cm
} capteurD, capteurG, capteurDLast, capteurGlast; 

uint8_t lecture8Bit(can& conv, uint8_t pos);
void lectureCapteurs();
void ajustementDroite();
void ajustementGauche();

int main()
{	
	DDRA = 0x00;
	DDRD = 0xff;
	
	can convertisseurD = can();
	uint8_t lectureDonneeD = 0;
	
	can convertisseurG = can();
	uint8_t lectureDonneeG = 0;
	
	capteurD = ok;
	capteurG = ok;
	capteurDLast = ok;
	capteurGlast = ok;
	
	avancerMoteur(200);
	
	while(true){
		
		lectureCapteurs();
		
		switch(capteurD){
			
			case proche:				// ajustement Droite
				ajustementDroite();
				break;
				
			case ok:
				if (capteurG == proche)	// ajustement Gauche (D ok)
					ajustementGauche();
				break;
					
			case loin:
				if (capteurG == proche)	// ajustement Gauche (D loin)
					ajustementGauche();
				if(
				break;
		}
	}
	
	return 0; 
}

uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}

void lectureCapteurs(){
	lectureDonneeD = lecture8Bit(convertisseurD, 5)
	capteurDLast = capteurD;
	if (lectureDonneeD > 90){ // 90 = 15cm
		capteurD = proche;
	}
	else if (lectureDonneeD < 25){ // 25 = 60cm
		capteurD = loin;
	}
	else{
		capteurD = ok;
	}
		
	lectureDonneeG = lecture8Bit(convertisseurG, 1)
	capteurGLast = capteurG;
	if (lectureDonneeG > 90){ // 90 = 15cm
		capteurG = proche;
	}
	else if (lectureDonneeG < 25){ // 25 = 60cm
		capteurG = loin;
	}
	else{
		capteurG = ok;
	}
}

void ajustementDroite(){
	if (OCR0A + 50 > 255)
		OCR0A = 255;
	else
		OCR0A += 50;
		
	while(capteurD == proche){
		lectureCapteurs();
		if (capteurD == ok)
			OCR0A -= 50;
	}
}

void ajustementGauche(){
	if (OCR0B + 50 > 255)
		OCR0B = 255;
	else
		OCR0B += 50;
		
	while(capteurG == proche){
		lectureCapteurs();
		if (capteurG == ok)
			OCR0B -= 50;
	}
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
