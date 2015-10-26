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
 * PWM on port D pin 12/13/14/15
 * ESP8266 API
 * CLI on USB2 OTG
 *
 * Author: JS
 * Date: 2015-10-03
 * Version: V0.1
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "chprintf.h"
#include "hal.h"
#include "shell.h"
#include "test.h"

#include "appl/console.h"
#include "appl/ESP8266.h"
#include "appl/motors.h"
#include "appl/usbConfig.h"
//#include "test/noRegression.h"

/*===========================================================================*/
/* Defines                                                                   */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE( 2048 )
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE( 256 )

/*===========================================================================*/
/* Global static functions                                                   */
/*===========================================================================*/

static void debugInit( void );
static void runGame( void *p );

/*===========================================================================*/
/* Global static variables                                                   */
/*===========================================================================*/

static bool running = false;
static int gameTick = 0;
static thread_t *tp = NULL;
static virtual_timer_t gameTimer;

/*===========================================================================*/
/* CLI functions here                                                        */
/* TODO : Nice to have little macro to add new cli function                  */
/*===========================================================================*/

static void cmd_mem( BaseSequentialStream *chp, int argc, char *argv[] )
{
    size_t n, size;

    (void)argv;
    if( argc > 0 )
    {
        chprintf( chp, "Usage: mem\r\n" );
        return;
    }

    n = chHeapStatus( NULL, &size );
    chprintf( chp, "core free memory : %u bytes\r\n", chCoreGetStatusX() );
    chprintf( chp, "heap fragments   : %u\r\n", n );
    chprintf( chp, "heap free total  : %u bytes\r\n", size );
}

static void cmd_threads( BaseSequentialStream *chp, int argc, char *argv[] )
{
    static const char *states[] = { CH_STATE_NAMES };
    thread_t *tp;

    (void)argv;
    if( argc > 0 )
    {
        chprintf( chp, "Usage: threads\r\n" );
        return;
    }

    chprintf( chp, "    addr    stack prio refs     state time\r\n" );
    tp = chRegFirstThread();
    do
    {
        chprintf( chp, "%08lx %08lx %4lu %4lu %9s\r\n",
                  (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                  (uint32_t)tp->p_prio, (uint32_t)( tp->p_refs - 1 ),
                  states[tp->p_state]);

        tp = chRegNextThread( tp );
    } while( tp != NULL );
}

static void cmd_benchmark( BaseSequentialStream *chp, int argc, char *argv[] )
{
    thread_t *tp;

    (void)argv;
    if( argc > 0 )
    {
        chprintf( chp, "Usage: benchmark\r\n" );
        return;
    }

    tp = chThdCreateFromHeap( NULL,
                              TEST_WA_SIZE,
                              chThdGetPriorityX(),
                              TestThread,
                              chp );
    if( tp == NULL )
    {
        chprintf( chp, "out of memory\r\n" );
        return;
    }

    chThdWait( tp );
}

static void cmd_tests( BaseSequentialStream *chp, int argc, char *argv[] )
{
    (void)argv;
    if( argc > 0 )
    {
        chprintf( chp, "Usage: tests\r\n" );
        return;
    }

    chprintf( chp, "Running\r\n" );
}

/*===========================================================================*/
/* Global structures                                                         */
/*===========================================================================*/

// Struct to config serial module for debug
static SerialConfig uartCfg =
{
    115200,
    0,
    0,
    0
};

static const ShellCommand commands[] =
{
    { "mem",       cmd_mem },
    { "threads",   cmd_threads },
    { "benchmark", cmd_benchmark },
    { "tests",     cmd_tests },
    { NULL, NULL }
};

static const ShellConfig shellCfg1 =
{
    (BaseSequentialStream *)&SDU2,
    commands
};

/*===========================================================================*/
/* Application threads                                                       */
/*===========================================================================*/

// Green LED blinker thread, times are in milliseconds.
static THD_WORKING_AREA( waThread1, 128 );
static THD_FUNCTION( Thread1, arg )
{
    (void)arg;
    chRegSetThreadName( "blinker" );
    while( true )
    {
        DPRINT( 3, "Running..." );

        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );

        // Wait 90s and stop all the thread properly
        if( gameTick >= 5 )
        {
            msg_t msg = NULL;
            chThdExit( msg );
        }
    }
}

// Killer thread : wait for all sigterm signals
static THD_WORKING_AREA( waThread2, 128 );
static THD_FUNCTION( Thread2, arg )
{
    (void)arg;
    chRegSetThreadName( "killer" );

    // Wait here all thread to terminate properly
    chThdWait( tp );

    MotorSetSpeed( 0, 0 );
    MotorSetSpeed( 1, 0 );
    MotorSetSpeed( 2, 0 );
    MotorSetSpeed( 3, 0 );
    DPRINT( 1, "Stop..." );
    palSetPad( GPIOC, GPIOC_LED );
}

int main( void )
{
    static thread_t *shelltp = NULL;

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();

    // Init IOs
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );

    // Init CLI
    UsbInit();
    shellInit();

    // Init debug UART
    debugInit();

    // Init ESP8266 WiFi module
    ESP8266Init();

    // Init Motors module
    MotorsInit();

    // Init done => Board ready
    palClearPad( GPIOC, GPIOC_LED );
    DPRINT( 1, "Ready..." );

    ESP8266RequestVersion();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    ESP8266SetMode();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    ESP8266EnableMultipleConnection();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    ESP8266ConfigureServer();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    ESP8266SetAccessPoint();
    chThdSleepMilliseconds( 1000 ); /* Iddle thread */

    ESP8266JoinAccessPoint();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    // Global main loop
    while( true )
    {
        if( !shelltp && ( SDU2.config->usbp->state == USB_ACTIVE ) )
        {
            shelltp = shellCreate( &shellCfg1, SHELL_WA_SIZE, NORMALPRIO );
        }
        else if( chThdTerminatedX( shelltp ) )
        {
            chThdRelease( shelltp );  /* Recovers memory of the previous shell.   */
            shelltp = NULL;           /* Triggers spawning of a new shell.        */
        }

        // Wait start button
        if ( palReadPad( GPIOA, GPIOA_BUTTON_WKUP ) != 0 && running == false )
        {
            // Start game timer
            chVTSet( &gameTimer, MS2ST( 1000 ), runGame, NULL );

            // Creates the blinker thread.
            tp = chThdCreateStatic( waThread1,
                                    sizeof( waThread1 ),
                                    NORMALPRIO,
                                    Thread1,
                                    NULL );

            // Set motors speed to 50% duty cycle
            pwmcnt_t speedM4 = 5000;
            MotorSetSpeed( 0, speedM4 );
            MotorSetSpeed( 1, speedM4 );
            MotorSetSpeed( 2, speedM4 );
            MotorSetSpeed( 3, speedM4 );

            // Start killer thread
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

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

void debugInit( void )
{
    // Configure UART3 for debug 115200 8N1
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) );
    sdStart( &SD3, &uartCfg );
}

// Game running loop
void runGame( void *p )
{
    // Restarts the timer
    chSysLockFromISR();
    chVTSetI( &gameTimer, MS2ST( 1000 ), runGame, p );
    chSysUnlockFromISR();

    if( ++gameTick < 90 ) {}
}
