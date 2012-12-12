/*
 * -------------------------------------------------------------------------------
 * File Name	: keypad.h
 * Title		: Keypad reader.
 * Description  : Reads value from a 16 button keypad connected in matrix fashion.
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 29/09/2007
 * Revised		: 29/09/2007
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 *
 * Keypad layout:
 *      4   5   6   7
 *      |   |   |   |
 *    +---------------+
 * 3 -| 1   2   3   A |
 * 2 -| 4   5   6   B |
 * 1 -| 7   8   9   C |
 * 0 -| *   0   #   D |
 *    +---------------+
 *
 * NOTE: Use callback?
 * -------------------------------------------------------------------------------
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>

#include "config.h"

#if !(defined(KEYPAD_PORT) || defined(KEYPAD_PIN) || defined(KEYPAD_DDR))
    #error "Must define KEYPAD_{PORT,PIN,DDR} macros."
#endif

extern void     keypad_init( void );
extern uint8_t  keypad_get_key( void );

#endif /* KEYPAD_H */
