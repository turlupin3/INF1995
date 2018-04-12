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

#include <stdlib.h>
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
	
	//~ jouerNote(55);	 ///
	//~ _delay_ms(200);	///
	//~ arreterJouer();	///
	
	delSwitcher(1); // 1 = vert/// 2 = rouge /// 0 = off

	if (capteurD == proche || capteurD == ok){ // permet de savoir quel cote on longe
		longerDroite = true;				   // au debut du parcours
		longerGauche = false;
	}
	else{
		longerDroite = false;
		longerGauche = true;
	}
	controleMoteurG(53);
	controleMoteurD(40);
	_delay_ms(250);
	while(true){
		ajustementDroite();
		if(capteurG == loin){
			if (capteurD == loin){
				faireLeTourDroite();
			}
		}
	}
	//~ while(true){
			//~ OCR0A = 0;
			//~ _delay_ms(1000);
		 //~ ajustementDroite();
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
	if (distanceD < 15){
		controleMoteurD(45);
		while(distanceD < 14 || distanceD > 16){
			if(capteurG == loin){
				if (capteurD == loin){
					faireLeTourDroite();
			}
		}
		}
		controleMoteurG(43);
		controleMoteurD(30);
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue gauche
	// plus vite.
	if (distanceD > 15 && distanceD < 20){ 
		controleMoteurG(55);	
		while(distanceD < 14 || distanceD > 16){
			if(capteurG == loin){
				if (capteurD == loin){
					faireLeTourDroite();
			}
		}
		}
		controleMoteurG(43);
		controleMoteurD(30);
	}

	else if (distanceD >= 20) {
		controleMoteurG(50);
		_delay_ms(250);
		controleMoteurG(43);
		controleMoteurD(30);
		while(distanceD > 16){
			if(capteurG == loin){
				if (capteurD == loin){
					faireLeTourDroite();
			}
		}
		}
	}
	PORTC = VERT;
}

void ajustementGauche(){
	// si le robot se dirige vers le panneau on fait tourner la roue gauche
	// plus vite.
	PORTC = ROUGE;
	if (distanceG < 15){
		controleMoteurG(55);
		while(distanceG < 14 || distanceG > 16){
		}
		controleMoteurG(43);
		controleMoteurD(30);
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue droite
	// plus vite.
	if (distanceG > 15 && distanceG < 20){ 
		controleMoteurD(45);	
		while(distanceG < 14 || distanceG > 16){
		}
		controleMoteurG(43);
		controleMoteurD(30);
	}

	else if (distanceG >= 20) {
		controleMoteurD(40);
		_delay_ms(250);
		controleMoteurG(43);
		controleMoteurD(30);
		while(distanceG > 16){
		}
	}
	PORTC = VERT;
}

void faireLeTourDroite(){
	// un roue tourne plus vite et l'autre moins vite pour contourner le panneau
	// jusqu'a temps que le capteur capte 15cm
	PORTC = ROUGE;
	controleMoteurG(70);
	//~ if (OCR0A - 50 < 0) // 50 est arbitraire
		//~ OCR0A = 0;
	//~ else
		//~ OCR0A -= 50;
	while (distanceD >= 15){ // si distance >= 15cm (arbitraire)
		//lectureCapteurs();
	}	 
	controleMoteurG(43);
	PORTC = VERT;
}

void faireLeTourGauche(){
	PORTC = ROUGE;
	controleMoteurD(60);
	//~ if (OCR0B - 50 < 0) // 50 est arbitraire
		//~ OCR0B = 0;
	//~ else
		//~ OCR0B -= 50;
	while (lectureDonneeG <= 90){ // si distance >= 15cm (arbitraire)
		//lectureCapteurs();
	}	 
	controleMoteurD(30);
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
//~ cli();
// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

//~ TCNT2 = 0;

//~ OCR2A = duree;
//~ OCR2B = duree;

//~ TCCR2A |= (0 << COM2A1);
//~ TCCR2A |= (0 << COM2B1);
//~ TCCR2A |= (0 << COM2A0);
//~ TCCR2A |= (0 << COM2B0);

//~ TCCR2B |= (1 << CS22);
//~ TCCR2B |= (1 << CS21);
//~ TCCR2B |= (1 << CS20);


//~ //TCCR1C = 0;

//~ TIMSK2 |= (1 << OCIE2A);
//~ sei();
   cli();

    TCNT2 = 0;
    OCR2A = 127;
    OCR2B = 225;

    TCCR2A |= (0 << COM2A1); //normal port operation OC2A, OC2B disconected
    TCCR2A |= (0 << COM2B1);
    TCCR2A |= (0 << COM2A0);
    TCCR2A |= (0 << COM2B0);

    TCCR2B |= (1 << CS22);  //prescaler
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);


    //TCCR1C = 0;

    TIMSK2 |= (1 << OCIE2A);
    TIMSK2 |= (1 << OCIE2B);

    sei();

}

