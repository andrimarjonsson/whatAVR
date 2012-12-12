#include "keypad.h"

void keypad_init( void )
{
    KEYPAD_DDR = 0xF0;  // Rows: low nibble. Columns: high nibble.
    KEYPAD_PORT = 0x0F; // Enable pullups.
}

uint8_t keypad_get_key( void )
{
    //first column
    KEYPAD_PORT = 0b11101111;
    if ( bit_is_set( KEYPAD_PIN, 3 ) ) 
        return '1';
    if ( bit_is_set( KEYPAD_PIN, 2 ) ) 
        return '4';
    if ( bit_is_set( KEYPAD_PIN, 1 ) ) 
        return '7';
    if ( bit_is_set( KEYPAD_PIN, 0 ) ) 
        return '*';

    //second column
    KEYPAD_PORT = 0b11011111;
    if ( bit_is_set( KEYPAD_PIN, 3 ) ) 
        return '2';
    if ( bit_is_set( KEYPAD_PIN, 2 ) ) 
        return '5';
    if ( bit_is_set( KEYPAD_PIN, 1 ) ) 
        return '8';
    if ( bit_is_set( KEYPAD_PIN, 0 ) ) 
        return '0';

    //third column
    KEYPAD_PORT = 0b10111111;
    if ( bit_is_set( KEYPAD_PIN, 3 ) )
        return '3';
    if ( bit_is_set( KEYPAD_PIN, 2 ) ) 
        return '6';
    if ( bit_is_set( KEYPAD_PIN, 1 ) ) 
        return '9';
    if ( bit_is_set( KEYPAD_PIN, 0 ) )
        return '#';

    //fourth column
    KEYPAD_PORT = 0b01111111;
    if ( bit_is_set( KEYPAD_PIN, 3 ) ) 
        return 'A';
    if ( bit_is_set( KEYPAD_PIN, 2 ) ) 
        return 'B';
    if ( bit_is_set( KEYPAD_PIN, 1 ) ) 
        return 'C';
    if ( bit_is_set( KEYPAD_PIN, 0 ) ) 
        return 'D';
    
    return 0;
}