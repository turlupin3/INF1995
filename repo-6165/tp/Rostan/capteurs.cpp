// distance = 2478,633156x^-1.125
// OCRA = roue droite
// OCRB = roue gauche
// PORTA pour CAN
// PORTB pour moteur
// PORTD pour son
// PORTC pour DEL
// 94 = 15 cm selon la fonction
// 27 = 60 cm selon la fonction

// verifier 180, pile

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
void allerDroit();
void poteau();
void ajustementDroite();
void ajustementGauche();
void faireLeTourDroite();
void faireLeTourGauche();
void changerPanneau();
void initialisation();
bool antiRebond();
void partirMinuterie ();
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
const volatile uint8_t vitesseRoueG = 50;
const volatile uint8_t vitesseRoueD = vitesseRoueG - 8;


int main(){	
	
	initialisationUART();
	initialisation();
	setUpPWMoteur();
	partirMinuterie();
	
	//~ jouerNote(55);	 ///
	//~ _delay_ms(200);	///
	//~ arreterJouer();	///
	
	delSwitcher(1); // 1 = vert/// 2 = rouge /// 0 = off
	
	_delay_ms(250); // Il faut mettre un delay pour que le if fonctionne
	
	if (distanceD < 60){ // permet de savoir quel cote on longe

		longerDroite = true;				   // au debut du parcours
		longerGauche = false; 
	}
	else {

		longerDroite = false;
		longerGauche = true;
	}
	
	allerDroit();
	
	//~ while (true){
		//~ if (longerDroite == true && droitChanger == true){
			//~ if (distanceG < 60){
				//~ jouerNote(69);
				//~ _delay_ms(200);
				//~ arreterJouer();
				//~ changerPanneau();
				//~ if (distangeG < 14 || distanceG > 16){
					//~ ajustementDroite();
				//~ }
			//~ }
			
		//~ }
	//~ }

	while(true){
		if (longerDroite == true){ 		// switch case lorsqu'on longe la droite
			switch(capteurD){
				case proche:
					if(capteurG == loin)
						droitChanger = true;
					if (capteurG == ok && droitChanger == true)
						changerPanneau();
					if(distanceD < 15)
						ajustementDroite(); 
					break;
					
				case ok:
					if(capteurG == loin)
						droitChanger = true;
					if (capteurG == ok && droitChanger == true)
						changerPanneau();
					if (distanceD < 15)
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
					if(distanceG < 15)
						ajustementGauche();
					break;
					
				case ok:
					if (capteurD == loin)
						droitChanger = true;
					if (capteurD == ok && droitChanger == true)
						changerPanneau();
					if (distanceG < 15)
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

// fait le decalage de deux bit CAN car 2 des 10 bits sont inutilises
uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}

void ajustementDroite(){
	// si le robot se dirige vers le panneau on fait tourner la roue droite
	// plus vite.
	//delSwitcher(2);
	if (distanceD < 15){
		controleMoteurD(vitesseRoueD+10);
		while(distanceD < 14 || distanceD > 16){
			if(capteurD == loin){
				if(capteurG == loin){
					faireLeTourDroite();
				}
			}
		}
		allerDroit();
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue gauche
	// plus vite.
	if (distanceD > 15 && distanceD < 20){ 
		controleMoteurG(vitesseRoueG+10);
		while(distanceD < 14 || distanceD > 16){
			if(capteurD == loin){
				if(capteurG == loin){
					faireLeTourDroite();
				}
			}
		}
		allerDroit();
	}

	else if (distanceD >= 20) {
		controleMoteurG(vitesseRoueG+10);
		_delay_ms(250);
		allerDroit();
		while(distanceD > 16){
			if(capteurD == loin){
				if(capteurG == loin){
					faireLeTourDroite();
				}
			}
		}
	}
	//delSwitcher(1);
}

void ajustementGauche(){
	// si le robot se dirige vers le panneau on fait tourner la roue gauche
	// plus vite.
	//delSwitcher(2);
	if (distanceG < 15){
		controleMoteurG(vitesseRoueG+10);
		while(distanceG < 14 || distanceG > 16){
			if(capteurG == loin){
				if(capteurD == loin){
					faireLeTourGauche();
				}
			}
		}
		allerDroit();
	}
	
	// si le robot s'eloigne du panneau on fait tourner la roue droite
	// plus vite.
	if (distanceG > 15 && distanceG < 20){ 
		controleMoteurD(vitesseRoueD+10);	
		while(distanceG < 14 || distanceG > 16){
			if(capteurG == loin){
				if(capteurD == loin){
					faireLeTourGauche();
				}
			}
		}
		allerDroit();
	}

	else if (distanceG >= 20) {
		controleMoteurD(vitesseRoueD+10);
		_delay_ms(250);
		allerDroit();
		while(distanceG > 16){
			if(capteurG == loin){
				if(capteurD == loin){
					faireLeTourGauche();
				}
			}
		}
	}
	//delSwitcher(1);
}

void faireLeTourDroite(){
	// un roue tourne plus vite et l'autre moins vite pour contourner le panneau
	// jusqu'a temps que le capteur capte 15cm
	//delSwitcher(2);
	controleMoteurG(vitesseRoueG+25);
	controleMoteurD(vitesseRoueD);
	while (distanceD >= 15){
	}	 
	allerDroit();
	//delSwitcher(1);
}

void faireLeTourGauche(){
	//delSwitcher(2);
	controleMoteurD(vitesseRoueD+25);
	controleMoteurG(vitesseRoueG);
	while (distanceG >= 15){
	}	 
	allerDroit();
	//delSwitcher(1);
}

void changerPanneau(){
	// La roue droite va tourner plus vite pendant quelque temps pour
	// que le bot s'oriente vers le panneau gauche. Ensuite les roues 
	// sont a la meme vitesse pour qu'il se dirige droit vers le panneau gauche
	//delSwitcher(2);
	if (longerDroite == true){
		controleMoteurD(vitesseRoueD+20); // 55 ARBITRAIRE
		_delay_ms(750); // 300 est arbitraire
		allerDroit();
		longerDroite = false; // avant ou apres while
		longerGauche = true;
		while(distanceG >= 20){
		}
		controleMoteurD(vitesseRoueG+30);
		_delay_ms(700);
		allerDroit();
	}
	
	// meme chose mais avec la roue gauche
	else if (longerGauche == true){
		controleMoteurG(vitesseRoueG+20);
		_delay_ms(750); // 750 est arbitraire
		allerDroit();
		longerGauche = false;
		longerDroite = true;
		while(distanceD >= 20){
			}
		jouerNote(69);
		_delay_ms(200);
		arreterJouer();
		
		controleMoteurD(vitesseRoueD+30);
		_delay_ms(700);
		allerDroit();
	}
	droitChanger = false;
	//delSwitcher(1);
}

void poteau(){
	jouerNote(69);
	_delay_ms(200);
	arreterJouer();
	
	_delay_ms(100);
	
	jouerNote(69);
	_delay_ms(200);
	arreterJouer();
	
	_delay_ms(100);
	
	jouerNote(69);
	_delay_ms(200);
	arreterJouer();
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
	DDRB = 0xff; // sortie
	DDRD = 0xf0; // sortie/entree
	DDRC = 0xff; // sortie
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01) | (0 << ISC00); // sensible falling edge

	sei();
}

ISR(INT0_vect){

	if(antiRebond()){
		boutonPoussoir = 1;
	}
	//delSwitcher(2);
	if(longerDroite == true){
		
		//~ arreterMoteur();
		//~ _delay_ms(750);
		
		longerDroite = false;
		longerGauche = true;
		
		//setUpPWMoteur();
		controleMoteurG(-vitesseRoueG);
		controleMoteurD(vitesseRoueD);
		
		_delay_ms(2200);
		jouerNote(55);
		_delay_ms(200);
		arreterJouer();
		//~ while (distanceG < 14 || distanceG > 16){
		
		//~ }
		
		allerDroit();
	}
	else{
		//~ arreterMoteur();
		//~ _delay_ms(750);
		
		longerGauche = false;
		longerDroite = true;
		
		//setUpPWMoteur();
		controleMoteurG(vitesseRoueG);
		controleMoteurD(-vitesseRoueD);
		
		_delay_ms(2200);
		//~ jouerNote(55);
		//~ _delay_ms(200);
		//~ arreterJouer();
		
		//~ while (distanceD < 14 || distanceD > 16){
		//~ }
		
		allerDroit();
		
	}
	//delSwitcher(1);
	EIFR |= (1 << INTF0);
}



void partirMinuterie () {
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
	transmissionUART(0xf6);
	transmissionUART(distanceD);

}
ISR ( TIMER2_COMPB_vect  ) {
	lectureDonneeG = lecture8Bit(convertisseurG, 5);
	distanceG = 2478.633156*(pow(lectureDonneeG,-1.125));
	if (distanceG < 15){ 
		capteurG = proche;
	}
	else if (distanceG > 60){ 
		capteurG = loin;
	}
	
	else{
		capteurG = ok;			// 15<= distance >= 60cm
	}
	transmissionUART(0xf7);
	transmissionUART(distanceG);
}

void allerDroit(){
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	}
	//~ if(pointeurMesureD != 120){
		//~ pointeurMesureD++;
	//~ }
	//~ else {
		//~ pointeurMesureD = 0;
	//~ }

	//~ mesuresD[pointeurMesureD] = distanceD;
//~ }

	//~ if (pointeurMesureG != 120) {
		//~ pointeurMesureG++;
	//~ }
	//~ else {
		//~ pointeurMesureG = 0;
	//~ }

	//~ mesuresG[pointeurMesureG] = distanceG;
//~ }
