
#include <ambre.h>

void ambre(){
	PORTA = VERT;
	_delay_ms(1.0);
	PORTA = ROUGE;
	_delay_ms(1.0);
}
void delVert(){
	PORTA = VERT;
}
void delRouge(){
	PORTA = ROUGE;
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

