/*
 * tinyuart.c
 *
 * Created: 16.12.2020 16:34:11
 *  Author: Dennis (keybase.io/nqtronix)
 */ 


#include "tinyuart.h"

#if __has_include("config/tinyuart_config.h")
	#include "config/tinyuart_config.h"
#else
	#include "tinyuart_config.h"
	#pragma message "tinyuart: build-in configuration loaded, for custom settings see note tinyuart_config.h"
#endif




//////////////////////////////////////////////////////////////////////////
// Snippets
//////////////////////////////////////////////////////////////////////////

// avr-easy-io
// 
// Specifying IO usually requires multiple definitions, one for the bit position and for each
// register of the IO group. With the macros below you can generate all register names from a single
// define in the form of:
//
// #define LIBNAME_IO_PINNAME	B,3		// LIBNAME_IO_PINNAME refers to group B, pin 3

#define _io(io)					IO_BIT(io)
#define _io_port(io)			IO_PORT(io)
#define _io_pin(io)				IO_PIN(io)
#define _io_ddr(io)				IO_DDR(io)

#define IO_BIT(reg, bit)		(bit)
#define IO_PORT(reg, bit)		(PORT ## reg)
#define IO_PIN(reg, bit)		(PIN ## reg)
#define IO_DDR(reg, bit)		(DDR ## reg)


//////////////////////////////////////////////////////////////////////////
// Compatibility
//////////////////////////////////////////////////////////////////////////

// delay loop step size
#define CYCLES_LOOP		3

// min cycles per tx bit
#define CYCLES_TX	9


// the c preprocessor does not support floating point. to get a higher accuracy for divisions the
// base value (F_CPU) is upscaled as a first step and converted back to the correct value at the end
// An upscale value of 100 works with F_CPU == 20000000; the maximum computational value is < 2^31
#define UPSCALE		100


// calculate the loop iterations required for the delay
// minimum allowed loop value is 1; value 0 is interpreted as 256
#ifdef TINYUART_OPT_HIGH_ACCURACY
	// round to nearest cycle
	#define CYCLES			(((UPSCALE*(TINYUART_F_CPU)/(TINYUART_BAUD)-UPSCALE*CYCLES_TX)+UPSCALE/2)/UPSCALE)
	#define LOOP_DELAY		(CYCLES/CYCLES_LOOP)
	#define CYCLES_EXTRA	(CYCLES-(LOOP_DELAY*CYCLES_LOOP))
	
#else
	// round to nearest 3 cycles
	#define LOOP_DELAY		((((UPSCALE*(TINYUART_F_CPU)/(TINYUART_BAUD)-UPSCALE*CYCLES_TX)/CYCLES_LOOP)+UPSCALE/2)/UPSCALE)
	#define CYCLES_EXTRA	0
#endif


#if LOOP_DELAY > 256
	#error "Selected baud rate is too low: bit delay value does not fit into 8 bit register."
#endif


// convert additional delay into NOPs
#if   CYCLES_EXTRA == 2			
	#define cycle_nop		"nop \n\t nop \n\t"
#elif CYCLES_EXTRA == 1
	#define cycle_nop		"nop \n\t"
#else
	#define cycle_nop		/*empty*/
#endif


// Calculate tolerance
#define TOLERANCE_PPM				(1000000*(TINYUART_BAUD)*(LOOP_DELAY*CYCLES_LOOP+CYCLES_TX+CYCLES_EXTRA)/TINYUART_F_CPU-1000000)
#define IS_WITHIN_RANGE(x, range)	((x < -range) || (x > range))

#if   defined(TINYUART_OPT_TOLERANCE_ERR) && IS_WITHIN_RANGE(TOLERANCE_PPM, 45000)
	#error "Selected baud rate can not be archived within tolerance."
#endif
#if defined(TINYUART_OPT_TOLERANCE_MSG) && IS_WITHIN_RANGE(TOLERANCE_PPM, 20000)
	#pragma message "Selected baud rate is beyond recommended tolerance limits. Needs accurate receiver to work."
#endif

// debug only: get loop delay formula with values inserted
// #define WRAP(a)	#a
// #define STR(a)	WRAP(a)
// #pragma message STR(LOOP_DELAY)


//////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////

#ifdef TINYUART_OPT_DBG
	volatile uint8_t dbg_loop_delay;
	volatile uint8_t dbg_cycles_extra;
	volatile uint8_t dbg_cycles;
#endif
	

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

void tinyuart_send_uint8(uint8_t data)
{
	_io_port(TINYUART_IO_TX)	|= (1 << _io(TINYUART_IO_TX));
	_io_ddr(TINYUART_IO_TX)		|= (1 << _io(TINYUART_IO_TX));
	
	#ifdef TINYUART_OPT_DBG
		dbg_loop_delay		= LOOP_DELAY;
		dbg_cycles_extra	= CYCLES_EXTRA;
		dbg_cycles			= CYCLES;
	#endif
	
	uint8_t cnt;		
	uint8_t mask = 1<<_io(TINYUART_IO_TX);	
	
	asm volatile
	(
			"mov	%[cnt],			%[data]		\n\t"	// use cnt as scratch pad
		
			"out	%[pin],			%[mask]		\n\t"	// Setup START bit => moved for cycles bevore delay loop for accurate timing
		
			"subi	%[cnt],			128			\n\t"	// invert MSB
			"lsl	%[cnt]						\n\t"	// shift to a) generate pattern with XOR b) place msb in carry		
			"eor	%[data],		%[cnt]		\n\t"	// generate toggle pattern
			"ldi	%[cnt],			9			\n\t"	// init counter: 8 bits + stop
		
		"send_bit:"
			"mov	__tmp_reg__,	%[loops]	\n\t"	// bit delay loop
		
			cycle_nop									// insert nops for cycle-accurate delays
		
		"send_bit_delay:"
			"dec	__tmp_reg__					\n\t"
			"brne	send_bit_delay				\n\t"

			"sbrc	%[data],		0			\n\t"   // toggle TX if change is required
			"out	%[pin],			%[mask]		\n\t"

			"ror	%[data]						\n\t"	// rotation to access the 9th bit in carry
			"dec	%[cnt]						\n\t"
			"brne	send_bit					\n\t"
		
		// output values, can be safely written to. "+" attribute is read-write
		: [data]	"+r"(data),							// use original value and allow overwrite
		  [cnt]		"=&d"(cnt)							// let compiler pick temporary register
		  
		// input values; never write to any of the below
		: [mask]	"r"(mask),							// let compiler put mask in register
		  [loops]	"r"((uint8_t)(LOOP_DELAY+1)),		// the 0th loop iteration already decrements, +1 as compensation
		  [pin]		"I"(_SFR_IO_ADDR(_io_ddr(TINYUART_IO_TX)))
		
		// cobblers: registers modified without compiler already knowing
		: /*none*/
	);
}