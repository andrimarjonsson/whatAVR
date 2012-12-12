/*
 * -----------------------------------------------------------------------------
 * File Name	: clcd.h
 * Title		: Character LCD driver
 * Description  : Character LCD driver (HD44780U in 4-bit IO mode)
 * Author		: Andri Mar Jónsson. Original code by Peter Fleury.
 * License      : LGPL
 * Created		: 02/01/2008
 * Revised		: 11/01/2008
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * TODO: Incorporate the avr-libc delay lib instead of the homebrew delays
 *       currently in the code.
 * -----------------------------------------------------------------------------
 */


#ifndef CLCD_H
#define CLCD_H

#include <avr/io.h>

#include "config.h"

#if LCD_HAVE_PROGMEM_SUPPORT
#include <avr/pgmspace.h>
#endif
#include <util/delay.h>

#include "utils/macros.h"


/* general commands */
#define LCD_CLR               0      //DB0: clear display
#define LCD_HOME              1      //DB1: return to home position
#define LCD_ENTRY_MODE        2      //DB2: set entry mode
#define LCD_ENTRY_INC         1      //DB1: 1=increment, 0=decrement
#define LCD_ENTRY_SHIFT       0      //DB2: 1=display shift on
#define LCD_ON                3      //DB3: turn lcd/cursor on
#define LCD_ON_DISPLAY        2      //DB2: turn display on
#define LCD_ON_CURSOR         1      //DB1: turn cursor on
#define LCD_ON_BLINK          0      //DB0: blinking cursor ?
#define LCD_MOVE              4      //DB4: move cursor/display
#define LCD_MOVE_DISP         3      //DB3: move display (0-> cursor) ?
#define LCD_MOVE_RIGHT        2      //DB2: move right (0-> left) ?
#define LCD_FUNCTION          5      //DB5: function set
#define LCD_FUNCTION_8BIT     4      //DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES   3      //DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS   2      //DB2: 5x10 font (0->5x7 font)
#define LCD_CGRAM             6      //DB6: set CG RAM address
#define LCD_DDRAM             7      //DB7: set DD RAM address
#define LCD_BUSY              7      //DB7: LCD is busy

/* entry modes, see <url> for description and example of each */
#define LCD_ENTRY_DEC            0x04   //display shift off, dec cursor move dir
#define LCD_ENTRY_DEC_SHIFT      0x05   //display shift on,  dec cursor move dir
#define LCD_ENTRY_INC_           0x06   //display shift off, inc cursor move dir
#define LCD_ENTRY_INC_SHIFT      0x07   //display shift on,  inc cursor move dir

/* display/cursor on/off */ 
#define LCD_DISP_OFF             0x08   //display off
#define LCD_DISP_ON              0x0C   //display on, cursor off
#define LCD_DISP_ON_BLINK        0x0D   //display on, cursor off, blink char
#define LCD_DISP_ON_CURSOR       0x0E   //display on, cursor on
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   //display on, cursor on, blink char

/* move commands */
#define LCD_MOVE_CURSOR_LEFT     0x10   //move cursor left  (decrement)
#define LCD_MOVE_CURSOR_RIGHT    0x14   //move cursor right (increment)
#define LCD_MOVE_DISP_LEFT       0x18   //shift display left
#define LCD_MOVE_DISP_RIGHT      0x1C   //shift display right


#define LCD_FUNCTION_4BIT_1LINE  0x20   //4-bit interface, single line, 5x7 dots
#define LCD_FUNCTION_4BIT_2LINES 0x28   //4-bit interface, dual line, 5x7 dots

#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )

/* Functions */
extern void lcd_init(uint8_t dispAttr);

extern void lcd_clrscr(void);

extern void lcd_home(void);

extern void lcd_gotoxy(uint8_t x, uint8_t y);

extern void lcd_putc(char c);

extern void lcd_puts(const char *s);

extern void lcd_command(uint8_t cmd);

extern void lcd_data(uint8_t data);

extern void lcd_put_string_float( const char *s, uint8_t decimalLoc );

#if LCD_HAVE_PROGMEM_SUPPORT

extern void lcd_puts_p(const char *progmem_s);

#endif

#endif /* CLCD_H */
