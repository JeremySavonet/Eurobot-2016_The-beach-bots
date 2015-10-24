/*
 * Simple demo program
 *
 * Description: 
 * A simple HelloWorld program that create a task that blinks a led
 *
 * Added support for:
 * 90s timer to stop all the thread
 * Uart
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

static int gameTick = 0;
static thread_t *tp;
static virtual_timer_t gameTimer;

/*
 * Game running loop 
 */
void runGame( void *p ) 
{
 
    /* Restarts the timer.*/
    chSysLockFromISR();
    chVTSetI( &gameTimer, MS2ST( 1000 ), runGame, p );
    chSysUnlockFromISR();
 
    
    // Wait 90s and stop all the thread properly
    if( ++gameTick >= 90 )
    {
        // Stop the blinker thread
        chThdWait( tp ); 
    }
}

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

    // Start game timer
    chVTSet( &gameTimer, MS2ST( 1000 ), runGame, NULL );
 
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );

    // used function : USART3_TX 
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // used function : USART3_RX 
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // starts the serial driver with uartCfg as a config 
    sdStart( &SD3, &uartCfg ); 
    const char data[] = "Hello World ! \n \r"; 
 
    /*
     * Creates the blinker thread.
     */
    tp = chThdCreateStatic( waThread1,
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
