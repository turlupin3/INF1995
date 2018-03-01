/*
 * Nom: Problème 1
 * Fait par Bei Ning Pan et Virasone Manibod
 * Créé le 23 janvier 2018
 * Description: 
 * 
 * 
 */
#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay_basic.h>
#include <util/delay.h>
 
void delayMs(int delay);
void delayUs(int delay);
//void delayUs(short delay);
void changeDirection(uint32_t output);
void actionnerPWM(uint32_t PWM, uint32_t tempMS, uint32_t frequence);
void actionnerPWM(float PWM, uint32_t tempMS, float frequence, int v2);
 
const uint32_t OFF = 0b00000000;
const uint32_t ACTIVE_ROUE  = 0b00000001;
const uint32_t CHANGE_DIRECTION  = 0b00000010;
const uint32_t DELAY_ANTIREBOND  = 25;
const uint32_t DELAY_SECONDE  = 1000;
const uint32_t PORT_SORTIE  = 0xff;
const uint32_t PORT_ENTREE  = 0x00;

int main()
{
	DDRD = PORT_SORTIE; // PORT D est en mode sortie

	

	actionnerPWM(0,2000,60,1);

/*	actionnerPWM(0, 2000, 60);
	actionnerPWM(25, 2000, 60);
	actionnerPWM(100, 2000, 60);
	actionnerPWM(100, 2000, 60);
	actionnerPWM(0, 2000, 60);
	actionnerPWM(100, 2000, 60);
	actionnerPWM(100, 2000, 60);
	actionnerPWM(100, 2000, 60);


*/
/*	for (int i = 0; i < 300; i++)	{
		a = rapportAB * b/100;
		PORTB = ROUGE;
		delayMs(a);
		PORTB = OFF;
		delayMs(b-a);
		counter++;
		if (counter == 3) {
			counter = 0;
			rapportAB= rapportAB - 1;
		}
	}		
*/			



  return 0; 
}

void delayMs(int delay) {
	for (int i = 0; i < delay; i++) {
		_delay_ms(1);
	}	
}


void delayUs(int delay) {
	for (int i = 0; i < delay; i++) {
		_delay_us(1);
	}	
}



void changeDirection(uint32_t output) {
	
}

void actionnerPWM(uint32_t PWM, uint32_t tempMS, uint32_t frequence) {
	uint32_t tempUs = tempMS * 1000;
	uint32_t frequenceUs = frequence / 1000000;
	uint32_t periodeUs = 1/frequenceUs;
	uint32_t iterationUs = tempUs/periodeUs;
	
	long int a = 0;
	uint32_t iteration = tempMS*frequence/1000;  //tempMS/periode=i 1/p = f
	
	a = PWM/100 * periodeUs;
	
	for (int i = 0; i < iterationUs; i++) {
		PORTD = ACTIVE_ROUE;
		delayUs(a);
		PORTD = OFF;
		delayUs((periodeUs-a));
	}
	
	 }

void actionnerPWM(float PWM, uint32_t tempMS, float frequence, int v2) {
	//uint32_t tempUs = tempMs * 1000;
	//uint32_t frequenceUs = frenquence * 1000000
	
	//PORTD = ACTIVE_ROUE;
	//delayMs(2000);
	//PORTD = OFF;
	
	float a = 0;
	float periode = (1.0/frequence)*1000.0;
	uint32_t iteration = tempMS*frequence/1000;  //tempMS/periode=i 1/p = f
	
	a = PWM * periode/100.0;
	
	for (uint32_t i = 0; i < iteration; i++) {
		PORTD = ACTIVE_ROUE;
		delayUs(static_cast<int>(a*1000));
		PORTD = OFF;
		delayUs(static_cast<int>((periode-a)*1000));
	}
	
	 } 
	
	 
// 16.67ms 120 = i, t = 2000ms

