#include <ini.h>

void demarrage(){
	
	DDRA = 0xff; // PORT A est en mode sortie
	
	PORTA = ROUGE;
	_delay_ms(500);
	PORTA = OFF;
	_delay_ms(250);
	PORTA = ROUGE;
	_delay_ms(500);
	PORTA = OFF;
	_delay_ms(250);
	
	PORTA = VERT;
	_delay_ms(500);
	PORTA = OFF;
	_delay_ms(250);
		PORTA = VERT;
	_delay_ms(500);
	PORTA = OFF;
}

void test(){
	DDRA=PORT_SORTIE;
	
	PORTA = VERT;
	_delay_ms(500);
	PORTA = OFF;
	_delay_ms(500);
}

void iniINterruptBoutton(){

// cli est une routine qui bloque toutes les interruptions.
// Il serait bien mauvais d'être interrompu alors que
// le microcontroleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées
// et les sorties. DDRx... Initialisez bien vos variables

	
DDRB = PORT_SORTIE; // PORT B est en mode sortie
DDRD = PORT_SORTIE; // PORT D est en entree


// cette procédure ajuste le registre EIMSK
// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ; // EIMSK = 0b00000001

// il faut sensibiliser les interruptions externes aux
// changements de niveau du bouton-poussoir
// en ajustant le registre EICRA

EICRA |= (1 << ISC00);
EICRA |= (1 << ISC01);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}
