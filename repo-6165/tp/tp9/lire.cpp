#include "memoire_24.h"
#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

const uint16_t ADRESSE = 0x00; 


int main() {

	
	uint8_t numberInstruction = 0;
	Memoire24CXXX memoire = Memoire24CXXX();
	memoire.init();
	
		
	uint8_t octet1 = 0x0;
	uint8_t octet2 = 0x0;	
	uint16_t longeur = 0x0;
	memoire.lecture(0x0, &octet1);
	memoire.lecture(0x1, &octet2);
	
	longeur = (octet1 << 8) + octet2;
	unsigned char donnesSortie [longeur+1];
	memoire.lecture(0x2, donnesSortie,longeur);	

	
}
