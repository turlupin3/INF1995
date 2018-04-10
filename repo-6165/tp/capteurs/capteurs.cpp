// distance = 2478,633156x^-1.125
// OCRA = roue droite
// OCRB = roue gauche
// PORTA pour CAN
// PORTB pour moteur
// PORTD pour son
// PORTC pour DEL
// 94 = 15 cm selon la fonction
// 27 = 60 cm selon la fonction

// verifier les commentaires (arbitraire)

#define F_CPU 8000000

#include <ini.h>
#include <memoire_24.h>
#include <UART.h>
#include <pwmoteur.h>
#include <note.h>
#include <manips.h>
#include <ambre.h>
#include <can.h>

enum  {
	      proche, ok, loin // proche < 15cm, 15<= ok >= 60cm, loin > 60cm
} volatile capteurD, capteurG;

uint8_t lecture8Bit(can& conv, uint8_t pos);

void lectureCapteurs();
void ajustementDroite();
void ajustementGauche();
void faireLeTourDroite();
void faireLeTourGauche();
void changerPanneau();
void initialisation();
bool antiRebond();
void partirMinuterie ( uint8_t duree );

bool longerDroite;
bool longerGauche;
bool droitChanger = true;

can convertisseurD = can();
can convertisseurG = can();
volatile uint8_t distanceD = 0;
volatile uint8_t distanceG = 0;
volatile uint8_t lectureDonneeD= 0;
volatile uint8_t lectureDonneeG= 0;

volatile uint8_t boutonPoussoir = 0;



int main(){	
	
	initialisationUART();
	initialisation();
	setUpPWMoteur();
	partirMinuterie(100);
	
	//_delay_ms(1000);
	if (capteurD == proche || capteurD == ok){ // permet de savoir quel cote on longe
		longerDroite = true;				   // au debut du parcours
		longerGauche = false;
	}
	else{
		longerDroite = false;
		longerGauche = true;
	}
	controleMoteur(75);
	_delay_ms(1000);
	OCR0A =0;
	_delay_ms(1000);
	//~ while(true){
			//~ OCR0A = 0;
			//~ _delay_ms(1000);
		 ajustementDroite();
		 //ajustementGauche();
		//~ }
		//~ //lectureCapteurs();
		
		//~ if (longerDroite == true){ 		// switch case lorsqu'on longe la droite
			//~ switch(capteurD){
				//~ case proche:
					//~ if(capteurG == loin)
						//~ droitChanger = true;
					//~ if (capteurG == ok && droitChanger == true)
						//~ changerPanneau();
					//~ if(lectureDonneeD > 94) // si distance < 15cm
						//~ ajustementDroite(); 
					//~ break;
					
				//~ case ok:
					//~ if(capteurG == loin)
						//~ droitChanger = true;
					//~ if (capteurG == ok && droitChanger == true)
						//~ changerPanneau();
					//~ if (lectureDonneeD < 94) // si distance > 15cm
						//~ ajustementDroite();
					//~ break;
						
				//~ case loin:
					//~ if (capteurG == loin){
						//~ droitChanger = true;
						//~ faireLeTourDroite();
					//~ }
					//~ break;
			//~ }
		//~ }
		
		
		//~ if (longerGauche == true){		// switch case lorsqu'on longe la gauche
			//~ switch(capteurG){
				//~ case proche:
					//~ if(capteurD == loin)
						//~ droitChanger = true;
					//~ if (capteurD == ok && droitChanger == true)
						//~ changerPanneau();
					//~ if(lectureDonneeG > 94) // si distance < 15cm
						//~ ajustementGauche();
					//~ break;
					
				//~ case ok:
					//~ if (capteurD == loin)
						//~ droitChanger = true;
					//~ if (capteurD == ok && droitChanger == true)
						//~ changerPanneau();
					//~ if (lectureDonneeG < 94) // si distance > 15cm
						//~ ajustementGauche();
					//~ break;
						
				//~ case loin:
					//~ if (capteurD == loin){
						//~ droitChanger = true;
						//~ faireLeTourGauche();
					//~ }
					//~ break;
			//~ }
		//~ }
	 //~ }
	
	return 0; 
}

