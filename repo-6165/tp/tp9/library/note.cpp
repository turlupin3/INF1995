#include <note.h>

void setupPWMNote(){
	DDRD = 0xff;
	TCNT1 = 0x0;
	
	TCCR1A |= (1 << WGM10);	//mode PWM top ocr1a
	TCCR1A &= ~(1 << WGM11);
	TCCR1B &= ~(1 << WGM12);
	TCCR1B |= (1 << WGM13);

	TCCR1B |= (1 << CS10); //set prescaler to 1
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
	
	TCCR1A &= ~(1 << COM1A1); //mode de compare
	TCCR1A &= ~(1 << COM1A0);
	TCCR1A |= (1 << COM1B1);
	TCCR1A &= ~(1 << COM1B0);
	
	OCR1B = 0;
	OCR1A = 0;
	
	//~ TIMSK1 |= (1<< OCIE1A);
	TIMSK1 |= (1<< OCIE1B);

}

uint16_t calculOCR1A(double frequence){
	return F_CPU/2/frequence;
}
void arreterJouer(){
	TCCR1B &= ~(1 << CS10); //set prescaler to 0 / stop counting
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
}

void jouerNote(uint8_t note){
	
	setupPWMNote();
	
	switch(note){
		case 45:
			OCR1A = calculOCR1A(110);
			break;
		case 46:
			OCR1A = calculOCR1A(116.5409404);
			break;
		case 47:
			OCR1A = calculOCR1A(123.4708253);
			break;
		case 48:
			OCR1A = calculOCR1A(130.8127827);
			break;
		case 49:
			OCR1A = calculOCR1A(138.5913155);
			break;
		case 50:
			OCR1A = calculOCR1A(146.832384);
			break;
		case 51:
			OCR1A = calculOCR1A(155.5634919);
			break;
		case 52:
			OCR1A = calculOCR1A(164.8137785);
			break;
		case 53:
			OCR1A = calculOCR1A(174.6141157);
			break;
		case 54:
			OCR1A = calculOCR1A(184.9972114);
			break;
		case 55:
			OCR1A = calculOCR1A(195.997718);
			break;
		case 56:
			OCR1A = calculOCR1A(207.6523488);
			break;
		case 57:
			OCR1A = calculOCR1A(220);
			break;
		case 58:
			OCR1A = calculOCR1A(233.0818808);
			break;
		case 59:
			OCR1A = calculOCR1A(246.9416506);
			break;
		case 60:
			OCR1A = calculOCR1A(261.6255653);
			break;
		case 61:
			OCR1A = calculOCR1A(277.182631);
			break;
		case 62:
			OCR1A = calculOCR1A(293.6647679);
			break;
		case 63:
			OCR1A = calculOCR1A(311.1269837);
			break;
		case 64:
			OCR1A = calculOCR1A(329.6275569);
			break;
		case 65:
			OCR1A = calculOCR1A(349.2282314);
			break;
		case 66:
			OCR1A = calculOCR1A(369.9944227);
			break;
		case 67:
			OCR1A = calculOCR1A(391.995436);
			break;
		case 68:
			OCR1A = calculOCR1A(415.3046976);
			break;
		case 69:
			OCR1A = calculOCR1A(440);
			break;
		case 70:
			OCR1A = calculOCR1A(466.1637615);
			break;
		case 71:
			OCR1A = calculOCR1A(493.8833013);
			break;
		case 72:
			OCR1A = calculOCR1A(523.2511306);
			break;
		case 73:
			OCR1A = calculOCR1A(554.365262);
			break;
		case 74:
			OCR1A = calculOCR1A(587.3295358);
			break;
		case 75:
			OCR1A = calculOCR1A(622.2539674);
			break;
		case 76:
			OCR1A = calculOCR1A(659.2551138);
			break;
		case 77:
			OCR1A = calculOCR1A(698.4564629);
			break;
		case 78:
			OCR1A = calculOCR1A(739.9888454);
			break;
		case 79:
			OCR1A = calculOCR1A(783.990872);
			break;
		case 80:
			OCR1A = calculOCR1A(830.6093952);
			break;
		case 81:
			OCR1A = calculOCR1A(880);
			break;
				
	}
	OCR1B = OCR1A/2;
}
