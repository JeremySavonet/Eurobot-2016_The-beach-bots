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

static bool running = false;
static int gameTick = 0;
static thread_t *tp = NULL;
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

    if( ++gameTick < 90 ) {}
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
        const char data[] = "Running..."; 
        sdWrite( &SD3, (uint8_t *) data, strlen( data ) ); 
 
        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );

        // Wait 90s and stop all the thread properly
        if( gameTick >= 5 )
        {
            msg_t msg;
            chThdExit( msg );
        }
    }
}

static THD_WORKING_AREA( waThread2, 128 );
static THD_FUNCTION( Thread2, arg )
{
    (void)arg;
    chRegSetThreadName( "killer" );
   
    // Wait here all thread to terminate properly 
    chThdWait( tp );
    
    const char data[] = "Stop..."; 
    sdWrite( &SD3, (uint8_t *) data, strlen( data ) );
    palSetPad( GPIOC, GPIOC_LED );   
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

    // Used function : USART3_TX 
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // Used function : USART3_RX 
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) ); 
    
    // Starts the serial driver with uartCfg as a config 
    sdStart( &SD3, &uartCfg ); 
    
    const char data[] = "Ready..."; 
    
    sdWrite( &SD3, (uint8_t *) data, strlen( data ) );  
    
    // Board is ready
    palClearPad( GPIOC, GPIOC_LED ); 
    
    /*
     * Normal main() thread activity, in this demo it does nothing except
     * sleeping in a loop and listen for events.
     */
    while( true )
    {
        // Wait start button
        if ( palReadPad( GPIOA, GPIOA_BUTTON_WKUP ) != 0 && running == false ) 
        {
            // Start game timer
            chVTSet( &gameTimer, MS2ST( 1000 ), runGame, NULL );
            
            /*
             * Creates the blinker thread.
             */ 
            tp = chThdCreateStatic( waThread1,
                                    sizeof( waThread1 ),
                                    NORMALPRIO,
                                    Thread1,
                                    NULL );
           
            chThdCreateStatic( waThread2,
                               sizeof( waThread2 ),
                               NORMALPRIO,
                               Thread2,
                               NULL );
            
            running = true;
        }
         
        chThdSleepMilliseconds( 100 ); /* Iddle thread */
    }
}
