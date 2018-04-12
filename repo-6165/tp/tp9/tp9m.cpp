

#include <./library/ini.h>
#include <./library/memoire_24.h>
#include <./library/UART.h>
#include <./library/pwmoteur.h>
#include <./library/note.h>
#include <./library/manips.h>





int main(){
	DDRD=0xff;
	DDRC=0xff;
	jouerNote(60);
	
	return 0;
}
