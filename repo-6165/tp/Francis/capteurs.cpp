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

void initialisation();
void initializeSensor();
uint8_t lecture8Bit(can& conv, uint8_t pos);
uint8_t calculDistance(uint8_t &donnee);
void wallFollow();
int8_t pidD(float kp, float ki,float  kd);

volatile const float ECARTENTREMESURES = 0.03264;
volatile const uint8_t DISTANCEVOULUE = 15;
volatile uint8_t erreurD = 0;
volatile uint8_t erreurG = 0;
volatile float tauxVariationD = 0;
volatile float tauxVariationG = 0;
volatile float integraleD = 0;
volatile float integraleG = 0;
volatile uint8_t pointeurMesureD = 0;
volatile uint8_t pointeurMesureG = 0;
uint8_t mesuresD[120];
uint8_t mesuresG[120];

int main(){
	initialisationUART();
	initialisation();
	initializeSensor();
	setUpPWMoteur();
	
	
	while(true){
		
		wallFollow();
		PORTC=1;


	}
	
	return 0; 
}

int8_t pidD(float kp, float ki, float kd) {
	
	int8_t retour = (kp * erreurD + ki * integraleD + kd * tauxVariationD);
	if (retour > 100) {
		return 100;
	}
	else if (retour < 0) {
		return abs(retour);
	}
	else {
		return retour;
	}
}

void wallFollow() {

	controleMoteurG(75);
	controleMoteurD(55);
	
	while(mesuresD[pointeurMesureD] < 12){
		PORTC=2;
		controleMoteurD(pidD(0.5, 0, 0));
		}
	while(mesuresD[pointeurMesureD] > 17){
		PORTC = 0;
		controleMoteurG(pidD(0.5, 0, 0));
	}
	controleMoteurG(75);
	controleMoteurD(55);

}

uint8_t lecture8Bit(can& conv, uint8_t pos){
    return conv.lecture(pos) >> 2;
}

uint8_t calculDistance(uint8_t &donnee) {

	if (donnee > 130) {
		return 10;
	}
	else if (donnee > 92) {
		return (uint8_t)(-0.13 * (float)(donnee)+27.11);
	}
	else if (donnee > 61) {
		return (uint8_t)(-0.32 * (float)(donnee)+44.68);
	}
	else if (donnee > 53) {
		return (uint8_t)(-1.25 * (float)(donnee)+101.25);
	}
	else if (donnee > 48) {
		return (uint8_t)(-5 * (float)(donnee)+300);
	}
	else {
		return 65;
	}
}

void initialisation(){
	cli();
	DDRA = 0x00; // entree
	DDRB = 0x00; //entree
	DDRD = 0xff; // sortie
	DDRC = 0xff; // sortie
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01) | (0 << ISC00); // sensible falling edge

	sei();
}

ISR(INT0_vect){

}

void initializeSensor() {
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

ISR (TIMER2_COMPA_vect){
	
	can  convertisseurD = can();
	
	uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 4);
	//uint8_t distanceD = uint8_t(964.2271747/((float)(lectureDonneeD)-28.6141603));
	uint8_t distanceD = calculDistance(lectureDonneeD);
	//~ transmissionUART(0xf7);
	transmissionUART(distanceD);

	erreurD = distanceD - DISTANCEVOULUE;
	tauxVariationD = (float)(erreurD) / ECARTENTREMESURES;
	integraleD += (float)(distanceD - DISTANCEVOULUE)* ECARTENTREMESURES;

	if(pointeurMesureD != 120){
		pointeurMesureD++;
	}
	else {
		pointeurMesureD = 0;
	}

	mesuresD[pointeurMesureD] = distanceD;
}

ISR(TIMER2_COMPB_vect){
	
	can  convertisseurG = can();

	uint8_t lectureDonneeG = lecture8Bit(convertisseurG, 5);
	//uint8_t distanceG = 2478.633156*(pow(lectureDonneeG, -1.125));
	uint8_t distanceG = calculDistance(lectureDonneeG);

	//~ transmissionUART(0xf6);
	//~ transmissionUART(distanceG);

	erreurG = (distanceG - DISTANCEVOULUE);
	tauxVariationG = (float)(erreurG) / ECARTENTREMESURES;
	integraleG += (float)(erreurG)* ECARTENTREMESURES;

	if (pointeurMesureG != 120) {
		pointeurMesureG++;
	}
	else {
		pointeurMesureG = 0;
	}

	mesuresG[pointeurMesureG] = distanceG;
}