// fait le decalage de deux bit CAN car 2 des 10 bits sont inutilises
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
		
	lectureDonneeG = lecture8Bit(convertisseurG, 4);
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
	// plus vite.
	PORTC = ROUGE;
	if (lectureDonneeD > 94){	// si distance < 15cm
		if (OCR0A + 75 > 255){ // 50 est arbitraire
			OCR0A = 255;
		}
		else{
			OCR0A += 75;
		}
		
		while(lectureDonneeD != 94){	// si distance != 15cm
			//lectureCapteurs();
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
	// plus vite.
	if (lectureDonneeD < 94){ // si distance > 15cm
		if (OCR0B + 75 > 255){ // 50 est arbitraire
			OCR0B = 255;
		}
		else{
			OCR0B += 75;
		}	
			
		while(lectureDonneeD != 94){ // si distance != 15cm
			//lectureCapteurs();
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
	// plus vite.
	PORTC = ROUGE;
	if (lectureDonneeG > 94){ // si distance < 15cm
		if (OCR0B + 50 > 255) // 25 est arbitraire
			OCR0B = 255;
		else
			OCR0B += 50;
			
		while(lectureDonneeG != 94){	// si distance != 15cm
			//lectureCapteurs();
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
	// plus vite.
	if (lectureDonneeG < 94){ // si distance > 18cm
		if (OCR0A + 50 > 255) // 25 est arbitraire
			OCR0A = 255;
		else
			OCR0A += 50;
			
		while(lectureDonneeG != 94){	// si distance != 15cm
			//lectureCapteurs();
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
	// un roue tourne plus vite et l'autre moins vite pour contourner le panneau
	// jusqu'a temps que le capteur capte 15cm
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
		//lectureCapteurs();
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
		//lectureCapteurs();
	}	 
	OCR0B = 150;
	OCR0A = OCR0B;
	PORTC = VERT;
}

void changerPanneau(){
	PORTC = ROUGE;
	// La roue droite va tourner plus vite pendant quelque temps pour
	// que le bot s'oriente vers le panneau gauche. Ensuite les roues 
	// sont a la meme vitesse pour qu'il se dirige droit vers le panneau gauche
	if (longerDroite == true){
		if (OCR0A + 50 > 255){ // 50 est arbitraire
			OCR0A = 255;
		}
		else{
			OCR0A += 50;
		}
		_delay_ms(300); // 300 est arbitraire
		OCR0A = OCR0B;
		while(lectureDonneeG <= 84){
		}								// si distance >= 17cm (arbitraire)
			//lectureCapteurs();
		longerDroite = false;
		longerGauche = true;
	}
	
	// meme chose mais avec la roue gauche
	else{
		if (OCR0B + 50 > 255){ // 50 est arbitraire
			OCR0B = 255;
		}
		else{
			OCR0B += 50;
		}
		_delay_ms(300); // 300 est arbitraire
		OCR0B = OCR0A;
		while(lectureDonneeD <= 84){
			}							// si distance >= 17cm (arbitraire)
			//lectureCapteurs();
		longerGauche = false;
		longerDroite = true;
	}
	droitChanger = false;
	PORTC = VERT;
}


// les trois fonctions suivantes pour l'interrupt
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
			//lectureCapteurs();
		}
		arreterMoteur();
		controleMoteur(60);
		longerDroite = false;
		longerGauche = true;
	}
	else{
		arreterMoteur();
		while (lectureDonneeD != 94){ // si distance != 15cm
			tournerGauche();
			//lectureCapteurs();
		}
		arreterMoteur();
		controleMoteur(60);
		longerGauche = false;
		longerDroite = true;
	}
	
	EIFR |= (1 << INTF0);
	PORTC = VERT;
}

void partirMinuterie ( uint8_t duree ) {
cli();
// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT2 = 0;

OCR2A = duree;

TCCR2A |= (0 << COM2A1);
TCCR2A |= (0 << COM2B1);
TCCR2A |= (0 << COM2A0);
TCCR2A |= (0 << COM2B0);

TCCR2B |= (1 << CS22);
TCCR2B |= (1 << CS21);
TCCR2B |= (1 << CS20);


//TCCR1C = 0;

TIMSK2 |= (1 << OCIE2A);
sei();

}

ISR ( TIMER2_COMPA_vect  ) {

	DDRC= 0xff;
	PORTC= 2;
	DDRA = 0x00;
	can  convertisseurD = can();
	can  convertisseurG = can();
	lectureCapteurs();
	//~ lectureDonneeD = lecture8Bit(convertisseurD, 5);
	//~ lectureDonneeG = lecture8Bit(convertisseurG, 4);
	distanceD = 2478.633156*(pow(lectureDonneeD,-1.125));
	distanceG = 2478.633156*(pow(lectureDonneeG,-1.125));
	transmissionUART(0xf6);
	transmissionUART(distanceD);
	transmissionUART(0xf7);
	transmissionUART(distanceG);

}
//~ ISR ( TIMER2_COMPA_vect  ) {
	//~ jouerNote(45);
	//~ _delay_ms(200);
	//~ arreterJouer();
	//~ DDRA = 0x00;
	//~ can  convertisseurD = can();
	//~ can  convertisseurG = can();
	//~ uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 5);
	//~ uint8_t lectureDonneeG = lecture8Bit(convertisseurG, 4);
	//~ double interD = 2478.633156*(pow(lectureDonneeD,-1.125));
	//~ double interG = 2478.633156*(pow(lectureDonneeG,-1.125));
	//~ transmissionUART(0xf6);
	//~ transmissionUART(interD);
	//~ transmissionUART(0xf7);
	//~ transmissionUART(interG);

//~ }
