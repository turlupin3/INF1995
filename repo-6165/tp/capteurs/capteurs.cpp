// distance = 2478,633156x^-1.125
// OCRA = roue droite
// OCRB = roue gauche
// PORTA pour CAN
// PORTB pour moteur
// PORTD pour son
// PORTC pour DEL

// verifier les commentaires (arbitraire)

#define F_CPU 8000000

//~ #include <./library/ambre.h>
#include <./library/can.h>
//~ #include <./library/UART.h>
#include <./library/pwmoteur.h>

enum  {
	      proche, ok, loin // proche < 15cm, 15<= ok >= 60cm, loin > 60cm
} capteurD, capteurG;

uint8_t lecture8Bit(can& conv, uint8_t pos);

void lectureCapteurs();
void ajustementDroite();
void ajustementGauche();
void faireLeTourDroite();
void faireLeTourGauche();
void changerPanneau();
void initialisation();
bool antiRebond();

bool longerDroite;
bool longerGauche;
bool droitChanger = true;

can convertisseurD = can();
can convertisseurG = can();
uint8_t lectureDonneeD = 0;
uint8_t lectureDonneeG = 0;

volatile uint8_t boutonPoussoir = 0;

const uint8_t VERT = 0b00000001;
const uint8_t ROUGE = 0b00000010; 
const uint8_t OFF = 0b00000000;


int main(){	
	initialisation();
	
	lectureCapteurs();
	if (capteurD == proche || capteurD == ok){ // permet de savoir quel cote on longe
		longerDroite = true;				   // au debut du parcours
		longerGauche = false;
	}
	else{
		longerDroite = false;
		longerGauche = true;
	}
	avancerMoteur(150);
	
	while(true){
		
		lectureCapteurs();
		
		if (longerDroite == true){ 		// switch case lorsqu'on longe la droite
			switch(capteurD){
				case proche:
					if(capteurG == loin)
						droitChanger = true;
					if (capteurG == ok && droitChanger == true)
						changerPanneau();
					if(lectureDonneeD > 94) // si distance < 15cm
						ajustementDroite(); 
					break;
					
				case ok:
					if(capteurG == loin)
						droitChanger = true;
					if (capteurG == ok && droitChanger == true)
						changerPanneau();
					if (lectureDonneeD < 94) // si distance > 15cm
						ajustementDroite();
					break;
						
				case loin:
					if (capteurG == loin){
						droitChanger = true;
						faireLeTourDroite();
					}
					break;
			}
		}
		
		
		if (longerGauche == true){		// switch case lorsqu'on longe la gauche
			switch(capteurG){
				case proche:
					if(capteurD == loin)
						droitChanger = true;
					if (capteurD == ok && droitChanger == true)
						changerPanneau();
					if(lectureDonneeG > 94) // si distance < 15cm
						ajustementGauche();
					break;
					
				case ok:
					if(capteurD == loin)
						droitChanger = true;
					if (capteurD == ok && droitChanger == true)
						changerPanneau();
					if (lectureDonneeG < 94) // si distance > 15cm
						ajustementGauche();
					break;
						
				case loin:
					if (capteurD == loin){
						droitChanger = true;
						faireLeTourGauche();
					}
					break;
			}
		}
	}
	
	return 0; 
}

// fait le decalage de deux bit CAN 2 des 10 bits sont inutilise
uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}

// lit la distance lu par les capteurs et change l'etat de capteurG/D en consequence
void lectureCapteurs(){
	lectureDonneeD = lecture8Bit(convertisseurD, 5);
	if (lectureDonneeD > 94){ // si distance < 15cm
		capteurD = proche;
	}
	else if (lectureDonneeD < 27){ // si distance > 60cm
		capteurD = loin;
	}
	
	else{
		capteurD = ok;			// 15<= distance >= 60cm
	}
		
	lectureDonneeG = lecture8Bit(convertisseurG, 1);
	if (lectureDonneeG > 94){ // si distance < 15cm
		capteurG = proche;
	}
	else if (lectureDonneeG < 27){ // si distance > 60cm
		capteurG = loin;
	}
	else{
		capteurG = ok;			// 15<= distance >= 60cm
	}
}


