#include "uprintf.h"
#include <avr/pgmspace.h>
#include <stdarg.h> // for arglist

#define READ_FROM_RAM_ROM(readFromRam,character)\
    ( ( ( readFromRam ) ? ( *character ) : ( pgm_read_byte( character ) ) ) )

static putc_func_t putc;
static inline void uprintf_prints_helper( const char* s, uint8_t in_ram );
static inline void uprintf_printf_helper( const char* format, uint8_t in_ram, va_list arguments );

void uprintf_init( putc_func_t printer )
{
    putc = printer;
}

inline void uprintf_printc( const char c )
{
    putc( c );
}

void uprintf_prints( const char* s )
{
    uprintf_prints_helper( s, 1 );
}

void uprintf_printdec( int32_t dec )
{
    int32_t c;
    
    if( dec < 0 )
    {
        uprintf_printc( '-' );
        dec = -dec;
    }
    c = dec / 10;
    if( c )
        uprintf_printdec( c );
    uprintf_printc( '0' + ( ( uint8_t ) ( dec % 10 ) ) );
}

#ifdef UPRINTF_EXTENDED
inline void uprintf_printf( const char* format, ... )
{
    va_list arguments;
    va_start( arguments, format );
    uprintf_printf_helper( format, 1, arguments );
    va_end( arguments );
}
#endif

#ifdef UPRINTF_WITH_PROGMEM_SUPPORT
inline void uprintf_prints_p( const char* ps )
{
    uprintf_prints_helper( ps, 0 );
}
#ifdef UPRINTF_EXTENDED 
inline void uprintf_printf_p( const char* format, ... )
{
    va_list arguments;
    va_start( arguments, format );
    uprintf_printf_helper( format, 0, arguments );
    va_end( arguments );
}
#endif
#endif

#ifdef UPRINTF_WITH_FLOAT_SUPPORT
void uprintf_printfloat( float num, uint8_t precision )
{
    // Find sign and get abs val.
    if( num < 0.0 )
    {
        uprintf_printc( '-' );
        num = -num;
    }

    // Find the decimal point
    // floats can have max log10(2^24)
    // decimal digits, which is about 7.2
    // use 9 to be sure to get all
    float decimal_place = 1.0f;
    uint8_t num_digits;
    for( num_digits = 0; num_digits < 9; ++num_digits )
    {
        if( num / decimal_place < 10.0 )
            break;
        else
            decimal_place *= 10.0;
    }

    // ...and start outputting the characters
    uint8_t num_to_print = num_digits + precision + 1;
    uint8_t to_print;
    for( int i = 0; i < num_to_print; ++i )
    {
        to_print = ( num / decimal_place );

        uprintf_printc( to_print | 0x30 );

        if( decimal_place == 1.0 )
            uprintf_printc( '.' );

        num -= (to_print * decimal_place );
        decimal_place /= 10.0;
    }
}
#endif

// Internal helper functions
static inline void uprintf_prints_helper( const char* s, uint8_t in_ram )
{
    if( !s )
        return;

    while( *s )
    {
        uprintf_printc( READ_FROM_RAM_ROM( in_ram, s ) );
        s++;
    }
}

static inline void uprintf_printf_helper( const char* format, uint8_t in_ram, va_list arguments )
{
    // TODO: Finish
    unsigned char current_token;
    int16_t short_arg;
    int32_t long_arg;
    double double_arg;
    char* str_arg;
    uint8_t precision = 0;
    
    while( ( current_token = READ_FROM_RAM_ROM( in_ram, format ) ) )
    {
        if( current_token == '%' )
        {
            current_token = READ_FROM_RAM_ROM( in_ram, format );
            format++;
            switch( current_token )
            {
                case 'd':
                    long_arg = va_arg( arguments, int32_t );
                    uprintf_printdec( long_arg );
                    break;
                case 'c':
                    // char gets promoted to int when passed through ...
                    short_arg = va_arg( arguments, int16_t );
                    uprintf_printc( (char)short_arg );
                    break;
                case 's':
                    str_arg = va_arg( arguments, char* );
                    uprintf_prints( str_arg );
                    break;
                case '.':
                    break;
                case 'f':
                    double_arg = va_arg( arguments, double );
                    uprintf_printfloat( (float)double_arg, precision );
                    break;
                default:
                    uprintf_prints( "printf error: expected d, c, s, . or f after %." );
            }
        }
        else
        {
            uprintf_printc( current_token );
        }
        ++format;
    }

    return;
}
