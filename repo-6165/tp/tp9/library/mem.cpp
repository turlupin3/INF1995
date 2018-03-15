#include <mem.h>

uint8_t* lecture() {

	Memoire24CXXX memoire = Memoire24CXXX(); //initialisation de la memoire
	memoire.init();
	
		
	uint8_t octet1 = 0x0;
	uint8_t octet2 = 0x0;	
	uint16_t longeur = 0x0;
	memoire.lecture(0x0, &octet1);
	memoire.lecture(0x1, &octet2);
	
	longeur = (octet1 << 8) + octet2;
	unsigned char donneesSortie[longeur+1];
	memoire.lecture(0x2, donneesSortie,longeur);	
	
	return donneesSortie;
}

void ecriture(char donneesEntrees,uint8_t longueur) {

	
	Memoire24CXXX memoire = Memoire24CXXX();
	memoire.init();
	
	memoire.ecriture(0x00,donneesEntrees,longueur);
	
}
