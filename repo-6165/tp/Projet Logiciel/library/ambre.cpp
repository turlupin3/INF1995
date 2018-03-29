
#include <ambre.h>

void ambre(){
	PORTA = VERTDEL;
	_delay_ms(1.0);
	PORTA = ROUGEDEL;
	_delay_ms(1.0);
}
void delVert(){
	PORTA = VERTDEL;
}
void delRouge(){
	PORTA = ROUGEDEL;
}
void delOff(){
	PORTA = 0;
}
void delSwitcher(unsigned char order){
	switch (order) {
		case 0:
		delOff();
		break;
		case 1:
		delVert();
		break;
		case 2:
		delRouge();
		break;
		default:
		ambre();
	}
}

