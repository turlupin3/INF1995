

#include <./library/ini.h>
#include <./library/memoire_24.h>
#include <./library/UART.h>

int main(){
	
	demarrage();
	
	Memoire24CXXX memoire = Memoire24CXXX();
	
	initialisationUART();
	
	uint8_t longueur = 0;
	
	while(UCSR0A & (1<<TXC0)){
		
		memoire.ecriture(++longueur, receptionUART());
	}
	
	unsigned char byteCode[longueur+1];
	
	for(uint16_t i = 0; i < longueur; i++){
		
		memoire.lecture(i, byteCode, longueur);
	}
	
	return 0;
}
