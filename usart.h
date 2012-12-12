/*
 * -------------------------------------------------------------------------------
 * File Name	: usart.h
 * Title		: Unbuffered USART library.
 * Description  : Handles serial communication using the hardware USART
 *                available in newer/bigger AVR's.
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 27/09/2007
 * Revised		: 27/09/2007
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * Important note: These files are for use with AVR's that only have a single
 *                 USART, for dual USART support user usart2.{h,c} files.
 *
 * TODO: Implement a callback mechanism for recieves?
 * TODO: #ifdefs which give errors for dual usart AVR's.                    Done.
 * TODO: Find the difference between USART_BAUDRATE and USART_BAUDRATE2.
 * TODO: Generalize datamode and interrupt settings.
 * TODO: Define common datamodes.
 *         - f.x. #define 8N1 ( 1 << URSEL ) | ( 1 << UCSZ0 ) | ( 1 << UCSZ1 )
 * -------------------------------------------------------------------------------
 */

#ifndef UART_H
#define UART_H

#include "config.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

//Finna muninn á þessum uart_baudrate föllum....

#ifdef F_CPU
    #define USART_BAUDRATE(baud)  ( ( F_CPU / ( 16 * baud ) ) - 1 )
    #define USART_BAUDRATE2(baud) ( ( F_CPU + ( baud * 8L ) ) / ( baud * 16L ) - 1 )
#else
    #error Please define F_CPU macro in config.h
#endif

//Error if has more than 1 usart, maybe changed in future versions.
//But this is a simpler starting point.
#if	defined(__AVR_ATmega128__) || \
    defined(__AVR_ATmega64__) || \
    defined(__AVR_ATmega162__) || \
    defined(__AVR_ATmega1281__) || \
    defined(__AVR_ATmega2561__) || \
    defined(__AVR_ATmega164__) || \
    defined(__AVR_ATmega324__)
    #error "Only single USART AVR's supported at this point."
#elif defined(__AVR_ATmega640__) || \
      defined(__AVR_ATmega1280__) || \
      defined(__AVR_ATmega2560__)
    #error "Only single USART AVR's supported at this point."
#endif

extern  void    usart_init( uint32_t );
extern  uint8_t usart_getc( void );
extern  void    usart_putc( const char );
extern  void    usart_puts( const char* );
extern  void    usart_puts_P( const char* );
extern  void    usart_printnum( int16_t );
/*extern  void    usart_set_recv_handler( void (*recv_fun_ptr)( uint8_t ) );*/

#endif
