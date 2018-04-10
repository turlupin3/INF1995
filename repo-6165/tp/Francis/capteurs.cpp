#define F_CPU 8000000

#include <ini.h>
#include <memoire_24.h>
#include <UART.h>
#include <pwmoteur.h>
#include <note.h>
#include <manips.h>
#include <ambre.h>
#include <can.h>

void initialisation();
void startTimer ( uint8_t duree );
uint8_t lecture8Bit(can& conv, uint8_t pos);

volatile uint8_t distanceD;
volatile uint8_t distanceG;

int main(){
	
	return 0; 
}
uint8_t lecture8Bit(can& conv, uint8_t pos){
    return conv.lecture(pos) >> 2;
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
	uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 5);
	distanceD = 2478.633156*(pow(lectureDonneeD,-1.125));
	transmissionUART(0xf6);
	transmissionUART(distanceD);


}

ISR(TIMER2_COMPB_vec){
	can  convertisseurG = can();
	uint8_t lectureDonneeG = lecture8Bit(convertisseurG, 4);
	distanceG = 2478.633156*(pow(lectureDonneeG, -1.125));
	transmissionUART(0xf7);
	transmissionUART(distanceG);
}