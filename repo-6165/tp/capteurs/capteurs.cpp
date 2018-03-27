#define F_CPU 8000000

#include <./library/ambre.h>
#include <./library/can.h>

//~ const uint32_t OFF = 0b00000000;
//~ const uint32_t ROUGE  = 0b00000001;
//~ const uint32_t VERT  = 0b00000010;
//~ const uint32_t DELAY_AMBRE = 10;

void initialisation ();
//void afficherAmbre();

int main() {
	
	initialisation();
	uint16_t donneeLecture = 0;
	can canObjet;
	
	while (true) {
		donneeLecture = canObjet.lecture(PINA);
		donneeLecture = donneeLecture & 0b0000001111111111;
		if (donneeLecture < 50) {
			PORTD = VERT;
		}
		else if (donneeLecture > 100) {
			PORTD = ROUGE; 
		} 
		else if (donneeLecture <= 100) {
			ambre();
		}  
	}
	return 0;
}

void initialisation ( ) {
	DDRD = 0xFF; // PORT D est en mode sortie
	DDRA = 0x00; // PORT A est en mode entree
	//DDRB = 0xFF; // PORT B est en mode sortie
}

//~ void afficherAmbre() {
	//~ PORTB = ROUGE;
	//~ _delay_ms(DELAY_AMBRE); 
	//~ PORTB = VERT; 
	//~ _delay_ms(DELAY_AMBRE);
//~ }
