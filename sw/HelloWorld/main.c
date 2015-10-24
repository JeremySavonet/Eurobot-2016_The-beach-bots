/*
 * Simple demo program
 *
 * Description: 
 * A simple HelloWorld program that create a task that blinks a led
 *
 * Author: JS
 * Date: 2015-10-03
 * Version: V0.1
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
* Struct to config serial module
*/
static SerialConfig uartCfg =
{
    115200, // bit rate
    0,
    0,
    0,
};

/*
* Green LED blinker thread, times are in milliseconds.
*/
static THD_WORKING_AREA( waThread1, 128 );
static THD_FUNCTION( Thread1, arg )
{
    (void)arg;
    chRegSetThreadName( "blinker" );
    while( true )
    {
        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );
    }
}

int main( void )
{
    /*
    * System initializations.
    * - HAL initialization, this also initializes the configured device drivers
    *   and performs the board-specific initializations.
    * - Kernel initialization, the main() function becomes a thread and the
    *   RTOS is active.
    */
    halInit();
    chSysInit();

    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );

    // used function : USART3_TX 
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // used function : USART3_RX 
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // starts the serial driver with uartCfg as a config 
    sdStart( &SD3, &uartCfg ); 
    char data[] = "Hello World ! \n \r"; 
 
    /*
    * Creates the blinker thread.
    */
    chThdCreateStatic( waThread1,
                       sizeof( waThread1 ),
                       NORMALPRIO,
                       Thread1,
                       NULL );

    /*
    * Normal main() thread activity, in this demo it does nothing except
    * sleeping in a loop and listen for events.
    */
    while( true )
    {
        // Writes "Hello World in the UART output
        sdWrite( &SD3, (uint8_t *) data, strlen( data ) ); 
        chThdSleepMilliseconds( 500 ); /* Iddle thread */
    }
}
