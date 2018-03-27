#include <manips.h>

void debut(bool & boolDebut){
	//debut
	boolDebut = true;
}

void fin(bool & boolFin) {
	//fin
	boolFin = false;
}



uint16_t dbc(unsigned char op) {
	//debut de boucle
	return op;
}
uint16_t fbc(unsigned char & counter,uint16_t & debutBoucle, uint16_t & nextInstruction) {
	//fin de boucle
	if (counter > 0) { //maybe use counter+1
		counter--;
		return debutBoucle; //maybe no ++
	}
	return nextInstruction;
}

void att(unsigned char op){
	for(; op > 0; op --){
	_delay_ms(25);
	}
}

void dal(unsigned char op){
	if (op != 0) {
		//allumer del
		DDRA=0xff;
		PORTA = 2;
	}
	//not necessary
	if (op == 2) {
		DDRA=0xff;
		PORTA = 2;
	}
}

void det(unsigned char op) {
	if (op != 0) {
	//eteindre les DEL
	PORTA = 0x00;
	}
}
