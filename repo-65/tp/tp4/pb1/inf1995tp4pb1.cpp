/*
 * Nom: Problème 1
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 23 janvier 2018
 * Description: 
 * 
 * 
 */
#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
 
void delayMs(int delay);

const uint32_t OFF = 0b00000000;
const uint32_t ROUGE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t DELAY_ANTIREBOND  = 25;
const uint32_t DELAY_SECONDE  = 1000;
const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;

int main()
{
	DDRB = PORT_SORTIE; // PORT B est en mode sortie
	PORTB = ROUGE;
	uint32_t b = 10;
	uint32_t a = 0;
	uint32_t rapportAB = 100;
	uint32_t counter = 0;
		
	for (int i = 0; i < 300; i++)	{
		a = rapportAB * b/100;
		PORTB = ROUGE;
		delayMs(a);
		PORTB = OFF;
		delayMs(b-a);
		counter++;
		if (counter == 3) {
			counter = 0;
			rapportAB= rapportAB - 1;
		}
	}		
  return 0; 
}

void delayMs(int delay) {
	for (int i = 0; i < delay; i++) {
		_delay_ms(1);
	}	
}
	
	 


