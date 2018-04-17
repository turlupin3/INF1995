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

#include <time.h> 

//clock_t t;
// t = clock();
//t = clock() - t;
//=((float)t)/CLOCKS_PER_SEC)

//variables et constantes
enum  {
	      longerMur, faireTour, changerPan, detectionPoteau
} volatile etat;


const volatile uint8_t vitesseRoueG = 55;
const volatile uint8_t vitesseRoueD = vitesseRoueG - 11; // roue pas meme vitesse, pour compenser
volatile uint8_t distanceD = 0;
volatile uint8_t distanceG = 0;
volatile const float ECARTENTREMESURES = 0.03264;
volatile const uint8_t DISTANCEVOULUE = 15;
volatile int8_t erreurD = 0;
volatile int8_t erreurG = 0;
volatile float tauxVariationD = 0;
volatile float tauxVariationG = 0;
volatile float integraleD = 0;
volatile float integraleG = 0;
volatile uint8_t lectureDonneeD= 0;
volatile uint8_t lectureDonneeG= 0;
volatile uint8_t boutonPoussoir = 0;
volatile uint8_t pointeurMesureD = 0;
volatile uint8_t pointeurMesureG = 0;
volatile bool estPoteau = false;
can convertisseurD = can();
can convertisseurG = can();
bool longerDroite;
bool longerGauche;
bool droitChanger = true;
uint8_t mesuresD[120];
uint8_t mesuresG[120];
uint8_t medianD[7];
uint8_t medianG[7];

//fonctions
uint8_t lecture8Bit(can& conv, uint8_t pos);
void determinerEtat();
void allerDroit();
void quelCote();
void determinerObstacle();
void poteau();
void wallFollow();
int8_t pidD(float kp, float ki, float kd);
int8_t pidG(float kp, float ki, float kd);
void ajustementDroite();
void ajustementGauche();
void faireLeTour();
void faireLeTourDroite();
void faireLeTourGauche();
void changerPanneau();
void initialisation();
bool antiRebond();
void partirMinuterie();
uint8_t calculerDistance(uint8_t donnee);
void sequence(uint8_t noteI,uint8_t noteII,uint8_t noteIII);
void sort(uint8_t * donnees, uint8_t size);


//~ jouerNote(55);
//~ _delay_ms(200);
//~ arreterJouer();

//~ delSwitcher(1); // 1 = vert/// 2 = rouge /// 0 = off
int main(){	
	
	initialisationUART();
	initialisation();
	setUpPWMoteur();
	partirMinuterie();
	
	
	jouerNote(69);
	_delay_ms(200);
	arreterJouer();
	_delay_ms(50);
	jouerNote(73);
	_delay_ms(200);
	arreterJouer();
	_delay_ms(50);
	jouerNote(76);
	_delay_ms(200);
	arreterJouer();
	
	_delay_ms(250); // Il faut mettre un delay pour que le if fonctionne
	
	quelCote();
	allerDroit();

	while (true){
		
		determinerEtat();
		
		switch (etat){
			case longerMur:
				//~ transmissionUART('L');
				//~ transmissionUART(' ');
				wallFollow();
				delSwitcher(1);
				
				break;
			
				
			//~ case faireTour:
				//~ //transmissionUART('T');
				//~ //transmissionUART(' ');
				//~ controleMoteurG(0);
				//~ controleMoteurD(0);
				//~ delSwitcher(0);
				//~ allerDroit();
				//~ faireLeTour();
				
				
				//~ break;
				
			case changerPan:
				//~ transmissionUART('P');
				//~ transmissionUART(' ');
				delSwitcher(2);
				changerPanneau();
				
				break;
				
			case detectionPoteau:
				poteau();
				break;
			default: 
				_delay_ms(50);
				delSwitcher(0);
				sequence(60,50,40);
				//transmissionUART('D');
				//transmissionUART(' ');
		};	
	}
	return 0; 
}



// fait le decalage de deux bit CAN car 2 des 10 bits sont inutilises
uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
}

void allerDroit(){
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	}

// permet de savoir quel cote on longe au debut
void quelCote(){
	if (distanceD < distanceG){ 
		longerDroite = true;
		longerGauche = false; 
	}
	else if (distanceG <= distanceD) {
		longerDroite = false;
		longerGauche = true;
	}
}
	
void wallFollow(){
	_delay_ms(200);
	if (longerDroite){
		ajustementDroite();
	}
	else if (longerGauche){
		ajustementGauche();
	}
}

