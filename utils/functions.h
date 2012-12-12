#ifndef FUNCTIONS_H
#define FUNCTIONS_H

uint8_t reverse_bit_order8( uint8_t _num )
{ 
    uint8_t i = 0x80;
    uint8_t temp = 0x01;
    while( i )
    { 
        if( _num & 0x01 )
            temp |= i;
        num >>= 1;
        i >>= 1;
    } 
    return temp;
}

#endif /* FUNCTIONS_H */
