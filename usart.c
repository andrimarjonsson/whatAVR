#include "usart.h"

void usart_init( uint32_t _baudrate )
{
    uint16_t bauddivisor;
    
    UCSRB |= ( 1 << TXEN ) | ( 1 << RXEN ) | ( 1 << RXCIE );        //UART Tx Enable, Rx enable and generates Rx Interrupt.
    UCSRC |= ( 1 << URSEL ) | ( 1 << UCSZ0 ) | ( 1 << UCSZ1 );      //Data mode: 8N1
    
    //Set baudrate
    bauddivisor = USART_BAUDRATE(_baudrate);
    UBRRL = bauddivisor;
    #ifdef UBRRH
    UBRRH = ( bauddivisor >> 8 );
    #endif
}

uint8_t usart_getc( void )
{
    loop_until_bit_is_set(UCSRA, RXC);     //Wait for USART to be ready
    return UDR;
}

void usart_putc( const char _c )
{
    loop_until_bit_is_set( UCSRA, UDRE );   //Wait for USART to be ready
    UDR = _c;
}

void usart_puts( const char* _str )
{
    while( *_str )
        usart_putc( *_str++ );
}

void usart_printnum( int16_t _num )
{
    int16_t c;
    
    if( _num < 0 )
    {
        usart_putc( '-' );
        _num = -_num;
    }
    c = _num / 10;
    if( c )
        usart_printnum( c );
    usart_putc( '0' + ( ( uint8_t ) ( _num % 10 ) ) );
}

void usart_puts_P( const char* _str )
{
    uint8_t c;
    while( (c = pgm_read_byte( _str++ ) ) ) //Short for checking for '\0'
        usart_putc( c );
}

