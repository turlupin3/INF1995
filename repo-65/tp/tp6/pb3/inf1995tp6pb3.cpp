


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

void initialisationUART();
void transmissionUART ( uint8_t donnee );


int main() {
	Memoire24CXXX memoire = Memoire24CXXX();
	unsigned char donnesSortie [LONGUEUR+1];
    unsigned char donnes[LONGUEUR+1] ="*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*";
    memoire.init();
	memoire.ecriture(ADRESSE,donnes,LONGUEUR);
	_delay_ms(15); 
	memoire.lecture(ADRESSE,donnesSortie,LONGUEUR);
	
    initialisationUART ( );
    char mots[21] = "Le robot en INF1995\n";
	uint8_t i, j;
	//for ( i = 0; i < 100; i++ ) {
		for ( j=0; j < 39; j++ ) {
			transmissionUART ( donnesSortie[j] );
		}

//}	
	return 0;
}

void initialisationUART ( ) {

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A |= (1 << TXC0);

	UCSR0B |= (1 << TXEN0);
	UCSR0B |= (1 << RXEN0);

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C |= (1 << UCSZ00);
	UCSR0C |= (1 << UCSZ01);

}
void transmissionUART ( uint8_t donnee ) {

	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = donnee;

}
