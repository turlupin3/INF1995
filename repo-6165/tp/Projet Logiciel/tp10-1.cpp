#include <ini.h>
#include <memoire_24.h>
#include <UART.h>
#include <pwmoteur.h>
#include <note.h>
#include <manips.h>
#include <ambre.h>
#include <can.h>

void envoieInfo(Memoire24CXXX memoire);
void ecrireInfo(Memoire24CXXX memoire);
ISR(INT0_vect);
bool antiRebond();
void initialisationInt();
uint8_t getBouton();

void partirMinuterie ( uint32_t duree );

//capteur
uint8_t lecture8Bit(can& conv, uint8_t pos);

volatile uint8_t boutonPoussoir = 0x01;
volatile bool boole = 0x01;
//capteur
//volatile can  convertisseurD = can();
//
int main() {
	demarrage();
	Memoire24CXXX memoire = Memoire24CXXX();
	initialisationUART();
	initialisationInt();
	setUpPWMoteur();
	partirMinuterie(195);	
	
	
	//ecrireInfo(memoire);
	_delay_ms(10);
	envoieInfo(memoire);
	
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
				//envoieInfo(memoire);
				break;
			default:
				//error
				break;
		}
	}	
	
	
	return 0;
}
	void envoieInfo(Memoire24CXXX memoire) {
		uint8_t bit;
		for (int i = 0; i < 33; i++){
			memoire.lecture(i, &bit);
			transmissionUART(bit);
		}
		
	}

	void ecrireInfo(Memoire24CXXX memoire){
	
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
////////////////////////////////////////////////////////////////////////
bool antiRebond(){
	DDRD = 0x00;
	bool estEnfonce = false;
	if(PIND & 0x04){
		_delay_ms(10.0);
		if(PIND & 0x04){
		estEnfonce = true;;
		}
	}
	return estEnfonce;
}


void initialisationInt(){
	cli();
	DDRA = 0xff;
	DDRD = 0x00;
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (0 << ISC01) | (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request

	sei();
}

ISR(INT0_vect){
	//rising and failling edge
	_delay_ms(25);
	boole = !boole;
	transmissionUART(0xf5);
	transmissionUART(boole);
		
	EIFR |= (1 << INTF0);
}

uint8_t getBouton(){
	
	return boutonPoussoir;
}

////////////////////////////////////////////////////////////////////////
//Partie Timer

//main here
//partirMinuterie(100000);
//


void partirMinuterie ( uint32_t duree ) {



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

}

ISR ( TIMER2_COMPA_vect  ) {
	//~ jouerNote(45);
	//~ _delay_ms(200);
	//~ arreterJouer();
	DDRA = 0x00;
	can  convertisseurD = can();
	can  convertisseurG = can();
	uint8_t lectureDonneeD = lecture8Bit(convertisseurD, 5);
	uint8_t lectureDonneeG = lecture8Bit(convertisseurG, 4);
	double interD = 2478.633156*(pow(lectureDonneeD,-1.125));
	double interG = 2478.633156*(pow(lectureDonneeG,-1.125));
	transmissionUART(0xf6);
	transmissionUART(interD);
	transmissionUART(0xf7);
	transmissionUART(interG);

}
////////////////////////////////////////////////////////////////////////
uint8_t lecture8Bit(can& conv, uint8_t pos){
	return conv.lecture(pos) >> 2;
	//return 2478,633156*((conv.lecture(pos) >> 2)^-1.125);
}
////////////////////////////////////////////////////////////////////////
