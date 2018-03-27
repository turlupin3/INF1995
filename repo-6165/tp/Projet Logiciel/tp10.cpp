

#include <ini.h>
#include <memoire_24.h>
#include <UART.h>
#include <pwmoteur.h>
#include <note.h>
#include <manips.h>

bool varDebut;
bool varFin;
//~ uint32_t counter;

struct operation{
	unsigned char instruction; 
	unsigned char operande;
	};


int main(){
	
	Memoire24CXXX memoire = Memoire24CXXX();
	initialisationUART();
	demarrage();
	
	uint16_t tailleByteCode = 0;
	uint8_t octet1= receptionUART();
	uint8_t octet2= receptionUART();
	tailleByteCode = (octet1 << 8) + (octet2);
	
	memoire.ecriture(0, octet1);
	memoire.ecriture(1, octet2);
	for(uint16_t i = 2; i < tailleByteCode; i++){
		memoire.ecriture(i, receptionUART());
	}
	//~ DDRA = 0xff;
	//~ for(int i =0; i < tailleByteCode; i++){
		//~ PORTA = 2;
		//~ _delay_ms(1000);
		//~ PORTA = 0;
		//~ _delay_ms(1000);
	//~ }
	//~ uint8_t octet1= 0;
	//~ uint8_t octet2= 0;
	
	//~ memoire.lecture(0, &octet1);
	//~ memoire.lecture(1, &octet2);
	//~ uint16_t tailleByteCode = (octet1 << 8) + (octet2);

	
	operation op;
	
	unsigned char compteur = 0;
	uint16_t debutBoucle = 0;

	for(uint16_t i = 2; i < tailleByteCode; i+=2){
		
		memoire.lecture(i, &op.instruction);
		memoire.lecture(i+1, &op.operande);
		if((op.instruction == 0x01 || varDebut) && !varFin ) {
		
			switch(op.instruction){
		
			case 0x01: 	
						debut(varDebut);
						break;
		
			case 0x02: 	
						att(op.operande);
						break;
			
			case  0x44: 
						dal(op.operande);
						break;
						
			case  0x45: 
						det(op.operande);
						break;
						
			case 0xFF : 
						fin(varFin);
						break;
			case 0xC0 :	debutBoucle = i;
						compteur = dbc(op.instruction);
						break;
			case 0xC1 :
						uint16_t finBoucle = i+2;
						i =	fbc(compteur, debutBoucle, finBoucle);
						break;
			case 0x48 : 
						jouerNote(op.operande);
						break;
			case 0x09 : 
						arreterJouer();
						break;
			case 0x60 :
						arreterMoteur();
						break;
			case 0x61 :
						arreterMoteur();
						break;
			case 0x62 :
						avancerMoteur(op.operande);
						break;
			case 0x63 :
						reculerMoteur(op.operande);
						break;
			case 0x64 :
						tournerDroite();
						break;
			case 0x65 :
						tournerGauche();
						break;
			}
		
		}
		
	}
	return 0;
}
