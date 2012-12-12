/*
 * -------------------------------------------------------------------------------
 * File Name	: spi.h
 * Title		: SPI comm. lib.
 * Description  : General SPI communication library.
 * Author		: Andri Mar Jónsson
 * License      : LGPL
 * Created		: 29/09/2007
 * Revised		: 29/09/2007
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 * -------------------------------------------------------------------------------
 */

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

//Loads of ifdefs to map pins


//Interface
extern void     spi_init( void );
extern uint8_t  spi_transmit_byte( uint8_t );
extern uint16_t spi_transmit_word( uint16_t );
extern void     spi_send_byte( uint8_t );
extern uint8_t  spi_receive_byte( uint8_t _dummy_byte = 0xFF );

#endif
