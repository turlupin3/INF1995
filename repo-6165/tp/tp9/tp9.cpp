

#include <./library/ini.h>
#include <./library/memoire_24.h>
#include <./library/UART.h>
#include <./library/pwmoteur.h>
#include <./library/note.h>
#include <./library/manips.h>

bool varDebut;
bool varFin;
//~ uint32_t counter;

struct operation{
	unsigned char instruction; 
	unsigned char operande;
	};

void delay_ms(double ms){
	for(; ms >0 ; ms--){
		_delay_ms(1);
	}
}

//~ void att(unsigned char op){
	

	//~ delay_ms(25*op);
//~ }

//~ void dal(unsigned char op){
	//~ if (op != 0) {
		//~ //allumer del
		//~ DDRA=0xff;
		//~ PORTA = 2;
	//~ }
	//~ //not necessary
	//~ if (op == 2) {
		//~ DDRA=0xff;
		//~ PORTA = 2;
	//~ }
//~ }

//~ void det(unsigned char op) {
	//~ if (op != 0) {
	//~ //eteindre les DEL
	//~ PORTA = 0x00;
	//~ }
//~ }

//~ void sgo(unsigned char op) {
	//~ // jouer une sonorite (D/J'A FAIT)
	//~ jouerNote(op);
//~ }

//~ void sar(unsigned char op) {
	//~ //arreter de jouer la sonorite
	
//~ }

//~ void mar(unsigned char op) {
	//~ //arreter les moteurs
	//~ arreterMoteur();
//~ }

//~ void mav(unsigned char op) {
	//~ //avancer
	//~ DDRB = 0xff;
	//~ setUpPWMoteur();
	//~ OCR0B = op;
	//~ OCR0A = op;
	//~ PORTB |= 0b10010;
	
//~ }
//~ void mre(unsigned char op) {
	//~ //reculer
	//~ DDRB = 0xff;
	//~ setUpPWMoteur();
	//~ OCR0B = op;
	//~ OCR0A = op;
	//~ PORTB |= 0b0;
	
//~ }
//~ void trd() {
	//~ //tourner a droite
	//~ DDRB = 0xff;
	//~ setUpPWMoteur();
	//~ OCR0B = 128; // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE

	//~ arreterMoteur();
	
//~ }
//~ void trg() {
	//~ //tourner a gauche
	//~ DDRB = 0xff;
	//~ setUpPWMoteur();
	//~ OCR0A = 128; // VOIR PENDANT COMBIEN DE TEMPS + AUTRE ROUE
	//~ arreterMoteur();
//~ }
//~ uint16_t dbc(unsigned char op, uint16_t instructionCounter) {
	//~ //debut de boucle
	//~ counter = op;
	//~ return instructionCounter;
//~ }
//~ uint16_t fbc(unsigned char op, uint16_t & instructionCounter, uint16_t currentInstruction) {
	//~ //fin de boucle
	//~ if (counter > 0) { //maybe use counter+1
		//~ counter--;
		//~ return ++instructionCounter; //maybe no ++
	//~ }
	//~ return currentInstruction;
//~ }
//~ void fin(unsigned char op) {
	//~ //fin
	//~ varFin = false;
//~ }






int main(){
	
	
	demarrage();
	
	while(true){}
	
	Memoire24CXXX memoire = Memoire24CXXX();
	
	initialisationUART();
	
	uint16_t tailleByteCode = 0;
	
	uint8_t octet1= receptionUART();

	
	uint8_t octet2= receptionUART();

	
	tailleByteCode = (octet1 << 8) + (octet2);
	
	memoire.ecriture(0, octet1);
	memoire.ecriture(1, octet2);
	
	for(uint16_t i = 2; i < tailleByteCode; i++){
		memoire.ecriture(i, receptionUART());
	}
	
	_delay_ms(1000);

	unsigned char byteCode[tailleByteCode];
	
	memoire.lecture(0, byteCode, tailleByteCode);
	
	
	operation operations[tailleByteCode];
	
	for(uint16_t i = 2; i < tailleByteCode; i+=2){
	
		operations[i-2].instruction = byteCode[i];
		operations[i-2].operande = byteCode[i+1];
	}

	uint16_t instructionCounter = 0;
	for(uint16_t i = 0; i < tailleByteCode; i++){
		
		if((operations[i].instruction == 0x01 || varDebut) && !varFin ) {
		
			switch(operations[i].instruction){
		
			case 0x01: 	debut(varDebut);
						break;
		
			case 0x02: 	att(operations[i].operande);
						break;
			
			case  0x44: dal(operations[i].operande);
						break;
			case  0x45: det(operations[i].operande);
						break;
			case 0xFF : fin(varFin);
						break;
			case 0xC0 :
						instructionCounter = dbc(operations[i].operande, i);
						break;
			case 0xC1 :
						i =	fbc(operations[i].operande, instructionCounter, i);
						break;
			case 0x48 : jouerNote(operations[i].operande);
						break;
			}
			
		
		}
		
	}
	return 0;
}
