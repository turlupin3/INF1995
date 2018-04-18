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


const volatile uint8_t vitesseRoueG = 65;
const volatile uint8_t vitesseRoueD = vitesseRoueG-2; // roue pas meme vitesse, pour compenser
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
volatile uint8_t pointeurMesureD = 0;
volatile uint8_t pointeurMesureG = 0;
volatile bool obstacle = false;
volatile bool poteau = false;
volatile bool mur = false;
can convertisseurD = can();
can convertisseurG = can();
bool longerDroite;
bool longerGauche;
bool droitChanger = true;
uint8_t mesuresD[120];
uint8_t mesuresG[120];
uint8_t medianD[7];
uint8_t medianG[7];
uint8_t waitTime = 0;

volatile uint8_t boutonPoussoir = 0x01;
volatile bool boole = 0x01;


//fonctions
uint8_t lecture8Bit(can& conv, uint8_t pos);
void determinerEtat();
void allerDroit();
void quelCote();
void detecterObstacle();
void determinerObstacle();
void changerMur();
void wallFollow();
void jouerSonPoteau();
int8_t pidD(float kp, float ki, float kd);
int8_t pidG(float kp, float ki, float kd);
void ajustementDroite();
void ajustementGauche();
void faireLeTour();
void faireLeTourDroite();
void faireLeTourGauche();
void initialisation();
bool antiRebond();
void partirMinuterie();
uint8_t calculerDistance(uint8_t donnee);
void sequence(uint8_t noteI,uint8_t noteII,uint8_t noteIII);
void sort(uint8_t * donnees, uint8_t size);
uint8_t pastData(uint8_t* donnees, uint8_t ptr, uint8_t byHowMuch);

void envoieInfo(Memoire24CXXX &memoire);
void ecrireInfo(Memoire24CXXX &memoire);
void initialisationInt();
uint8_t getBouton();

volatile bool diagnostic;
uint8_t reset;
Memoire24CXXX memoire = Memoire24CXXX();

void parcours();
void diag();


int main(){
	
	memoire.lecture(100, &reset);
	reset = !reset;
	memoire.ecriture(100, reset);
	
	
	//~ parcours();
	if(reset){
		diagnostic = true;
		diag();
	}
	else {
		diagnostic = false;
		parcours();
	}
	return 0;
}



void parcours(){	
	diagnostic = false;
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
				transmissionUART('L');
				transmissionUART(' ');
				wallFollow();
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
				transmissionUART('M');
				transmissionUART(' ');
				changerMur();
				break;
				
			case detectionPoteau:
				transmissionUART('P');
				transmissionUART(' ');	
				jouerSonPoteau();			
				break;
				
			default: 
				transmissionUART('D');
				transmissionUART(' ');
		};	
	} 
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
	//~ _delay_ms(200);
	if (longerDroite){
		ajustementDroite();
	}
	else if (longerGauche){
		ajustementGauche();
	}
}

int8_t pidD(float kp, float ki, float kd) {
	
	int8_t retour = ( (kp * erreurD) + (ki * integraleD) + (kd * tauxVariationD) );
	if (abs(retour) > 35) {
		return 100;
	}
	else {
		return abs(retour)+65;
	}
}
int8_t pidG(float kp, float ki, float kd) {
	
	int8_t retour = ( (kp * erreurG) + (ki * integraleG) + (kd * tauxVariationG) );
	if (abs(retour) > 35) {
		return 100;
	}
	else {
		return abs(retour)+65;
	}
}

void ajustementDroite(){
	
	delSwitcher(2);	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	
	while(mesuresD[pointeurMesureD] < 15 && etat == longerMur){
		//delSwitcher(1);
		controleMoteurD(pidD(2, 0.0, 0));
		if(mesuresD[pointeurMesureD] < 11){
			//controleMoteurG(100);
			controleMoteurD(100);
		}
	}
	while(mesuresD[pointeurMesureD] > 16 && etat == longerMur){
		//delSwitcher(2);
		controleMoteurG(pidD(1.5, 0.0, 0));
		if(mesuresD[pointeurMesureD] > 25){
			controleMoteurG(100);
			controleMoteurD(50);
		}
	}

	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	delSwitcher(1);
}

