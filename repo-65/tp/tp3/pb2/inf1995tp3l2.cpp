/*
 * Nom: Problème 2
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 23 janvier 2018
 * Description: Programme qui change la couleur de la DEL après avoir appuyer ou relâcher
 * 				le bouton-poussoir.
 * 
 * 		Table des etats
 * 
 * Etat present 	A 		Etat suivant 	Sortie
 * 		ini			0			ini		 	 rouge
 * 		ini			1			state1		 ambre
 * 		state1		0			state2		 vert
 * 		state1		1			state1		 ambre
 * 		state2		0			state2		 vert
 * 		state2		1			state3		 rouge
 * 		state3		0			state4		 eteint
 * 		state3		1			state3		 rouge
 * 		state4		0			state4		 eteint
 * 		state4		1			state5		 vert
 * 		state5		0			ini			 rouge
 * 		state5		1			state5		 vert
 */
#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

void afficherAmbre();

enum  {
	      ini = 0, state1, state2, state3, state4, state5
} state; 

	const uint32_t OFF = 0b00000000;
	const uint32_t ROUGE  = 0b00000001;
	const uint32_t VERT  = 0b00000010;
	const uint32_t DELAY_ANTIREBOND  = 25;
	const uint32_t DELAY_ANTIREBOND_COURT  = 10;
	const uint32_t DELAY_SECONDE  = 1000;
	const uint32_t PORT_SORTIE  = 0xff;
	const uint32_t PORT_ENTREE  = 0x00;

int main()
{
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	DDRD = PORT_ENTREE; // PORT D est en entree
		   
	state = ini;
	   
    for(;;) { // Boucle sans fin
		switch (state) {
			case ini:
				PORTB = ROUGE;			
				_delay_ms(DELAY_ANTIREBOND);
				if ( PIND & 0x04 ) {  
					state = state1;	
				}
				break;
			case state1:
				while ((PIND &0x04)) {	
					afficherAmbre();
				}
				state = state2;			
				break;
			case state2:
				PORTB = VERT;		
				_delay_ms(DELAY_ANTIREBOND);
				if ( PIND & 0x04 ) {  
					state = state3;
				}
				break;
			case state3:
				while ((PIND &0x04)) {
					PORTB = ROUGE;
				}
				state = state4;
				break;
			case state4:
				PORTB = OFF;
				_delay_ms(DELAY_ANTIREBOND);
				if ( PIND & 0x04 ) {  
					state = state5;
				}
				break;
			case state5:
				while ((PIND &0x04)) {
					PORTB = VERT;
				}
				state = ini;	
				break;
		}	
	}
  return 0; 
}

void afficherAmbre() {
	PORTB = ROUGE;
	_delay_ms(DELAY_ANTIREBOND_COURT); 
	PORTB = VERT; 
	_delay_ms(DELAY_ANTIREBOND_COURT); 
}


