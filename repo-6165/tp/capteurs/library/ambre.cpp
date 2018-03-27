
#include <ambre.h>

void ambre(){
	PORTD = VERT;
	_delay_ms(10);
	PORTD = ROUGE;
	_delay_ms(10);
}

