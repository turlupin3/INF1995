#include <ini.h>
#include <memoire_24.h>
#include <UART.h>
#include <pwmoteur.h>
#include <note.h>
#include <manips.h>
#include <ambre.h>
//#include <interruptBouton.h>

//~ volatile uint8_t boutonPoussoir = 1;	
//~ unsigned char capteurGauche = 0;
//~ unsigned char capteurDroit = 0;	
volatile uint8_t boutonPoussoir = 0x01;
volatile bool boole = 0x01;
void envoieInfo(Memoire24CXXX memoire, uint8_t *tableau);
ISR(INT0_vect);
bool antiRebond();
void initialisation();
uint8_t getBouton();
int main() {
	demarrage();
	Memoire24CXXX memoire = Memoire24CXXX();
	initialisationUART();
	initialisation();


	uint8_t tableauEcriture[39];
	
	tableauEcriture[0] = 0xf0;
	char nomRobot[13] = "J Rom Bot   ";
	for (int i = 0; i < 9; i++) {
		tableauEcriture[i+1] = nomRobot[i];
	}
	
	tableauEcriture[14] = 0xf1;
	char numeroEquipe[9] = "6165    ";
	for (int i = 0; i < 9; i++) {
		tableauEcriture[i+15] = numeroEquipe[i];
	}

	tableauEcriture[24] = 0xf2;
	tableauEcriture[25] = 3;
	tableauEcriture[26] = 0xf3;
	char session[4] = {'1','8','-','1'};
	for (int i = 0; i < 4; i++) {
		tableauEcriture[i+27] = session[i];
	}
	tableauEcriture[31] = 0xf4;
	tableauEcriture[32] = 1;
	tableauEcriture[33] = 0xf5;
	tableauEcriture[34] = getBouton();
	tableauEcriture[35] = 0xf6;
	tableauEcriture[36] = 0;
	tableauEcriture[37] = 0xf7;
	tableauEcriture[38] = 0;
	
	
	memoire.ecriture(0, tableauEcriture, 39);

		uint8_t info;
		for (int i = 0; i < 39; i++){
			info = tableauEcriture[i];
			transmissionUART(info);
		}
	
	struct operation{
		unsigned char instruction; 
		unsigned char operande;
		};
		
	operation op;	 
for(;;){ 
		
			
		op.instruction= receptionUART();
		op.operande = receptionUART();
	//	envoieInfo(memoire, tableauEcriture);// it doest not excute
		switch (op.instruction) {
			case 0xf8: //vitesse de roue gauche
				avancerMoteurD(op.operande);
				break;
			case 0xf9: //vitesse de rouge droite
				avancerMoteurG(op.operande);
				break;
			case 0xfa: // couleur de la del libre
				delSwitcher(op.operande);
				break;
			case 0xfb: // requete denvoi des information didentification
				envoieInfo(memoire, tableauEcriture);
				break;
			default:
				//error
				break;
		}
		
	}	
	
	
	return 0;
}
	void envoieInfo(Memoire24CXXX memoire, uint8_t *tableau) {
		jouerNote(45);
		_delay_ms(500);
		arreterJouer();
		
		unsigned char capteurGauche = 0;
		unsigned char capteurDroit = 0;	
		uint8_t info;
		for (int i = 0; i < 39; i++){
			
			tableau[33] = 0xf5;
			tableau[34] = getBouton();
			//tableau[34] = 0x00;//enfoncer
			//tableau[34] = 0x01;//relacher
			tableau[35] = 0xf6;
			tableau[36] = capteurGauche;/// IS TEST
			tableau[37] = 0xf7;
			tableau[38] = capteurDroit;/// IS TEST
			
			info = tableau[i];
			transmissionUART(info);
		}
		
	}
////////////////////////////////////////////////////////////////////////
bool antiRebond(){
	DDRD = 0x00;
	bool estEnfonce = false;
	if(PIND & 0x04){
		_delay_ms(10.0);
		if(PIND & 0x04){
		estEnfonce = true;;
		}
	}
	return estEnfonce;
}


void initialisation(){
	cli();
	DDRA = 0xff;
	DDRD = 0x00;
	
	
	EIMSK |= (1 << INT0);
	EICRA |= (0 << ISC01) | (1 << ISC00); // set external interupt on pin INT0 with option any logical change generate an interupt request

	sei();
}

ISR(INT0_vect){
	//rising and failling edge
	_delay_ms(25);
	boole = !boole;
	transmissionUART(0xf5);
	transmissionUART(boole);
		
	EIFR |= (1 << INTF0);
}

uint8_t getBouton(){
	
	return boutonPoussoir;
}


