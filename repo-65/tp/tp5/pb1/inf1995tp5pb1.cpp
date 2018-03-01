
void initialisation ();
//ISR();
void afficherAmbre();
#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> //: Interrupts

enum  {
	      ini = 0, state1, state2, state3, state4, state5
} volatile state; 

const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;
const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t DELAY_ANTIREBOND  = 25;
const uint32_t DELAY_ANTIREBOND_COURT  = 10;
const uint32_t DELAY_SECONDE  = 1000;

//volatile bool pushed = false; 

int main() {
	
	initialisation();
	while (true) {
		switch (state) {
			case ini:
				PORTB = ROUGE;			
				break;
			case state1:
				//while ((pushed == true)) {	
					//pushed = false;
					afficherAmbre();
					
				//}
				//state = state2;			
				break;
			case state2:
				PORTB = VERT;		

				break;
			case state3:
				//while ((pushed == true)) {
					PORTB = ROUGE;
					//pushed = false;
				//}
				//state = state4;
				break;
			case state4:
				PORTB = OFF;
				break;
			case state5:
				//while (pushed == true) {
					PORTB = VERT;
					//pushed = false;
				//}
				//state = ini;	
				break;
		}
		//pushed = false;
	}
	
	  return 0; 
}

void initialisation ( ) {

// cli est une routine qui bloque toutes les interruptions.
// Il serait bien mauvais d'être interrompu alors que
// le microcontroleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées
// et les sorties. DDRx... Initialisez bien vos variables

	state = ini;
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	DDRD = PORT_ENTREE; // PORT D est en entree


// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001

// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir
// en ajustant le registre EICRA

EICRA |= (1 << ISC00);
//EICRA |= (1 << ISC01);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}

 ISR ( INT0_vect ) {

// laisser un delai avant de confirmer la réponse du
// bouton-poussoir: environ 30 ms (anti-rebond)

_delay_ms(30);

// se souvenir ici si le bouton est pressé ou relâché

///'modifier ici'
//pushed = true;

// changements d'états tels que ceux de la
// semaine précédente

///'modifier ici'
		if ( state == ini) {  
					state = state1;	}
		else if ( state == state1) {  
					state = state2;
					
				}
		else if ( state == state2) {  
					state = state3;
				}
		else if ( state == state3 ) {  
			state = state4;
		}
		else if ( state == state4 ) {  
					state = state5;
				}
		else if ( state == state5 ) {  
					state = ini;
				}
		

// Voir la note plus bas pour comprendre cette instruction et son rôle

EIFR |= (1 << INTF0) ;

}

void afficherAmbre() {
	PORTB = ROUGE;
	_delay_ms(DELAY_ANTIREBOND_COURT); 
	PORTB = VERT; 
	_delay_ms(DELAY_ANTIREBOND_COURT); 
}
