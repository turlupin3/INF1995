/*
 * Nom: Problème 1
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 23 janvier 2018
 * Description: Programme qui allume une DEL après avoir appuyer et relâcher
 * 				5 fois le bouton-poussoir.
 * 
 * 					Table des etats
 * 
 * Etat present 	A 		Etat suivant 	Sortie
 * 		ini			0			  ini		   0
 * 		ini			1			state1		   0
 * 		state1		0			state1		   0
 * 		state1		1			state2		   0
 * 		state2		0			state2		   0
 * 		state2		1			state3		   0
 * 		state3		0			state3		   0
 * 		state3		1			state4		   0
 * 		state4		0			state4		   0
 * 		state4		1			  ini		   1
 * 
 */
#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

enum  {
		  ini = 0, state1, state2, state3, state4
} state; 

	const uint32_t OFF = 0b00000000;
	const uint32_t ON  = 0b00000001;
	const uint32_t DELAY_ANTIREBOND  = 25;
	const uint32_t DELAY_SECONDE  = 1000;
	const uint32_t PORT_SORTIE  = 0xff;
	const uint32_t PORT_ENTREE  = 0x00;

int main()
{
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	DDRD = PORT_ENTREE; // PORT D est en entree
	   
	state = ini;
	   
	for(;;) { // Boucle sans fin
		_delay_ms(DELAY_ANTIREBOND);
		if ( PIND & 0x04 ) {  
			while ((PIND &0x04)) { // Le programme ne continuera pas tant que le bouton
			}					   // n'est pas relache
			switch (state) {
				case ini:
					PORTB = OFF;
					state = state1;
					break;
				case state1:
					state = state2;			
					break;
				case state2:
					state = state3;	
					break;
				case state3:
					state = state4;	
					break;
				case state4:
					state = ini;
					PORTB = ON;
					_delay_ms(DELAY_SECONDE);
					PORTB = OFF;	
					break;
			}
		}
  }
  return 0; 
}


