#include "rc5.h"

/*
 * VARIABLES
 */
volatile int8_t rc5_valid = 0;
volatile uint16_t rc5_stream = 0;

static uint8_t rc5_bit = 0;
static uint8_t rc5_first_half;
static uint8_t first_level = 1;

/*
 * 'PRIVATE' FUNCTIONS
 */

static void rc5_capture_reset( void )
{
    MCUCR &= ~( 1 << ISC10 ); // falling edge
    GICR |= ( 1 << INT1 );    // ext. int. enabled
    GIFR |= ( 1 << INTF1 );   // clear an eventually set ext. int. flag
    TIMSK &= ~( 1 << TOIE0 ); // disable timer int.
    first_level = 1;          // as the next ext. int. appears at the 
                              //falling edge, assume the first level as high
    rc5_bit = 0;
}

/*
 * PUBLIC FUNCTIONS
 */
void rc5_init( void )
{
    TCCR0 = TIMERCR;         // Set prescaler
    TIMSK &= ~( 1 << TOIE0); // timer ovf disabled. gets enabled by ext. int.

    DDRD &= ~( 1 << DD3);    // pin is input
    PORTD &= ~( 1 << PD3);   // pullup disabled (receiver has defined output)

    MCUCR |= ( 1 << ISC11);  // ext. int. activated by falling edge
    MCUCR &= ~( 1 << ISC10);

    GICR |= ( 1 << INT1 );   // ext. int. enabled
    GIFR |= ( 1 << INTF1 );  // clear an eventually set ext. int. flag
}

/*
 * INTERRUPTS
 */
ISR( INT1_vect, ISR_BLOCK )
{
    /*	external interrupt handler
    edge from IR receiver detected.
    this is assumed to be the middle of a bit.
    */

    uint8_t i;

    // resample to filter out spikes
    if( MCUCR & ( 1 << ISC10 ) )
    {   // we were waiting for a rising edge, so cancel if we sample low
        for( i = RC5_RESAMPLE; i; i-- )
        { 
            if ( !( PIND & ( 1 << PIND3 ) ) ) 
                return;
        }
    }
    else
    {   // vice versa...
        for( i = RC5_RESAMPLE; i; i-- ) 
        {
            if ( ( PIND & ( 1 << PIND3 ) ) ) 
                return;
        }
    }

    TCNT0 = RC5TIMERSECOND; // preset timer to sample (ovf int) at second 
                            // half (3/4 bit time)
    rc5_first_half = 0;     // next sample will be in the second half
    TIMSK |= ( 1 << TOIE0 );// enable timer ovf int
    GICR &= ~( 1 << INT1 ); // disable ext int (this handler)
}


ISR ( TIMER0_OVF_vect, ISR_BLOCK ) {
/*  timer overflow handler
    sample the present level of the interrupt line.
*/  
    static uint16_t rc5_shift;
    static uint16_t rc5_temp;
    static uint8_t level;
    uint8_t i = 0;
    uint8_t temp;

    // resample to filter out spikes
    for( temp = RC5_RESAMPLE; temp; temp-- ) 
    {
        if( PIND & ( 1<<PIND3 ) ) 
            i++;
    }

    level = ( i > ( ( uint8_t ) ( RC5_RESAMPLE / 2 ) ) );

    if( rc5_first_half == 2 )
    {   // this is a timeout, cancel operation
        rc5_capture_reset();
        return; // Since we are done, exit interrupt
    }
    else if( rc5_first_half == 1 )
    {   // this sample is taken in the first half of the bit (1/4 bit time)
        first_level = level; // save current level
        if( level ) 
            MCUCR &= ~( 1 << ISC10 ); // currently at high level, wait for falling edge
        else
            MCUCR |= ( 1 << ISC10 ); // currently at low level, wait for rising edge
        rc5_first_half = 2; // special code, means: next ovf int is a timeout
        GICR |= ( 1 << INT1 ); // enable ext int again
        TCNT0 = RC5TIMERCANCEL; // at timer ovf (in around 1/2 bit time) we should get a timeout
    }
    else
    {
        // this sample is taken in the second half of the bit (3/4 bit time)
        TCNT0 = RC5TIMERFIRST; // next sample at first half (1/4 bit time) of next bit
        rc5_first_half = 1;
        if( first_level != level )
        {   // levels differ -> valid manchester encoded bit detected
            if( !rc5_bit )
            {   // this is the first bit, reset values
                rc5_temp = 0;
                rc5_shift = 0x2000; //Shift 14bits
            }

            if( !level )
                rc5_temp |= rc5_shift; // active-low => store a '1'
            else if( rc5_bit < 2 )
            {
                rc5_capture_reset(); // high level('0' bit) as startbit -> error
                return; // Since we are done, exit interrupt
            }
            if( ++rc5_bit == 14 )
            {   // reception complete, stop operation
                rc5_stream = rc5_temp;
                rc5_valid = 1;
                rc5_capture_reset();
                return; // Since we are done, exit interrupt
            }
            
            rc5_shift >>= 1;
        }
        else// error in manchester stream -> cancel operation
            rc5_capture_reset();
            //Don't need return here since we will exit very soon
    }
}
