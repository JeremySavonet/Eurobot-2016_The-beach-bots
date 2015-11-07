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
#include "test.h"

#include "appl/console.h"
#include "appl/ESP8266.h"
#include "appl/motors.h"
#include "appl/usbConfig.h"
#include "appl/microrl.h"
#include "appl/microshell2.h"
//#include "test/noRegression.h"

/*===========================================================================*/
/* Defines                                                                   */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE( 2048 )

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

// Here the list of user commands (i.e.: all commands to access robot structure)
ShellCommand user_commands[] = {

    //TODO
    {NULL, NULL}
};

/*===========================================================================*/
/* Application threads                                                       */
/*===========================================================================*/

//Shell thread
static THD_WORKING_AREA( waShell, 2048 );
static THD_FUNCTION( Shell, arg )
{
    (void) arg;
    chRegSetThreadName("shell");
    start_shell();
}

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
            shelltp = chThdCreateStatic( waShell,
                                         sizeof( waShell ),
                                         NORMALPRIO,
                                         Shell,
                                         NULL );
        }
        else if( chThdTerminatedX( shelltp ) )
        {
            chThdRelease( shelltp );  // Recovers memory of the previous shell.
            shelltp = NULL;           // Triggers spawning of a new shell.
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
