/*
 * -------------------------------------------------------------------------------
 * File Name	: config_template.h
 * Title		: Whatever template configuration file.
 * Description  : Configuration template for the whatever AVR library.
 * Author		: Andri Mar Jónsson - Copyright (C) 2007
 * Created		: 26/09/2007
 * Revised		: dd/mm/yyyy
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * !!Copy this file to your projects root directory and rename to config.h!!
 * -------------------------------------------------------------------------------
 */
#ifndef CONFIG_H
#define CONFIG_H

/* 
 * -------------------------------------------------------------------------------
 * Start by setting the clock, very very important.
 * -------------------------------------------------------------------------------
 */
#define F_CPU 4000000UL

/* 
 * -------------------------------------------------------------------------------
 * LFSR related config
 * -------------------------------------------------------------------------------
 */
//#define LFSR_BITS 16
//#define LFSR_TAP 0xD001

/* 
 * -------------------------------------------------------------------------------
 * USART related config
 * -------------------------------------------------------------------------------
 */


/* 
 * -------------------------------------------------------------------------------
 * Keypad related config
 * -------------------------------------------------------------------------------
 */
//#define KEYPAD_PORT PORTx
//#define KEYPAD_PIN  PINx
//#define KEYPAD_DDR  DDRx

/* 
 * -------------------------------------------------------------------------------
 * Character LCD related config (HD44780U in 4-bit IO mode)
 * -------------------------------------------------------------------------------
 */
/*General*/
//#define LCD_HAVE_PROGMEM_SUPPORT 0 /*Include functions dealing with progmem strings*/

/* Display setup */
//#define LCD_LINES           2     /* lines on display 1, 2 or 4 */
//#define LCD_DISP_LENGTH    16     /* chars per line */
//#define LCD_LINE_LENGTH  0x40     /* line length(in memory)*/
//#define LCD_START_LINE1  0x00     /* start mem address for line 1 */
//#define LCD_START_LINE2  0x40     /* start mem address for line 1 */
//#define LCD_START_LINE3  0x14     /* start mem address for line 1 */
//#define LCD_START_LINE4  0x54     /* start mem address for line 1 */
//#define LCD_WRAP_LINES      0     /* line wrapping flag */

/* Port setup */
//#define LCD_PORT         PORTA        /* port for the LCD lines   */
//#define LCD_DATA0_PORT   LCD_PORT     /* port for data bit 0 */
//#define LCD_DATA1_PORT   LCD_PORT     /* port for data bit 1 */
//#define LCD_DATA2_PORT   LCD_PORT     /* port for data bit 2 */
//#define LCD_DATA3_PORT   LCD_PORT     /* port for data bit 3 */
//#define LCD_DATA0_PIN    0            /* data bit 0  */
//#define LCD_DATA1_PIN    1            /* data bit 1  */
//#define LCD_DATA2_PIN    2            /* data bit 2  */
//#define LCD_DATA3_PIN    3            /* data bit 3  */
//#define LCD_RS_PORT      LCD_PORT     /* port for RS line         */
//#define LCD_RS_PIN       4            /* pin  for RS line         */
//#define LCD_RW_PORT      LCD_PORT     /* port for RW line         */
//#define LCD_RW_PIN       5            /* pin  for RW line         */
//#define LCD_E_PORT       LCD_PORT     /* port for Enable line     */
//#define LCD_E_PIN        6            /* pin  for Enable line     */

/* 
 * -------------------------------------------------------------------------------
 * NS73M related config
 * -------------------------------------------------------------------------------
 */
/* Port settings */
//#define NS73M_PORT	PORTB
//#define NS73M_CK_PORT	NS73M_PORT
//#define NS73M_DA_PORT	NS73M_PORT
//#define NS73M_LA_PORT	NS73M_PORT
//#define NS73M_CK		1
//#define NS73M_DA		2
//#define NS73M_LA		3
/* Defaults frequency setup */
//#define NS73M_DEFAULT_FREQ    953     //95.3 MHz
//#define NS73M_DEFAULT_BAND    NS73M_BAND2

/* 
 * -------------------------------------------------------------------------------
 * RC5 related config
 * -------------------------------------------------------------------------------
 */
 
/* 
 * -------------------------------------------------------------------------------
 * uprintf related config
 * -------------------------------------------------------------------------------
 */

//#define UPRINTF_EXTENDED
//#define UPRINTF_WITH_PROGMEM_SUPPORT 
//#define UPRINTF_WITH_FLOAT_SUPPORT
//#define UPRINTF_
//#define UPRINTF_
//#define UPRINTF_

#endif
