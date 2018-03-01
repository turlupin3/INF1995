#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> //: Interrupts

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir; 

const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;
const uint32_t OFF = 0b00000000;
const uint32_t ACTIVE_ROUE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t DELAY_ANTIREBOND  = 25;
const uint32_t DELAY_ANTIREBOND_COURT  = 10;
const uint32_t DELAY_SECONDE  = 1000;

void initialisation ();
void ajustementPWM ( uint8_t frequence );


int main() {
	TCNT1 = 0;
	initialisation();
	ajustementPWM(150);
	_delay_ms(2000);
	ajustementPWM(255);
	_delay_ms(2000);
	ajustementPWM(0);
	_delay_ms(2000);
	  return 0; 
}

void initialisation ( ) {

// cli est une routine qui bloque toutes les interruptions.
// Il serait bien mauvais d'être interrompu alors que
// le microcontroleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées
// et les sorties. DDRx... Initialisez bien vos variables

	
DDRB = PORT_SORTIE; // PORT B est en mode sortie
DDRD = PORT_SORTIE; // PORT D est en entree


// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001

// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir
// en ajustant le registre EICRA

EICRA |= (1 << ISC00);
EICRA |= (1 << ISC01);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}



void ajustementPWM (uint8_t frequence) {

// mise à un des sorties OC1A et OC1B sur comparaison

// réussie en mode PWM 8 bits, phase correcte

// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

// page 177 de la description technique du ATmega324PA)

OCR1A = frequence;

OCR1B = frequence;


// division d'horloge par 8 - implique une frequence de PWM fixe

//TCCR1A |= (0 << WGM11);
TCCR1A |= (1 << WGM10);
TCCR1A |= (1 << COM1A1);
TCCR1A |= (1 << COM1B1);
//TCCR1A |= (0 << COM1A0);
//TCCR1A |= (0 << COM1B0);

//TCCR1B |= (0 << CS12);
//TCCR1B |= (1 << CS11);
TCCR1B |= (1 << CS11);

TCCR1C = 0;

}