int8_t pidD(float kp, float ki, float kd) {
	
	int8_t retour = ( (kp * erreurD) + (ki * integraleD) + (kd * tauxVariationD) );
	if (abs(retour) > 30) {
		return 80;
	}
	else {
		return abs(retour)+50;
	}
}
int8_t pidG(float kp, float ki, float kd) {
	
	int8_t retour = ( (kp * erreurG) + (ki * integraleG) + (kd * tauxVariationG) );
	if (abs(retour) > 30) {
		return 80;
	}
	else {
		return abs(retour)+50;
	}
}

void ajustementDroite(){
	
	//delSwitcher(2);	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	
	while(mesuresD[pointeurMesureD] < 14 && etat == longerMur){
		//delSwitcher(1);
		controleMoteurD(pidD(1, 0, 0));
	}
	while(mesuresD[pointeurMesureD] > 16 && etat == longerMur){
		//delSwitcher(2);
		controleMoteurG(pidD(1.22, 0, 0));
		if(mesuresD[pointeurMesureD] > 20){
			controleMoteurG(100);
		}
	}

	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	//delSwitcher(1);
}

void ajustementGauche(){
	
	//delSwitcher(2);	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	
	while(mesuresG[pointeurMesureG] < 14 && etat == longerMur){
		//delSwitcher(1);
		controleMoteurG(pidG(1.21, 0, 0));
	}	
	while(mesuresG[pointeurMesureG] > 16 && etat == longerMur){
		//delSwitcher(2);
		controleMoteurD(pidG(1, 0, 0));
		if(mesuresG[pointeurMesureG] > 20){
			controleMoteurD(100);
		}
	}

	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	//delSwitcher(1);
}

void faireLeTour(){
	if (longerDroite == true){
		faireLeTourDroite();
	}
	if (longerGauche == true){
		faireLeTourGauche();
	}
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
		
		while(distanceG >= 20){
		}
		longerDroite = false; // avant ou apres while
		longerGauche = true;
		controleMoteurD(vitesseRoueG+30);
		_delay_ms(700);
		allerDroit();
	}
	
	// meme chose mais avec la roue gauche
	else if (longerGauche == true){
		controleMoteurG(vitesseRoueG+20);
		_delay_ms(750); // 750 est arbitraire
		allerDroit();
		
		while(distanceD >= 20){
			}
		longerGauche = false;
		longerDroite = true;
		controleMoteurD(vitesseRoueD+30);
		_delay_ms(700);
		allerDroit();
	}
	droitChanger = false;
	//delSwitcher(1);
}

void determinerObstacle(){
	//~ if (longerDroite == true){
		//~ uint8_t temp = distanceG;
		//~ _delay_ms(750);
		//~ if (temp == distanceG + 8 || temp == distanceG - 8){ // avec le timer, la distanceG aura changer
			//~ estMur = true;
			//~ estPoteau = false;
		//~ }
		//~ else
	//~ }
			 
	
	//~ if (longerGauche == true{
		
	//~ }
}

void poteau(){
	arreterMoteur();
	
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
	
	setUpPWMoteur();
	allerDroit();
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
		longerDroite = false;
		longerGauche = true;

		controleMoteurG(-vitesseRoueG);
		controleMoteurD(vitesseRoueD);
		
		_delay_ms(2200);
		
		//~ while (distanceG < 14 || distanceG > 16){
		
		//~ }
		
		allerDroit();
	}
	else{
		
		longerGauche = false;
		longerDroite = true;
		
		controleMoteurG(vitesseRoueG);
		controleMoteurD(-vitesseRoueD);
		
		_delay_ms(2200);
		
		//~ while (distanceD < 14 || distanceD > 16){
		//~ }
		
		allerDroit();
		
	}
	//delSwitcher(1);
	EIFR |= (1 << INTF0);
}

