/*
 * main.c
 *
 * Created: 16.12.2020 16:34:18
 *  Author: Dennis (nqtronix@protonmail.com)
 */ 

#define F_CPU 9600000

#include <avr/io.h>
#include "tinyuart/tinyuart.h"
#include "util/delay.h"


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

int main(void)
{
	// initializes IO
	tinyuart_init();
	
	_delay_us(40);
	// toggle pattern test: after every bit delay the TX pin is toggled
// 	tinyuart_send_uint8(0b01010101);
//  	_delay_us(10);

	// every character test: verify that your receiver can read all chars at the chosen baud rate
	// cheap UART-USB converts often have an issue with 0x00
	for (uint16_t n = 0; n<256; n++)
	{
		tinyuart_send_uint8((uint8_t)(255-n));
		_delay_us(10);
	}
	
	while(1)
	{
		tinyuart_send_uint8(0);
		_delay_us(1000);
	}
}