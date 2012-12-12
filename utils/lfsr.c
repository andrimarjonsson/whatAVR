#include "lfsr.h"

static LFSR_USIZE next = 42;

void lfsr_srand( LFSR_USIZE lfsr_seed )
{
    next = lfsr_seed;
}

LFSR_USIZE lfsr_rand( void )
{
    next = ( ( next >> 0x01 ) ^ ( - ( LFSR_SIZE ) ( next & 0x01 ) & LFSR_TAP ) );
    return next;
}
