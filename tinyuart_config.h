/*
 * tinyuart_config.h
 *
 * Created: 21.03.2021 15:25:07
 *  Author: Dennis (keybase.io/nqtronix)
 */ 


#ifndef TINYUART_CONFIG_H_
#define TINYUART_CONFIG_H_


//////////////////////////////////////////////////////////////////////////
// User Config
//////////////////////////////////////////////////////////////////////////

// specify real frequency of the CPU. If communication is unsuccessful, you can tweak this value.
// Beware, you CAN NOT use floating point calculations here!
#define TINYUART_F_CPU				9600000

// define a baud rate
#define TINYUART_BAUD				115200

// define IO
#define TINYUART_IO_TX				B,3


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


#endif /* TINYUART_CONFIG_H_ */