ISR ( TIMER2_COMPA_vect  ) {


	lectureDonneeD = lecture8Bit(convertisseurD, 4);
	distanceD = 2478.633156*(pow(lectureDonneeD,-1.125));
	if (distanceD < 15){ 
		capteurD = proche;
	}
	else if (distanceD > 60){ 
		capteurD = loin;
	}
	
	else{
		capteurD = ok;			// 15<= distance >= 60cm
	}
	//~ distanceG = 2478.633156*(pow(lectureDonneeG,-1.125));
	transmissionUART(0xf6);
	transmissionUART(distanceD);
	//~ transmissionUART(0xf7);
	//~ transmissionUART(distanceG);

}
ISR ( TIMER2_COMPB_vect  ) {
	//~ jouerNote(45);
	//~ _delay_ms(200);
	//~ arreterJouer();
	//~ DDRA = 0x00;
	//~ can  convertisseurD = can();
	//~ can  convertisseurG = can();
	//~ lectureCapteurs();
	//~ uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 4);
	lectureDonneeG = lecture8Bit(convertisseurG, 5);
	//~ double interD = 2478.633156*(pow(lectureDonneeD,-1.125));
	distanceG = 2478.633156*(pow(lectureDonneeG,-1.125));
	if (distanceG < 15){ // si distance < 15cm
		capteurG = proche;
	}
	else if (distanceG > 60){ // si distance > 60cm
		capteurG = loin;
	}
	
	else{
		capteurG = ok;			// 15<= distance >= 60cm
	}
	//~ transmissionUART(0xf6);
	//~ transmissionUART(interD);
	transmissionUART(0xf7);
	transmissionUART(distanceG);
}

//~ ISR (TIMER2_COMPA_vect){
	
	//~ can  convertisseurD = can();
	
	//~ uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 4);
	//~ //uint8_t distanceD = uint8_t(964.2271747/((float)(lectureDonneeD)-28.6141603));
	//~ uint8_t distanceD = calculDistance(lectureDonneeD);
	//~ transmissionUART(0xf7);
	//~ transmissionUART(distanceD);
	//~ tauxVariationD = (float)(distanceD - mesuresD[pointeurMesureD]) / ECARTENTREMESURES;

	//~ if(pointeurMesureD != 120){
		//~ pointeurMesureD++;
	//~ }
	//~ else {
		//~ pointeurMesureD = 0;
	//~ }

	//~ mesuresD[pointeurMesureD] = distanceD;
//~ }

//~ ISR(TIMER2_COMPB_vect){
	
	//~ can  convertisseurG = can();

	//~ uint8_t lectureDonneeG = lecture8Bit(convertisseurG, 5);
	//~ //uint8_t distanceG = 2478.633156*(pow(lectureDonneeG, -1.125));
	//~ uint8_t distanceG = calculDistance(lectureDonneeG);

	//~ transmissionUART(0xf6);
	//~ transmissionUART(distanceG);

	//~ tauxVariationG = (float)(distanceG - mesuresG[pointeurMesureG]) / ECARTENTREMESURES;

	//~ if (pointeurMesureG != 120) {
		//~ pointeurMesureG++;
	//~ }
	//~ else {
		//~ pointeurMesureG = 0;
	//~ }

	//~ mesuresG[pointeurMesureG] = distanceG;
//~ }
