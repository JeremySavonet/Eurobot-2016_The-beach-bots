/*
 * Base software for non-holonome robot
 *
 * Description:
 * All the stuff related to make a non-holonome robot working.
 * See: system.c for board management
 * See: versatile_cs.c for robot control system stuff
 *
 * Added support for:
 * 90s timer to stop all the thread
 * Uart
 * UEXT reader thread
 * PWM on port D pin 12/13/14/15
 * ESP8266 API
 * CLI on USB2 OTG
 * MRF Zigbee chip
 *
 * Author: Jeremy S.
 * Date: 2015-10-03
 * Version: V1.0
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "system.h"

#include "versatile_cs.h"

#include "strat.h"

#include "color.h"
#include "comm/debug_manager.h"
#include "comm/microshell.h"

/*===========================================================================*/
/* Defines                                                                   */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE( 2048 )

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

        // TODO: Wait for call to start over CLI or add a button
        // ( this one is not the starter pull)

        chThdSleepMilliseconds( 100 ); // Iddle thread
    }
}