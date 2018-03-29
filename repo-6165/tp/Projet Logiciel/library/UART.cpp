#include <UART.h>

//~ void initialisationUART ( ) {

	//~ // 2400 bauds. Nous vous donnons la valeur des deux

	//~ // premier registres pour vous éviter des complications

	//~ UBRR0H = 0;
	//~ UBRR1H = 0;
	
	//~ UBRR0L = 0xCF;
	//~ UBRR1L = 0xCF;
	//~ // permettre la reception par le UART0 et la transmission par le UART1

	//~ //UCSR0A |= (1 << TXC0);
	//~ UCSR0A |= (1 << RXC0);
	//~ UCSR1A |= (1 << TXC1);
	
	//~ UCSR0B |= (1 << RXEN0);		//USART 0 en reception et USART1 en transmission
	//~ UCSR1B |= (1 << TXEN1);
	

	//~ // Format des trames: 8 bits, 1 stop bits, none parity

	//~ UCSR0C |= (1 << UCSZ00);
	//~ UCSR0C |= (1 << UCSZ01);

	//~ UCSR1C |= (1 << UCSZ10);
	//~ UCSR1C |= (1 << UCSZ11);
//~ }
//~ void transmissionUART ( uint8_t donnee ) { //USART1

	//~ /* Wait for empty transmit buffer */
	//~ while ( !( UCSR1A & (1<<UDRE1)) );
	
	//~ /* Put data into buffer, sends the data */
	//~ UDR1 = donnee;
//~ }

void initialisationUART ( ) {

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A |= (1 << TXC0);

	UCSR0B |= (1 << TXEN0);
	UCSR0B |= (1 << RXEN0);

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C |= (1 << UCSZ00);
	UCSR0C |= (1 << UCSZ01);

}
void transmissionUART ( uint8_t donnee ) {

	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = donnee;

}

unsigned char receptionUART (){//USART0
	
	while( !(UCSR0A & (1<<RXC0)) ){
		//wait for data
	}
	return UDR0;
}
