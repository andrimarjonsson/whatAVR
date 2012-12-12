#whatAVR
Small collection of code I refactored from various project I've done relating to AVRs. Getting pretty dated now since I haven't done much AVR programming in a few years.

Below is a small overview of the modules available.

##Getting started
1. `mkdir /path/to/project`
2. `touch /path/to/project/main.c`
3. `cp /path/to/whatAVR/Makefile_template /path/to/project/Makefile`
4. `cp /path/to/whatAVR/config.h /path/to/project/config.h`
5. `cd /path/to/project`
5. Edit the paths in the `Makefile` to point to where whatAVR is.
6. Add the following code to `main.c` and run `make` 

```cpp
#include "config.h"

#include <avr/io.h>

int main( void ) 
{
    sei();
    
    // start main loop
    while( 1 ) 
    {

    }
    return 0;
}
```



##Modules
| Name | Description | 
| ------------ | ------------- |
| LFSR | Pseudo-random number generator using linear feedback shift register in Galois configuration. |
| RC5 | Decodes RC5 codes from IR remotes. |
| UPRINTF | Small printf lib. |
| USART | Hardware USART library with interrupt support and send/recv callbacks. |
| SPI | Hardware SPI library. |
| TWI | Hardware TWI/I2C library. |

##Drivers
| Name | Description | 
| ------------ | ------------- |
| SD/MMC | Routines for accessing SD/MMC cards. |
| MAX6675 | Routines for communicating with the MAX6675 chip from Maxim. |
| NSM73M | Routines for communicating with the NSM73M FM transmitter from SparkFun. |
| CLCD | Library for character LCD's using the HD44780U(4 bit mode) and compatible controllers. |