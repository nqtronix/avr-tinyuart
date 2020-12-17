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
	
		
	//tinyuart_send_uint8(0xff);
// 	TINYUART_PIN	|= (1 << TINYUART_IO_TX);
//  _delay_us(40);
// 	TINYUART_PIN	|= (1 << TINYUART_IO_TX);
	 
	//tinyuart_send_uint8(0b01010101);
 	_delay_us(10);

	for (uint16_t n = 0; n<256; n++)
	{
		tinyuart_send_uint8((uint8_t)(255-n));
		_delay_us(10);
	}
	
	while(1);
}