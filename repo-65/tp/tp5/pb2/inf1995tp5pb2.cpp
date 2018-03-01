#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> //: Interrupts

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir; 
volatile uint8_t lockInt;

const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;
const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t DELAY_ANTIREBOND  = 25;
const uint32_t DELAY_ANTIREBOND_COURT  = 10;
const uint32_t DELAY_SECONDE  = 3000;

void initialisation ();
void partirMinuterie ( uint32_t duree );


int main() {

	boutonPoussoir = 0;
	initialisation();
	//cli();
		PORTB = VERT;
		_delay_ms(DELAY_SECONDE);
		PORTB = ROUGE;
		_delay_ms(100);
		PORTB = OFF;
		boutonPoussoir = 0;
	partirMinuterie(8000000);
	//sei();

	do {

		// attendre qu'une des deux variables soit modifiée

		// par une ou l'autre des interruptions.
		if (minuterieExpiree == 1) {
			PORTB = ROUGE;
		}

	} while ( minuterieExpiree == 0 || boutonPoussoir == 0 );
		
	PORTB = VERT;

	// Une interruption s'est produite. Arrêter toute

	// forme d'interruption. Une seule réponse suffit.

	cli ();

	// Verifier la réponse

	//'modifier ici'
	
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
DDRD = PORT_ENTREE; // PORT D est en entree


// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001

// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir
// en ajustant le registre EICRA

EICRA |= (1 << ISC00);
EICRA |= (1 << ISC01);

lockInt = 0;

// sei permet de recevoir à nouveau des interruptions.

sei ();

}

ISR ( TIMER1_COMPA_vect  ) {

minuterieExpiree = 1;
//COMPA |= (1 << );
}


ISR ( INT0_vect ) {

	_delay_ms(30);

	boutonPoussoir = 1;

// anti-rebond


}

void partirMinuterie ( uint32_t duree ) {

minuterieExpiree = 0;

// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT1 = 0;

OCR1A = duree;

TCCR1A |= (0 << COM1A1);
TCCR1A |= (0 << COM1B1);
TCCR1A |= (0 << COM1A0);
TCCR1A |= (0 << COM1B0);

TCCR1B |= (1 << CS12);
TCCR1B |= (0 << CS11);
TCCR1B |= (1 << CS10);


TCCR1C = 0;

TIMSK1 |= (1 << OCIE1A);

}