void ajustementDroite(){
	// si le robot se dirige vers le panneau on fait tourner la roue droite
	// plus vite. Ensuite, on fait tourner la gauche pour le remettre parallele
	PORTC = ROUGE;
	if (lectureDonneeD > 94){	// si distance < 15cm
		if (OCR0A + 50 > 255) // 50 est arbitraire
			OCR0A = 255;
		else
			OCR0A += 50;
			
		while(lectureDonneeD != 94){	// si distance != 15cm
			lectureCapteurs();
		}
		OCR0A = OCR0B;
		//~ if (OCR0B + 50 > 255){
			//~ OCR0B = 255;
			//~ _delay_ms(200); // 200 est arbitraire
			//~ OCR0B = OCR0A;
		//~ }
		//~ else{
			//~ OCR0B += 50;
			//~ _delay_ms(200);
			//~ OCR0B = OCR0A;
		//~ }
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue gauche
	// plus vite. Ensuite, on fait tourner la droite pour le remettre parallele
	if (lectureDonneeD < 94){ // si distance > 15cm
		if (OCR0B + 50 > 255) // 50 est arbitraire
			OCR0B = 255;
		else
			OCR0B += 50;
			
		while(lectureDonneeD != 94){ // si distance != 15cm
			lectureCapteurs();
		}
		OCR0B = OCR0A;
		//~ if (OCR0A + 50 > 255){
			//~ OCR0A = 255;
			//~ _delay_ms(200); // 200 est arbitraire
			//~ OCR0A = OCR0B;
		//~ }
		//~ else{
			//~ OCR0A += 50;
			//~ _delay_ms(200);
			//~ OCR0A = OCR0B;
		//~ }
	}
	PORTC = VERT;
}

void ajustementGauche(){
	
	// si le robot se dirige vers le panneau on fait tourner la roue gauche
	// plus vite. Ensuite, on fait tourner la droite pour le remettre parallele
	PORTC = ROUGE;
	if (lectureDonneeG > 94){ // si distance < 15cm
		if (OCR0B + 50 > 255) // 50 est arbitraire
			OCR0B = 255;
		else
			OCR0B += 50;
			
		while(lectureDonneeG != 94){	// si distance != 15cm
			lectureCapteurs();
		}
		OCR0B = OCR0A;
		//~ if (OCR0A + 50 > 255){
			//~ OCR0A = 255;
			//~ _delay_ms(200); // 200 est arbitraire
			//~ OCR0A = OCR0B;
		//~ }
		//~ else{
			//~ OCR0A += 50;
			//~ _delay_ms(200);
			//~ OCR0A = OCR0B;
		//~ }
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue droite
	// plus vite. Ensuite, on fait tourner la gauche pour le remettre parallele
	if (lectureDonneeG < 94){ // si distance > 18cm
		if (OCR0A + 50 > 255) // 50 est arbitraire
			OCR0A = 255;
		else
			OCR0A += 50;
			
		while(lectureDonneeG != 94){	// si distance != 15cm
			lectureCapteurs();
		}
		OCR0A = OCR0B;
		//~ if (OCR0B + 50 > 255){
			//~ OCR0B = 255;
			//~ _delay_ms(200); // 200 est arbitraire
			//~ OCR0B = OCR0A;
		//~ }
		//~ else{
			//~ OCR0B += 50;
			//~ _delay_ms(200);
			//~ OCR0B = OCR0A;
		//~ }
	}
	PORTC = VERT;
}

void faireLeTourDroite(){
	PORTC = ROUGE;
	if (OCR0B + 50 > 255) // 50 est arbitraire
		OCR0B = 255;
	else
		OCR0B += 50;
	if (OCR0A - 50 < 0) // 50 est arbitraire
		OCR0A = 0;
	else
		OCR0A -= 50;
	while (lectureDonneeD <= 90){ // si distance >= 15cm (arbitraire)
		lectureCapteurs();
	}	 
	OCR0A = 150;
	OCR0B = OCR0A;
	PORTC = VERT;
}

void faireLeTourGauche(){
	PORTC = ROUGE;
	if (OCR0A + 50 > 255) // 50 est arbitraire
		OCR0A = 255;
	else
		OCR0A += 50;
	if (OCR0B - 50 < 0) // 50 est arbitraire
		OCR0B = 0;
	else
		OCR0B -= 50;
	while (lectureDonneeG <= 90){ // si distance >= 15cm (arbitraire)
		lectureCapteurs();
	}	 
	OCR0B = 150;
	OCR0A = OCR0B;
	PORTC = VERT;
}

void changerPanneau(){
	PORTC = ROUGE;
	if (longerDroite == true){
		if (OCR0A + 50 > 255) // 50 est arbitraire
		OCR0A = 255;
		else
		OCR0A += 50;
		_delay_ms(300); // 300 est arbitraire
		OCR0A = OCR0B;
		while(lectureDonneeG <= 84)	// si distance >= 17cm (arbitraire)
			lectureCapteurs();
		longerDroite == false;
		longerGauche == true;
	}
	
	else{
		if (OCR0B + 50 > 255) // 50 est arbitraire
		OCR0B = 255;
		else
		OCR0B += 50;
		_delay_ms(300); // 300 est arbitraire
		OCR0B = OCR0A;
		while(lectureDonneeD <= 84) // si distance >= 17cm (arbitraire)
			lectureCapteurs();
		longerGauche == false;
		longerDroite == true;
	}
	droitChanger == false;
	PORTC = VERT;
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


void initialisation(){
	cli();
	DDRA = 0x00; // entree
	DDRD = 0xff; // sortie
	DDRC = 0xff; // sortie
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01) | (0 << ISC00); // sensible falling edge

	sei();
}

ISR(INT0_vect){

	if(antiRebond()){
		boutonPoussoir = 1;
	}
	
	PORTC = ROUGE;
	
	if(longerDroite == true){
		arreterMoteur();
		while (lectureDonneeG != 94){ // si distance != 15cm
			tournerDroite();
			lectureCapteurs();
		}
		arreterMoteur();
		avancerMoteur(150);
		longerDroite = false;
		longerGauche = true;
	}
	else{
		arreterMoteur();
		while (lectureDonneeD != 94){ // si distance != 15cm
			tournerGauche();
			lectureCapteurs();
		}
		arreterMoteur();
		avancerMoteur(150);
		longerGauche = false;
		longerDroite = true;
	}
	
	EIFR |= (1 << INTF0);
	PORTC = VERT;
}

