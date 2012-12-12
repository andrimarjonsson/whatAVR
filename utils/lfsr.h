/*
 * -------------------------------------------------------------------------------
 * File Name	: lfsr.h
 * Title		: Linear Feedback Shift Register
 * Description  : LFSR in Galois configuration for pseudorandom number generation.
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 26/09/2007
 * Revised		: 27/09/2007
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * Adapted from example at 
 *   http://en.wikipedia.org/wiki/Linear_feedback_shift_register
 * -------------------------------------------------------------------------------
 */

#ifndef LFSR_H
#define LFSR_H

#include "config.h"

#include <avr/io.h>

#ifndef LFSR_BITS
    #error "Please specify number of bits, 8,16 and 32 bits supported, to be used in LFSR generation."
#else
    #if LFSR_BITS == 8
        #define LFSR_USIZE uint8_t
        #define LFSR_SIZE int8_t
        #define DEFAULT_TAP 0xD1        /* taps bits 8 7 5 1 */
    #elif LFSR_BITS == 16
        #define LFSR_USIZE uint16_t
        #define LFSR_SIZE int16_t
        #define DEFAULT_TAP 0xD001      /* taps bits 16 15 13 1 */
    #elif LFSR_BITS == 32
        #define LFSR_USIZE uint32_t
        #define LFSR_SIZE int32_t
        #define DEFAULT_TAP 0xD0000001  /* taps bits 32 31 29 1 */
    #else
        #error "Only 8,16 and 32 bits are supported at this moment."
    #endif
#endif

#ifndef LFSR_TAP
    #define LFSR_TAP DEFAULT_TAP
#endif

#define LFSR_RAND_MAX 0xFFFF

extern  void        lfsr_srand( LFSR_USIZE );
extern  LFSR_USIZE  lfsr_rand( void );

#endif

