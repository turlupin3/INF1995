


#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h> //: Interrupts
#include "memoire_24.h"

const uint16_t ADRESSE = 0x00; 
const uint8_t LONGUEUR = 39;

const uint32_t PORT_SORTIE  = 0xff;
const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;


int main() {
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	
	///sequence demarage
	PORTB =VERT;
			_delay_ms(2500); 
			PORTB = OFF;
	///
	
    unsigned char donnesSortie [LONGUEUR+1];
    unsigned char donnes[LONGUEUR+1] ="*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*";
	Memoire24CXXX memoire = Memoire24CXXX();
	memoire.init();
	memoire.ecriture(ADRESSE,donnes,LONGUEUR);
	_delay_ms(15); 
	memoire.lecture(ADRESSE,donnesSortie,LONGUEUR);
	
	for (uint8_t i = 0; i < LONGUEUR; i++){
		
		
		if (donnes[i] == donnesSortie[i] ) {
			
			PORTB =VERT;
			_delay_ms(250); 
			PORTB = OFF;
			_delay_ms(250); 
		}
		else {
			PORTB =ROUGE;
			_delay_ms(250); 
			PORTB = OFF;
			_delay_ms(250); 
		}
		
		
	}
	return 0;
}
