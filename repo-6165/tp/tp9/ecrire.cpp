#include "memoire_24.h"
#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>

const uint16_t ADRESSE = 0x00; 


int ecrireDonnees(unsigned char * donneesEntrees,uint8_t longueur) {

	
	Memoire24CXXX memoire = Memoire24CXXX();
	memoire.init();
	
	memoire.ecriture(0x00,donneesEntrees,longueur);
	
}
