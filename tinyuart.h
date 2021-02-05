/*
 * tinyuart.h
 *
 * Created: 16.12.2020 16:34:18
 *  Author: Dennis (nqtronix@protonmail.com)
 */ 


#ifndef TINYUART_H_
#define TINYUART_H_


#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////
// User Config
//////////////////////////////////////////////////////////////////////////

// specify real frequency of the CPU. If communication is unsuccessful, you can tweak this value.
// Beware, you CAN NOT use floating point calculations here!
#define TINYUART_F_CPU				9600000

// select a baud rate
#define TINYUART_BAUD				115200

// specify pins used
#define TINYUART_PORT				PORTB
#define TINYUART_PIN				PINB
#define TINYUART_DDR				DDRB
#define TINYUART_IO_TX				3


// Options. Comment line to disable.

// if defined, the preprocessor will insert up to 2 NOPs to archive cycle-accurate bit timings
// the optional tolerance calculations take this into account
#define TINYUART_OPT_HIGH_ACCURACY

// if defined, the the compiler will trow an error (or message) if the actual baud rate differs more
// than the absolute maximum tolerance (or recommended tolerance) from set baud rate. 
#define TINYUART_OPT_TOLERANCE_ERR
#define TINYUART_OPT_TOLERANCE_MSG

// if defined additional debug features are enabled. ctrl + F "TINYUART_USE_DBG" for details
//#define TINYUART_OPT_DBG


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

// General
// The software UART requires precise timing and therefore must not be interrupted by ISRs. If your
// application requires ISRs, you must disable them before calling any function below.

// sends a single byte of data, including zero
void tinyuart_send_uint8(uint8_t data);


#endif /* TINYUART_H_ */