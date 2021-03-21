/*
 * tinyuart.h
 *
 * Created: 16.12.2020 16:34:18
 *  Author: Dennis (keybase.io/nqtronix)
 */ 


#ifndef TINYUART_H_
#define TINYUART_H_


#include <avr/io.h>


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

// General
// The software UART requires precise timing and therefore must not be interrupted by ISRs. If your
// application requires ISRs, you must disable them before calling any function below.

// sends a single byte of data, including zero
void tinyuart_send_uint8(uint8_t data);



#endif /* TINYUART_H_ */