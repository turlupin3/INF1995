/*
 * Nom: TP7 Problème 1
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 26 fevrier 2018
 * Description: Programme qui clignote une lumiere rouge selon le temps pese sur un
 * 				bouton-poussoir.
*/

#define F_CPU 8192000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> //: Interrupts

const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint8_t 	COMPTEUR_MAX = 	120;

volatile uint8_t compteur = 0;
volatile uint8_t minuterieExpiree = 0;

void initialisation ();
void partirMinuterie ( uint32_t duree );
void clignoterCompteur(uint8_t compteur);

int main() {
	
	initialisation();
	bool operation = true;
	partirMinuterie(8000);
	
	while (true) {
		if (PIND & 0x01) {
			operation = true;
			while (operation == true) {		
				if (compteur >= COMPTEUR_MAX || PIND == 0x00) {
					PORTB = VERT;
					_delay_ms(500);
					PORTB = OFF;
					_delay_ms(2000);
					clignoterCompteur(compteur);
					PORTB = VERT;
					_delay_ms(1000);
					PORTB = OFF;
					compteur = 0;
					operation = false;
				}
			}
		}
	}
	return 0;
}

void clignoterCompteur(uint8_t compteur) {
	
	uint8_t rep = compteur/2;
	for (int i = 0; i < rep; i++) {
	PORTB = ROUGE;
	_delay_ms(250);
	PORTB = OFF;
	_delay_ms(250);
	}
}

ISR ( TIMER1_COMPA_vect  ) {

	if (PIND != 0x00) {
		compteur+= 10;
	}
}

void initialisation ( ) {
	DDRD = 0x00; // PORT D est en mode entree
	DDRB = 0xFF; // PORT B est en mode sortie

	cli ();

	// configurer et choisir les ports pour les entrées
	// et les sorties. DDRx... Initialisez bien vos variables

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes

	EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA

	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC01);

	sei ();

}

void partirMinuterie ( uint32_t duree ) {

	minuterieExpiree = 0;

	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée

	TCNT1 = 0;
	
	OCR1A = duree;
	
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12);
	TCCR1B |= (0 << CS11);
	TCCR1B |= (1 << CS10);
	
	TCCR1C = 0;
	
	TIMSK1 |= (1 << OCIE1A);

}
	
	


