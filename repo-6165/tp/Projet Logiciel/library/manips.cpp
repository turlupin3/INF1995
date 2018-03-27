#include <manips.h>

void debut(bool & boolDebut){
	//debut
	boolDebut = true;
}

void fin(bool & boolFin) {
	//fin
	boolFin = false;
}



uint16_t dbc(unsigned char op, uint16_t instructionCounter,uint16_t & counter) {
	//debut de boucle
	counter = op;
	return instructionCounter;
}
uint16_t fbc(unsigned char op, uint16_t instructionCounter, uint16_t currentInstruction,uint16_t & counter) {
	//fin de boucle
	if (counter > 0) { //maybe use counter+1
		counter--;
		return instructionCounter +1; //maybe no ++
	}
	return currentInstruction;
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
