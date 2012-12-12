#include "ns73m.h"

/*
 * PRIVATE
 */

/*
 * PUBLIC
 */
void ns73m_init( void )
{
    uint16_t freq_calc;
    
    /*Set as outputs*/
    DDR(NS73M_CK_PORT) |= ( 1<<NS73M_CK );
    DDR(NS73M_DA_PORT) |= ( 1<<NS73M_DA );
    DDR(NS73M_LA_PORT) |= ( 1<<NS73M_LA );
    
    /* Transmitter setup */
    ns73m_send( NS73M_REG14, NS73M_SOFT_RES );
    ns73m_send( NS73M_REG1, 0b10110100 );   //Register 1: forced subcarrier, pilot tone on
    ns73m_send( NS73M_REG2, 0b00000011 );   //Register 2: Unlock detect off, 2mW Tx Power

    freq_calc = ns73m_calc_freq( NS73M_DEFAULT_FREQ );

    ns73m_send( 0x03, ( ( uint8_t )( freq_calc & 0xFF ) ) );
    ns73m_send( 0x04, ( ( uint8_t )( ( freq_calc>>8 ) & 0xFF ) ) );

    ns73m_send( NS73M_REG8, NS73M_DEFAULT_BAND );
    ns73m_send( NS73M_REG0, 0b00000001 );   //Register 0: 200mV audio input, 50us pre-emphasis on, crystal off, power on
    ns73m_send( NS73M_REG14, NS73M_SOFT_RES );
    ns73m_send( NS73M_REG6, 0b00011010 );   //CIA -> 1.25uA, CIB -> 320uA

}

void ns73m_send( uint8_t _address, uint8_t _data )
{
    uint8_t i;
    
    /* ensure the latch is off */
    NS73M_LA_PORT &= ~( 1<<NS73M_LA );
    
    /* send address */
    for( i = 0; i < 4 ; i++ )
    {
        NS73M_CK_PORT &= ~( 1<<NS73M_CK );

        if( _address & ( uint8_t )( 1<<i ) )
            NS73M_DA_PORT |= ( 1<<NS73M_DA );
        else
            NS73M_DA_PORT &= ~( 1<<NS73M_DA );

        //Clock the bit out
        NS73M_CK_PORT |= ( 1<<NS73M_CK );
    }

    /* send data */
    for( i = 0 ; i < 8 ; i++ )
    {
        NS73M_CK_PORT &= ~( 1<<NS73M_CK );

        if( _data & ( uint8_t )( 1<<i ) )
            NS73M_DA_PORT |= ( 1<<NS73M_DA );
        else
            NS73M_DA_PORT &= ~( 1<<NS73M_DA );

        //Clock the bit out
        NS73M_CK_PORT |= ( 1<<NS73M_CK );
    }
    
    
    /* latch */
    NS73M_LA_PORT |= ( 1<<NS73M_LA );
    _delay_ms(1);
    NS73M_LA_PORT &= ~( 1<<NS73M_LA );
}

uint16_t ns73m_calc_freq( uint16_t _freq )
{
    if( (_freq < 875)  || ( _freq > 1080 ) )
        return -1;

    return ( uint16_t )( ( ( _freq / 10.0 ) + 0.304 ) / 0.008192);
}

uint8_t ns73m_get_band( uint16_t _calc_freq )
{
    /*
    Based on observations from: http://forum.sparkfun.com/viewtopic.php?t=9766
    Band 0: 96.1 - 106.1 (didn't test above 106.1)
    Band 1: 93.1 - 100.1
    Band 2: 89.1 - 99.1
    Band 3: 87.9 - 94.1 (didn't test below 87.9)
    
    Strategy: Find high and low points first, then split between Band 1-2
    */
    if( _calc_freq > 12256 )        //100.1
        return NS73M_BAND0;
    else if( _calc_freq < 10913 )    //89.1
        return NS73M_BAND3;
    else if( _calc_freq < 11584 )  // (100.1-89.1)/2 + 89.1 = 94.6
        return NS73M_BAND2;
    else
        return NS73M_BAND1;
}