void ajustementGauche(){
	
	delSwitcher(2);	
	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	
	while(mesuresG[pointeurMesureG] < 15 && etat == longerMur){
		//delSwitcher(1);
		controleMoteurG(pidG(1, 0.01, 0));
	}	
	while(mesuresG[pointeurMesureG] > 16 && etat == longerMur){
		//delSwitcher(2);
		controleMoteurD(pidG(1, 0.01, 0));
		if(mesuresG[pointeurMesureG] > 25){
			controleMoteurD(100);
			controleMoteurG(50);
		}
	}

	controleMoteurG(vitesseRoueG);
	controleMoteurD(vitesseRoueD);
	delSwitcher(1);
}

void detecterObstacle(){
	if(longerDroite && !obstacle){
		if( (pastData(mesuresG, pointeurMesureG, 20) - mesuresG[pointeurMesureG]) >= 10){
			obstacle = true;
			mur = false;
			poteau = false;
			waitTime = 0;
		}	
	}
	else if(longerGauche && !obstacle){
		if( pastData(mesuresD, pointeurMesureD, 20) - abs(mesuresD[pointeurMesureD]) >= 10){
			obstacle = true;
			mur = false;
			poteau = false;
			waitTime = 0;
		}	
	}	
}

void determinerObstacle(){
	cli();
	if(longerDroite && obstacle && waitTime > 40){
		if( abs(mesuresG[pointeurMesureG] - pastData(mesuresG, pointeurMesureG, 35)) >= 10){
			poteau = true;
			mur = false;
		}
		else {
			mur = true;
			poteau = false;
		}
		waitTime = 0;
		obstacle = false;
	}
	else if(longerGauche && obstacle && waitTime > 40){
		if( abs(mesuresD[pointeurMesureD] - pastData(mesuresD, pointeurMesureD, 35)) >= 10){
			poteau = true;
			mur = false;
		}
		else {
			mur = true;
			poteau = false;
		}
		waitTime = 0;
		obstacle = false;
	}
	
	else{
		waitTime++;
		mur = false;
		poteau = false;
	}
	sei();
}

