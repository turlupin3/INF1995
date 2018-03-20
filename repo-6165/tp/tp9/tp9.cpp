

#include <./library/ini.h>
#include <./library/memoire_24.h>
#include <./library/UART.h>

bool debut = false;
bool fin = false;

struct operation{
	unsigned char instruction; 
	unsigned char operande;
	};

void delay_ms(double ms){
	for(; ms >0 ; ms--){
		_delay_ms(1);
	}
}

void att(unsigned char op){

	delay_ms(25*op);
}

void dal(unsigned char op){
	DDRA=0xff;
	PORTA = 0x01;
}

void det(unsigned char op){
	DDRA=0xff;
	PORTA=0x10;
}
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
	
	operation operations[longueur];
	
	for(uint16_t i = 2; i < longueur; i+=2){
	
		operations[i-2].instruction = byteCode[i];
		operations[i-2].operande = byteCode[i+1];
	}
	
	for(int i = 0; i < longueur; i++){
		
		if((operations[i].instruction == 0x01 || debut) && !fin ) {
		
			switch(operations[i].instruction){
		
			case 0x01: debut = true;
					break;
		
			case 0x02: att(operations[i].operande);
					break;
			
			case  0x44: dal(operations[i].operande);
						break;
			case  0x45: det(operations[i].operande);
						break;
			case 0xC1 : fin = false;
						break;
		
			}
		
		}
		
	}
	return 0;
}
