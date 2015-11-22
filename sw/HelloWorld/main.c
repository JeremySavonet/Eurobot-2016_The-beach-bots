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
#include "hal.h"
#include "test.h"

#include "system.h"

#include "versatile_cs.h"

#include "color.h"
#include "comm/debug_manager.h"
#include "comm/microshell.h"

// Needed by thread declared here but move this
//#include "modules/motor_manager.h"
#include "modules/robot/trajectory_manager/trajectory_manager_core.h"

/*===========================================================================*/
/* Defines                                                                   */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE( 2048 )

/*===========================================================================*/
/* Global static functions                                                   */
/*===========================================================================*/

static void run_game( void *p );

/*===========================================================================*/
/* Global static variables                                                   */
/*===========================================================================*/

static bool running = false;
static int game_tick = 0;
static virtual_timer_t game_timer;

/*===========================================================================*/
/* Application threads                                                       */
/*===========================================================================*/

//Shell thread
static THD_WORKING_AREA( wa_cli, 2048 );
static THD_FUNCTION( CLI, arg )
{
    (void) arg;
    chRegSetThreadName( "CLI" );
    start_shell();
}

// Killer thread : wait for all sigterm signals
static THD_WORKING_AREA( wa_killer, 128 );
static THD_FUNCTION( Killer, arg )
{
    (void)arg;
    chRegSetThreadName( "killer" );

    //// Wait here all thread to terminate properly
    //chThdWait( tp );

    // Stop trajectory manager => stop the robot
    // Probably we will need to stop cs qnd odometry thread...
    trajectory_hardstop( &sys.controls.robot.traj );
 
    DPRINT( 1, "Stop...\r\n" );
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

    // Init system
    system_init();
        
    // Global main loop
    while( true )
    {
        if( !shelltp && ( SDU2.config->usbp->state == USB_ACTIVE ) )
        {
            shelltp = chThdCreateStatic( wa_cli,
                                         sizeof( wa_cli ),
                                         NORMALPRIO,
                                         CLI,
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
            chVTSet( &game_timer, MS2ST( 1000 ), run_game, NULL );

            // Init strat here
            // strat_init();

            // Start killer thread
            chThdCreateStatic( wa_killer,
                               sizeof( wa_killer ),
                               NORMALPRIO,
                               Killer,
                               NULL );
            running = true;
        }
        chThdSleepMilliseconds( 100 ); /* Iddle thread */
    }
}

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

// Game running loop
void run_game( void *p )
{
    // Restarts the timer
    chSysLockFromISR();
    chVTSetI( &game_timer, MS2ST( 1000 ), run_game, p );
    chSysUnlockFromISR();

    if( ++game_tick < 90 ) {}
}
