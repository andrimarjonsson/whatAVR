/*
 * -----------------------------------------------------------------------------
 * File Name	: uprintf.h
 * Title		: Micro printf library
 * Description  : Very small printf lib supporting only basic functions. 
 * Author		: Andri Mar Jónsson.
 * License      : LGPL
 * Created		: 07/02/2011
 * Revised		: dd/mm/yyyy
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * -----------------------------------------------------------------------------
 */

#ifndef UPRINTF_H
#define UPRINTF_H
#include "config.h"

#include <avr/io.h>

typedef void (*putc_func_t)( const char c );

// Basic interface
extern void uprintf_init( putc_func_t printer );
extern void uprintf_printc( const char c );
extern void uprintf_prints( const char* s );
extern void uprintf_printdec( int32_t dec );
// Basic interface begin

#ifdef UPRINTF_EXTENDED
extern void uprintf_printf( const char* format, ... );
#endif

#ifdef UPRINTF_WITH_PROGMEM_SUPPORT
extern void uprintf_prints_p( const char* ps );
#ifdef UPRINTF_EXTENDED 
extern void uprintf_printf_p( const char* format, ... );
#endif
#endif

#ifdef UPRINTF_WITH_FLOAT_SUPPORT
extern void uprintf_printfloat( float num, uint8_t precision );
#endif

#endif
