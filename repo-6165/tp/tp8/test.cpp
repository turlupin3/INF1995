#include <./library/ambre.h>

int main()
{
	DDRA = 0xff;
	while(true){
		ambre();
	}
	
}
