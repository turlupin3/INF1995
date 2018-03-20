#define F_CPU 8000000


#include <avr/io.h> 
#include <avr/interrupt.h> //: Interrupts
#include <util/delay_basic.h>
#include <util/delay.h>
#include <./library/pwm.h>


const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;
const uint32_t OFF = 0b00000000;
const uint32_t ACTIVE_ROUE  = 0b00000001;
const uint32_t VERT  = 0b00000010;
const uint32_t FreCPU = F_CPU/1024 

void initialisation ();
void jouerMusic(uint32_t note, uint32_t temps_Ms);

int main(){
	initialisation();
	PORTD = OFF;
	
	jouerMusic(49, 1000);
	jouerMusic(58, 1000);
	jouerMusic(49, 1000);
	jouerMusic(65, 1000);
	jouerMusic(80, 1000);
	

	

	
	return 0;
}

void initialisation ( ) {

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

void jouerMusic(uint32_t note, uint32_t temps_Ms){
	
	uint32_t valeurPWM = 0;
	
	switch(note){
		case 45:
			valeurPWM = FreCPU * 4.54545454545455
			break;
		case 46:
			valeurPWM = FreCPU * 4.29033778491762
			break;
		case 47:
			valeurPWM = FreCPU * 4.04953962791165
			break;
		case 48:
			valeurPWM = FreCPU * 3.82225643297141
			break;
		case 49:
			valeurPWM = FreCPU * 3.60772966356503
			break;
		case 50:
			valeurPWM = FreCPU * 3.405243356538
			break;
		case 51:
			valeurPWM = FreCPU * 3.21412173266696
			break;
		case 52:
			valeurPWM = FreCPU * 3.03372694129618
			break;
		case 53:
			valeurPWM = FreCPU * 2.86345693157929
			break;
		case 54:
			valeurPWM = FreCPU * 2.70274344318825
			break;
		case 55:
			valeurPWM = FreCPU * 2.5510501097937
			break;
		case 56:
			valeurPWM = FreCPU * 2.40787066899808
			break;
		case 57:
			valeurPWM = FreCPU * 2.27272727272727
			break;
		case 58:
			valeurPWM = FreCPU * 2.14516889245881
			break;
		case 59:
			valeurPWM = FreCPU * 2.02476981395501
			break;
		case 60:
			valeurPWM = FreCPU * 1.91112821648571
			break;
		case 61:
			valeurPWM = FreCPU * 1.80386483178186
			break;
		case 62:
			valeurPWM = FreCPU * 1.702621678269
			break;
		case 63:
			valeurPWM = FreCPU * 1.60706086633296
			break;
		case 64:
			valeurPWM = FreCPU * 1.51686347064763
			break;
		case 65:
			valeurPWM = FreCPU * 1.43172846578964
			break;
		case 66:
			valeurPWM = FreCPU * 1.35137172159413
			break;
		case 67:
			valeurPWM = FreCPU * 1.27552505489718
			break;
		case 68:
			valeurPWM = FreCPU * 1.20393533449933
			break;
		case 69:
			valeurPWM = FreCPU * 1.13636363636364
			break;
		case 70:
			valeurPWM = FreCPU * 1.07258444622917
			break;
		case 71:
			valeurPWM = FreCPU * 1.01238490697771
			break;
		case 72:
			valeurPWM = FreCPU * 0.955564108242853
			break;
		case 73:
			valeurPWM = FreCPU * 0.901932415891094
			break;
		case 74:
			valeurPWM = FreCPU * 0.8513108391345
			break;
		case 75:
			valeurPWM = FreCPU * 0.803530433166482
			break;
		case 76:
			valeurPWM = FreCPU * 0.758431735323929
			break;
		case 77:
			valeurPWM = FreCPU * 0.715864232894822
			break;
		case 78:
			valeurPWM = FreCPU * 0.675685860796972
			break;
		case 79:
			valeurPWM = FreCPU * 0.637762527448506
			break;
		case 80:
			valeurPWM = FreCPU * 0.601967667249592
			break;
		case 81:
			valeurPWM = FreCPU * 0.568181818181818
			break;
	}
	ajustementPWM(valeurPWM);
	_delay_ms(temps_Ms);		
	
}

