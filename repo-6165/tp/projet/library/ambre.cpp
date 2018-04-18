
#include <ambre.h>

void ambre(){
	delOff();
	PORTD |= VERTDEL;
	_delay_ms(1.0);
	delOff();
	PORTD |= ROUGEDEL;
	_delay_ms(1.0);
}
void delVert(){
	delOff();
	PORTD |= VERTDEL;
}
void delRouge(){
	delOff();
	PORTD |= ROUGEDEL;
}
void delOff(){
	PORTD &= ~(1<<5);
	PORTD &= ~(1<<7);
}
void delSwitcher(unsigned char order){
	switch (order) {
		case 0:
		delOff();
		break;
		case 1:
		delRouge();
		break;
		case 2:
		delVert();
		break;
		default:
		ambre();
	}
}

