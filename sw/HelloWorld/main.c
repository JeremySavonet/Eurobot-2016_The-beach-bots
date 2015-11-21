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

#include "modules/sensors/infrared.h" //for command : move this in system
#include "modules/motor_manager.h"
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
static thread_t *tp = NULL;
static virtual_timer_t game_timer;

/*===========================================================================*/
/* User commands for CLI                                                     */
/*===========================================================================*/
ShellCommand user_commands[] = {

    { "ir", cmd_print_ir_distance },
    { NULL, NULL }
};

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

// Green LED blinker thread, times are in milliseconds.
static THD_WORKING_AREA( wa_alive, 128 );
static THD_FUNCTION( Alive, arg )
{
    (void)arg;
    chRegSetThreadName( "alive" );
    while( true )
    {
        DPRINT( 3, KBLU "Running...\r\n" );

        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );

        // Wait 90s and stop all the thread properly
        if( game_tick >= 5 )
        {
            msg_t msg = NULL;
            chThdExit( msg );
        }
    }
}

// Killer thread : wait for all sigterm signals
static THD_WORKING_AREA( wa_killer, 128 );
static THD_FUNCTION( Killer, arg )
{
    (void)arg;
    chRegSetThreadName( "killer" );

    // Wait here all thread to terminate properly
    chThdWait( tp );

    // Stop trajectory manager => stop the robot
    // Probably we will need to stop cs qnd odometry thread...
    trajectory_hardstop( &robot.traj );
 
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

    //init all managers
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

            // Creates the blinker thread.
            tp = chThdCreateStatic( wa_alive,
                                    sizeof( wa_alive ),
                                    NORMALPRIO,
                                    Alive,
                                    NULL );

            // Set motors speed to 50% duty cycle
            pwmcnt_t speedM4 = 5000;
            motor_set_speed( 0, speedM4 );
            motor_set_speed( 1, speedM4 );
            motor_set_speed( 2, speedM4 );
            motor_set_speed( 3, speedM4 );

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
