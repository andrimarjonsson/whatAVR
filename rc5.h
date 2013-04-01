/*
 * -------------------------------------------------------------------------------
 * File Name	: rc5.h
 * Title		: rc5 protocol
 * Description  : Handles capture and decoding of rc5 data.
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 22/06/2008
 * Revised		: 22/06/2008
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * TODO: Make getter macros atomic.
 *
 * Feature: Use callbacks for handling?
 *
 * Improvement: Use inline functions instead of getter macros?
 *
 * -------------------------------------------------------------------------------
 */
#ifndef RC5_H
#define RC5_H

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

extern volatile int8_t rc5_valid;
extern volatile uint16_t rc5_stream;

/* Bitmasks */
#define RC5_SYNC_START  0b0011000000000000
#define RC5_TOGGLE      0b0000100000000000
#define RC5_ADDRESS     0b0000011111000000
#define RC5_COMMAND     0b0000000000111111

/* If you are having trouble getting 
 * valid readings, increase this number.
 * Note: Odd values are prefered for symmetrical level detection
 */
#define RC5_RESAMPLE 59

/* Configure timer prescaler */
#if F_CPU < 285000
    #define PRESCALEDIV 1
    #define TIMERCR (1<<CS00)
#elif F_CPU < 2300000
    #define PRESCALEDIV 8
    #define TIMERCR (1<<CS01)
#elif F_CPU < 18000000 
    #define PRESCALEDIV 64
    #define TIMERCR (1<<CS01) | (1<<CS00) 
#else
    #define PRESCALEDIV 256
    #define TIMERCR (1<<CS02)
#endif

#define RC5TIMERFIRST  0x100 - ( (uint8_t) ( F_CPU / PRESCALEDIV * 0.0008890 ) )
#define RC5TIMERSECOND 0x100 - ( (uint8_t) ( F_CPU / PRESCALEDIV * 0.0004445 ) )
#define RC5TIMERCANCEL 0x100 - ( (uint8_t) ( F_CPU / PRESCALEDIV * 0.0008890 ) )

/* Function prototypes, and simple macros. Saves memory. */
extern void rc5_init( void );
#define rc5_reset() rc5_valid = 0;

/* Getter macros. */
#define rc5_get_command()   ( ( uint8_t ) ( rc5_stream & RC5_COMMAND ) )
#define rc5_get_address()   ( ( uint8_t ) ( ( rc5_stream & RC5_ADDRESS ) >> 6 ) )
#define rc5_get_toggle()    ( ( uint8_t ) ( ( rc5_stream & RC5_TOGGLE ) >> 11 ) )
#define rc5_get_syncstart() ( ( uint8_t ) ( ( rc5_stream & RC5_SYNC_START ) >> 12 ) )

#endif /* RC5_H */
