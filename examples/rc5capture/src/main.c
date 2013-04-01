
#include "config.h"

#include <avr/io.h>

#include "usart.h"
#include "rc5.h"

int main( void ) 
{
    usart_init(9600);
    rc5_init(); // activate RC5 detection
    
    usart_puts("RC5 protocol test application!");
    usart_puts("\r\n");
    
    sei();
    
    // start main loop
    while( 1 ) 
    {
        if( rc5_valid )
        {           
            usart_puts("Sync Start: ");
            usart_printnum(rc5_get_syncstart());
            usart_puts("\r\n");
            
            usart_puts("Toggle Bit: ");
            usart_printnum(rc5_get_toggle());
            usart_puts("\r\n");
            
            usart_puts("Address: ");
            usart_printnum(rc5_get_address());
            usart_puts("\r\n");
            
            usart_puts("Command: ");
            usart_printnum(rc5_get_command());
            usart_puts("\r\n\r\n");
            rc5_reset();
        }
    }
    return 0;
}
