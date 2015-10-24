/*
 * Simple demo program
 *
 * Description: 
 * A simple HelloWorld program that create a task that blinks a led
 *
 * Added support for:
 * 90s timer to stop all the thread
 * Uart 
 * UEXT reader thread
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

#define USART_CR1_9BIT_WORD     (1 << 12)   /* CR1 9 bit word */
#define USART_CR1_PARITY_SET    (1 << 10)   /* CR1 parity bit enable */
#define USART_CR1_EVEN_PARITY   (0 << 9)    /* CR1 even parity */

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
    115200,              
    0,
    0,
    0
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

/* Periodic thread for reading data */
static THD_WORKING_AREA( waRead, 128 );
static THD_FUNCTION( Thread3, arg )
{
    (void)arg;
    chRegSetThreadName( "uartModwifi" );

    while( true )
    {
        /* This will wait for a character to be received */
        char c;
        c = sdGet( &SD6 );
        sdPut( &SD3, c );
    }

    /*event_listener_t s6EventListener;*/
    /*chEvtRegisterMask((event_source_t *)chnGetEventSource(&SD6), &s6EventListener, EVENT_MASK(1));*/

    /*while( true )*/
    /*{*/
    /*eventflags_t flags;*/
    /*chEvtWaitOneTimeout(EVENT_MASK(1), MS2ST(10));*/
    /*chSysLock();*/
    /*flags = chEvtGetAndClearFlags(&s6EventListener);    */
    /*chSysUnlock(); */
    /*uint8_t c = sdGet( &SD6 );*/
    /*sdPut( &SD3, c ); */
    /*if (flags & CHN_INPUT_AVAILABLE)*/
    /*{*/
    /**//* Data available read here.*/
    /*uint8_t c = sdGet( &SD6 );*/
    /*sdPut( &SD3, c );*/
    /*}*/
    /*if (flags & CHN_OUTPUT_EMPTY) {*/
    /**//* Data sent, you may transmit from here.*/
    /*}*/
    /*if (flags & (SD_PARITY_ERROR | SD_FRAMING_ERROR | SD_OVERRUN_ERROR |*/
    /*SD_NOISE_ERROR | SD_BREAK_DETECTED)) {*/
    /**//* Some receive error happened.*/
    /*}*/
    /*}*/
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
  
    // Used function : USART6_TX 
    palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE( 8 ) ); 
    
    // Used function : USART6_RX 
    palSetPadMode( GPIOC, 7, PAL_MODE_ALTERNATE( 8 ) ); 
 
    // UART6 READER THREAD 
    chThdCreateStatic( waRead, sizeof(waRead), NORMALPRIO, Thread3, NULL );
 
    // Starts the serial driver with uartCfg as a config 
    sdStart( &SD3, &uartCfg ); 
    sdStart( &SD6, &uartCfg );

    const char data[] = "Ready..."; 

    // Board is ready
    palClearPad( GPIOC, GPIOC_LED ); 
    sdWrite( &SD3, (uint8_t *) data, strlen( data ) );
    
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
