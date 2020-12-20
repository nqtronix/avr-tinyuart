/*
 * main.c
 *
 * Created: 16.12.2020 16:34:18
 *  Author: Dennis (nqtronix@protonmail.com)
 */ 

#define F_CPU 9600000

#include <avr/io.h>
#include "tinyuart.h"
#include "util/delay.h"


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

// Demo code to check if selected baud rate works on device
int main(void)
{
	// disable the default div8 prescaler
	CLKPR = 1<<CLKPCE;
	CLKPR = 0b0000<<CLKPS0;
	// interrupts must be disabled while using the send function
	// cli();

	// toggle pattern test: after every bit delay the TX pin is toggled
	tinyuart_send_uint8(0b01010101);	// = 0x55
 	_delay_us(10);

	// every character test: verify that your receiver can read all chars at the chosen baud rate
	// cheap UART-USB converts often have an issue with 0x00
	for (uint16_t n = 0; n<256; n++)
	{
		tinyuart_send_uint8((uint8_t)(n));
		_delay_us(10);
	}
	
	// Test 0x00 to catch any glitches
	for (uint8_t n = 0; n<64; n++)
	{
		tinyuart_send_uint8(0);
		_delay_us(1000);
	}
	
	// re-enable interrupts
	// sei();
	
	while(1);
}