void changerMur(){
	// La roue droite va tourner plus vite pendant quelque temps pour
	// que le bot s'oriente vers le panneau gauche. Ensuite les roues 
	// sont a la meme vitesse pour qu'il se dirige droit vers le panneau gauche
	delSwitcher(2);
	if (longerDroite == true){
		controleMoteurD(100);
		//controleMoteurG(55); // 55 ARBITRAIRE
		_delay_ms(750); // 300 est arbitraire
		allerDroit();
		
		while(distanceG >= 20){
		}
		longerDroite = false; // avant ou apres while
		longerGauche = true;
		controleMoteurD(80);
		_delay_ms(700);
		allerDroit();
	}
	
	// meme chose mais avec la roue gauche
	else if (longerGauche == true){
		controleMoteurG(100);
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
	delSwitcher(1);
}

void jouerSonPoteau(){
	sequence(69,69,69);
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

void determinerEtat(){
	

	
	if (longerDroite){ // On redonne droit de changer si detecte rien
		
		//delSwitcher(1);

		if (distanceG > 60) {
			droitChanger = true;
		}
		
		//~ if(distanceD > 60 && distanceG > 60){
			//~ etat = faireTour;
		//~ }
	
		else if (distanceG < 60 && droitChanger && mur){
			etat = changerPan;
		}
		
		else if (distanceG < 60 && poteau){
			etat = detectionPoteau;
		}

		else {
			etat = longerMur;
		}
	}
	
	else if (longerGauche){
		
		//delSwitcher(2);

		
		if (distanceD > 60) {
			droitChanger = true;
		}		
		
		//~ if(distanceG > 60 && distanceD > 60){
			//~ etat = faireTour;
		//~ }
		
		else if (distanceD < 60 && droitChanger && mur){
			etat = changerPan;
		}
		
		else if (distanceD < 60 && poteau){
			etat = detectionPoteau;
		}
		

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
		return 85;
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

uint8_t pastData(uint8_t* donnees, uint8_t ptr, uint8_t byHowMuch){
	if( (ptr - byHowMuch) < 0){
		return donnees[120-abs(ptr - byHowMuch)];
	}
	else {
		return donnees[(ptr - byHowMuch)];
	}
}



void diag(){
	diagnostic = true;
	DDRD = 0xf0;
	//demarrage();

	initialisationUART();
	initialisationInt();	
	setUpPWMoteur();
	jouerNote(55);	
	_delay_ms(200);
	arreterJouer();

	
	//ecrireInfo(memoire);
	_delay_ms(10);
	envoieInfo(memoire);
	partirMinuterie(); ///MAKE A SECONDE UART ELSE INT BUTTON IS NOT CLEAN///	
	struct operation{
		unsigned char instruction; 
		int8_t operande;
		};
		
	operation op;	 
for(;;){ 
		op.instruction= receptionUART();
		op.operande = receptionUART();
		
		switch (op.instruction) {
			case 0xf8: //vitesse de roue gauche

				controleMoteurG(op.operande);

				break;
			case 0xf9: //vitesse de rouge droite

				controleMoteurD(op.operande);

				break;
			case 0xfa: // couleur de la del libre
				
				delSwitcher(op.operande);

				break;
			case 0xfb: // requete denvoi des information didentification
				envoieInfo(memoire);
				break;
			default:
				//error
				break;
		}
	}
}
	
void envoieInfo(Memoire24CXXX &memoire) {
	cli();
		uint8_t bit;
		for (int i = 0; i < 33; i++){
			memoire.lecture(i, &bit);
			transmissionUART(bit);
		}
		sei();
	}

void ecrireInfo(Memoire24CXXX &memoire){
	
		uint8_t addresseNomRobot = 0xf0;
		uint8_t addresseNumeroEquipe = 0xf1;
		uint8_t addresseNumeroSection = 0xf2;
		uint8_t addresseSession = 0xf3;
		uint8_t addresseCouleur = 0xf4;
		
		uint8_t nomRobot[13] = "JRomBot";
		uint8_t numeroEquipe[9] = "6165";
		uint8_t numeroSection = 3;
		uint8_t session[4] = {'1','8','-','1'};
		uint8_t couleur = 1;
		
		memoire.ecriture(0, addresseNomRobot);
		memoire.ecriture(1, nomRobot, 13);
		memoire.ecriture(14, addresseNumeroEquipe);
		memoire.ecriture(15, numeroEquipe, 9);
		memoire.ecriture(24, addresseNumeroSection);
		memoire.ecriture(25, numeroSection);
		memoire.ecriture(26, addresseSession);
		memoire.ecriture(27, session, 4);
		memoire.ecriture(31, addresseCouleur);
		memoire.ecriture(32, couleur);
		
	}

void initialisationInt(){
	cli();
	//DDRA = 0xff;
	//DDRD = 0x00;
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request
	
	sei();
}



ISR(INT0_vect){
	cli();
	
	if(antiRebond()){
		boutonPoussoir = 1;
	}
	
	if(diagnostic){
		_delay_ms(25);
		boole = !boole;
		transmissionUART(0xf5);
		transmissionUART(boole);
	}
	else {
		delSwitcher(2);
		if(longerDroite){

			controleMoteurG(-vitesseRoueG);
			controleMoteurD(vitesseRoueD);
			
			_delay_ms(2000);
			
			//~ while (tmp + 3 < distanceG){
			
			//~ }
			allerDroit();
			//~ longerDroite = false;
			//~ longerGauche = true;
			quelCote();
		}
		else if(longerGauche){
			
			controleMoteurG(vitesseRoueG);
			controleMoteurD(-vitesseRoueD);
			
			_delay_ms(2000);
			
			//~ while (tmp + 3 < distanceD){
			//~ }
			
			allerDroit();
			//~ longerGauche = false;
			//~ longerDroite = true;
			quelCote();
		}
		delSwitcher(1);
	}
	EIFR |= (1 << INTF0);
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

	//~ detecterObstacle();
	//~ determinerObstacle();

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
	if(!diagnostic){
		detecterObstacle();
		determinerObstacle();
	}
	transmissionUART(0xf6);
	transmissionUART(distanceG);
	
	determinerEtat();
	sei();
}