void determinerEtat(){
	

	
	if (longerDroite){ // On redonne droit de changer si detecte rien
		
		//~ jouerNote(55);

		if (distanceG > 60) {
			droitChanger = true;
		}
		
		//~ if(distanceD > 60 && distanceG > 60){
			//~ etat = faireTour;
		//~ }
	
		else if (distanceG < 60 && droitChanger == true && estPoteau == false){
			etat = changerPan;
		}
		
		//~ else if (distanceG < 60 && estPoteau == true){
			//~ etat = detectionPoteau;
		//~ }

		//~ else if (distanceG < 60){ // des qu'il detecte un obstacle, on doit determiner le type (mur ou poteau)
			//~ determinerObstacle();
		//~ }

		else {
			etat = longerMur;
		}
	}
	
	else if (longerGauche){
		
		//~ jouerNote(48);

		
		if (distanceD > 60) {
			droitChanger = true;
		}		
		
		//~ if(distanceG > 60 && distanceD > 60){
			//~ etat = faireTour;
		//~ }
		
		else if (distanceD < 60 && droitChanger == true && estPoteau == false){
			etat = changerPan;
		}
		
		//~ else if (distanceD < 60 && estPoteau == true){
			//~ etat = detectionPoteau;
		//~ }
		
		//~ else if (distanceD < 60){
			//~ determinerObstacle();
		//~ }

		else {
			etat = longerMur;
		}
	}
	

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

ISR ( TIMER2_COMPA_vect  ) { // timer pour capteurD
	cli();

	mesuresD[pointeurMesureD] = distanceD;
	lectureDonneeD = lecture8Bit(convertisseurD, 4);
	distanceD = calculerDistance(lectureDonneeD);
	
	erreurD = DISTANCEVOULUE - distanceD;
	tauxVariationD = (float)(erreurD) / ECARTENTREMESURES;
	integraleD += (float)(DISTANCEVOULUE-distanceD)* ECARTENTREMESURES;
	
	if(pointeurMesureD != 120){
		pointeurMesureD++;
	}
	else {
		pointeurMesureD = 0;
	}
	medianD[pointeurMesureD % 7] = distanceD;
	sort(medianD, 7);
	distanceD = medianD[4];
	mesuresD[pointeurMesureD] = medianD[4];

	transmissionUART(0xf7);
	transmissionUART(distanceD);
	sei();
}

ISR ( TIMER2_COMPB_vect  ) { // timer pour capteurG
	cli();
	lectureDonneeG = lecture8Bit(convertisseurG, 5);
	distanceG = calculerDistance(lectureDonneeG);
	
	erreurG = (distanceG - DISTANCEVOULUE);
	tauxVariationG = (float)(erreurG) / ECARTENTREMESURES;
	integraleG += (float)(DISTANCEVOULUE-distanceD)* ECARTENTREMESURES;
	
	if (pointeurMesureG != 120) {
		pointeurMesureG++;
	}
	else {
		pointeurMesureG = 0;
	}

	medianG[pointeurMesureG % 7] = distanceG;
	sort(medianG, 7);
	distanceG = medianG[4];
	mesuresG[pointeurMesureG] = medianG[4];
	

	transmissionUART(0xf6);
	transmissionUART(distanceG);
	determinerEtat();
	sei();
}

void sequence(uint8_t noteI,uint8_t noteII,uint8_t noteIII) {
	jouerNote(noteI);
	_delay_ms(150);
	arreterJouer();
	_delay_ms(25);
	jouerNote(noteII);
	_delay_ms(150);
	arreterJouer();
	_delay_ms(25);
	jouerNote(noteIII);
	_delay_ms(150);
	arreterJouer();
}

uint8_t calculerDistance(uint8_t donnee){
	//~ distanceG = 2478.633156*(pow(lectureDonneeG,-1.125));
	//~ if(distanceG > 65){
		//~ distanceG = 65;
	//~ }
	//~ if(distanceG < 10){
		//~ distanceG = 10;
	//~ }
	
	//~ distanceD = 2478.633156*(pow(lectureDonneeD,-1.125));
	//~ if(distanceD > 65){
		//~ distanceD = 65;
	//~ }
	//~ if(distanceD < 10){
		//~ distanceD = 10;
	//~ }
	
		if (donnee > 132) {
		return 10;
	}
	else if (donnee > 91) {
		return (uint8_t)(-0.12 * (float)(donnee)+26.1);
	}
	else if (donnee > 70) {
		return (uint8_t)(-0.24 * (float)(donnee)+36.67);
	}
	else if (donnee > 49) {
		return (uint8_t)(-0.48 * (float)(donnee)+53.33);
	}
	else if (donnee > 37) {
		return (uint8_t)(-0.83 * (float)(donnee)+70.83);
	}
	else if (donnee > 22) {
		return (uint8_t)(-2 * (float)(donnee)+114);
	}
	else {
		return 75;
	}
}

void sort(uint8_t * donnees, uint8_t size){
	bool swapped = true;
	uint8_t tmp;
	do {
		swapped = false;
		for(uint8_t i = 1; i < size; i++){
			if(donnees[i] > donnees[i-1]){
				tmp = donnees[i];
				donnees[i] = donnees[i-1];
				donnees[i-1] = tmp;
				swapped = true;
			}
		}
	}
	while(swapped);
}
