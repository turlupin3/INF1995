/*
 * Nom: TP7 Problème 2
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 26 fevrier 2018
 * Description: Programme qui fait un robot pouvant se faire bronzer.
*/

#define F_CPU 8192000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include "can.h"

const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t DELAY_AMBRE = 10;

void initialisation ();
void afficherAmbre();

int main() {
	
	initialisation();
	uint16_t donneeLecture = 0;
	can canObjet;
	
	while (true) { // boucle sans fin	
		donneeLecture = canObjet.lecture(PINA);
		donneeLecture = donneeLecture & 0b0000001111111111;
		if (donneeLecture < 100) {
			PORTB = VERT; //vert, lumiere basse
		}
		else if (donneeLecture < 200) {
			afficherAmbre(); // ambre, lumiere a bon niveau
		} 
		else if (donneeLecture >= 200) {
			PORTB = ROUGE; //rouge, lumiere forte
		}  
	}
	return 0;
}

void initialisation ( ) {
	DDRD = 0x00; // PORT D est en mode entree
	DDRA = 0x00; // PORT A est en mode entree
	DDRB = 0xFF; // PORT B est en mode sortie
}

void afficherAmbre() {
	PORTB = ROUGE;
	_delay_ms(DELAY_AMBRE); 
	PORTB = VERT; 
	_delay_ms(DELAY_AMBRE);
}
