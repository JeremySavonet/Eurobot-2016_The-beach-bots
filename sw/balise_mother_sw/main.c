/*
 * A simple program that blinks a led.
 * 
 * Description:
 * The program have a main that blink a LED4 - IO
 * All peripherals we'll need for the balise are configured (SPI, UART, TIMER)
 * 
 * Revision: 0.1
 * Date: 2015-10-11
 * Author: JS
 * 
 */

#include <string.h>

#include "ADXL362.h"
#include "config.h"
#include "console.h"
#include "delay.h"
#include "global_variables.h"
#include "version.h"

u16 sampled_rcon;

// ENTRY POINT
int main( void )
{
    // Clear WD timer before init
    ClrWdt();                   

    // Save RCON
    sampled_rcon = RCON; 
    
    // Reset RCON value
    RCON = 0;                   
    RCONbits.SWDTEN = WDT_ENABLED;
    RCONbits.RETEN = 1;

    global_init();
    DPRINT( 2, "START" );
    
    // Version
    DPRINT( 2, 
            "VERSION=%d.%d.%d", 
            MAJOR_VERSION, 
            MINOR_VERSION, 
            PATCH_VERSION );

    
    //Init ADXL362 measurement mode
    adxl362_init();

    while( 1 ) 
    {
        LED4 ^= 1; //Blink the led
        delay_ms( 250 );
    }
}
