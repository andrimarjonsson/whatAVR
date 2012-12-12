/*
 * -------------------------------------------------------------------------------
 * File Name	: macros.h
 * Title		: Utility macros
 * Description  : Global utility macros for use in the library
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 14/01/2008
 * Revised		: dd/mm/yyyy
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * NOTE: Source of code if not my own is in comments near the "contributed" code.
 * -------------------------------------------------------------------------------
 */

#ifndef MACROS_H
#define MACROS_H

#include <avr/io.h>

/* 
 * Macros for getting DDR and PIN from the portname.
 * Original code by Peter Fleury.
 * TODO: Find more corner cases, there just must be more, especially on the higher pincount AVR's.
 */
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
    /* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
    #define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
	#define PIN(x) (*(&x - 2))    /* address of input register of port x */
#endif

#endif /* MACROS_H */
