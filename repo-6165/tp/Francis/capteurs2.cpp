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
volatile int8_t erreurD = 0;
volatile int8_t erreurG = 0;
volatile float tauxVariationD = 0;
volatile float tauxVariationG = 0;
volatile float integraleD = 0;
volatile float integraleG = 0;
volatile uint8_t pointeurMesureD = 0;
volatile uint8_t pointeurMesureG = 0;
uint8_t mesuresD[120];
uint8_t mesuresG[120];

int main(){
	//~ jouerNote(69);
	//~ _delay_ms(200);
	//~ arreterJouer();
	//~ _delay_ms(50);
	//~ jouerNote(73);
	//~ _delay_ms(200);
	//~ arreterJouer();
	//~ _delay_ms(50);
	//~ jouerNote(76);
	//~ _delay_ms(200);
	//~ arreterJouer();
	
	initialisationUART();
	initialisation();
	initializeSensor();
	setUpPWMoteur();
	
	DDRD = 0xf0;
	while(true){
		
		wallFollow();
		delSwitcher(0);


	}
	
	return 0; 
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

void wallFollow() {

	controleMoteurG(50);
	controleMoteurD(40);
	
	while(mesuresD[pointeurMesureD] < 15){
		delSwitcher(1);
		controleMoteurD(pidD(1, 0, 0));
		}
	while(mesuresD[pointeurMesureD] > 17){
		delSwitcher(2);
		controleMoteurG(pidD(1, 0, 0));
	}
	controleMoteurG(50);
	controleMoteurD(40);

}

uint8_t lecture8Bit(can& conv, uint8_t pos){
    return conv.lecture(pos) >> 2;
}

uint8_t calculDistance(uint8_t &donnee) {

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
	transmissionUART(0xf7);
	transmissionUART(distanceD);

	erreurD = DISTANCEVOULUE - distanceD;
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

	transmissionUART(0xf6);
	transmissionUART(distanceG);

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
