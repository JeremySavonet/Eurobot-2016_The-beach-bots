/*
 * Start module: Here is the fsm of the robot
 * Author: Jeremy S.
 * Date: 2015-10-03
 * Rev: 1.0
 */

// Platform includes
#include "ch.h"
#include "hal.h"

#include "color.h"
#include "comm/debug_manager.h"

#include "modules/robot/trajectory_manager/trajectory_manager_core.h"
#include "modules/robot/trajectory_manager/trajectory_manager_utils.h"

#include "system.h"

#include "strat.h"
#include "strat_high_level.h"

strat_info_t strat_info;

static int game_tick = 0;
static bool running = false;

static THD_WORKING_AREA( wa_game_timer, 128 );
static THD_FUNCTION( GameTimer, arg );

static THD_WORKING_AREA( wa_strat_executor, 512 );
static THD_FUNCTION( StratExecutor, arg );

void do_strat( void )
{
    static thread_t *timer = NULL;

    bd_set_thresholds( &sys.controls.robot.distance_bd, 6000, 1 );

    // Init arms position
    setup_arm_position();

    strat_set_speed( FAST );

    // Check color
    if( palReadPad( GPIOD, 1 ) == 0 )
    {
        strat_info.color = YELLOW;
    }
    else
    {
        strat_info.color = RED;
    }

    // Wait start button
    DPRINT( 1, KBLU "Wait starter pull\r\n" );
    while ( palReadPad( GPIOA, GPIOA_BUTTON_WKUP ) == 0 );

    DPRINT( 1, KBLU "Start\r\n" );

    // Start the timer
    if( !timer )
    {
        timer = chThdCreateStatic( wa_game_timer,
                                   sizeof( wa_game_timer ),
                                   NORMALPRIO,
                                   GameTimer,
                                   NULL );
    }

    DPRINT( 1, KNRM "Start color => %d\r\n", strat_info.color );

    trajectory_goto_forward_xy_abs( &sys.controls.robot.traj,
                                    690,
                                    COLOR_Y( 1050 ) );

    while( position_get_x_float( &sys.controls.robot.pos ) < 500 && running )
    {
        test_traj_end( TRAJ_FLAGS_STD );
        trajectory_goto_forward_xy_abs( &sys.controls.robot.traj,
                                        690,
                                        COLOR_Y( 1050 ) );
    }

    while( running );

    trajectory_hardstop( &sys.controls.robot.traj );
    DPRINT( 1, KBLU "HARD STOP\r\n" );

    // If we reach this, then the match is finished. Reset the timer thread
    if( NULL != timer )
    {
        chThdRelease( timer );
        timer = NULL;
    }
}

void strat_begin( void )
{
    // start game strat
    if( !running )
    {
        sys.sys_state = SYSTEM_GAME_STARTED;

        running = true;
        strat_timer_stop(); // reset the timer
        // Start the strat thread : avoid blocking the main loop and the cli
        chThdCreateStatic( wa_strat_executor,
                           sizeof( wa_strat_executor ),
                           NORMALPRIO,
                           StratExecutor,
                           NULL );
    }
    else
    {
        DPRINT( 1, KRED "Game already running\r\n" );
    }
}

// Strat job thread
static THD_FUNCTION( StratExecutor, arg )
{
    (void)arg;
    chRegSetThreadName( "strat" );
    do_strat();
}

// Timer thread
static THD_FUNCTION( GameTimer, arg )
{
    (void)arg;
    chRegSetThreadName( "timer" );

    systime_t time = chVTGetSystemTimeX();

    while( ++game_tick < GAME_RUNNING_TIME )
    {
        time += MS2ST( 1000 ); // Next deadline in 1s
        chThdSleepUntil( time );
    }

    running = false;
    game_tick = 0;
    
    sys.sys_state = SYSTEM_RUNNING;

    DPRINT( 1, KBLU "Stop\r\n" );
}

int strat_get_time( void )
{
    return game_tick;
}

void strat_timer_stop( void )
{
    if( 0 != game_tick )
    {
        game_tick = GAME_RUNNING_TIME;
    }
}

//============================================================================//
//================================== UTILS ===================================//
//============================================================================//
int test_traj_end( int why )
{
    if( ( why & END_TRAJ ) && trajectory_finished( &sys.controls.robot.traj ) )
    {
        return END_TRAJ;
    }
    if( why & END_NEAR )
    {
        int16_t d_near = 100; // mm
        /* XXX Change distance depending on speed. */
        if( trajectory_in_window( &sys.controls.robot.traj,
                                  d_near,
                                  RAD( 5.0 ) ) )
        {
            return END_NEAR;
        }
    }
    if( ( why & END_BLOCKING ) && bd_get( &sys.controls.robot.distance_bd ) )
    {
        trajectory_hardstop( &sys.controls.robot.traj );
        bd_reset( &sys.controls.robot.distance_bd );
        bd_reset( &sys.controls.robot.angle_bd );
        return END_BLOCKING;
    }
    if( ( why & END_BLOCKING ) && bd_get( &sys.controls.robot.angle_bd ) )
    {
        trajectory_hardstop( &sys.controls.robot.traj );
        bd_reset( &sys.controls.robot.distance_bd );
        bd_reset( &sys.controls.robot.angle_bd );
        return END_BLOCKING;
    }

    if( ( why & END_TIMER ) && strat_get_time() >= GAME_RUNNING_TIME )
    {
        trajectory_hardstop( &sys.controls.robot.traj );
        return END_TIMER;
    }

    return 0;
}

int wait_traj_end_debug( int why, char *file, int line )
{
    int ret;
    do
    {
        ret = test_traj_end( why );
    } while( ret == 0 );

    DPRINT( 1, "%s:%d got %d", file, line, ret );

    return ret;
}

void strat_wait_ms( int ms )
{
    chThdSleepMilliseconds( ms );
}

void strat_set_speed( enum speed_e speed )
{
    int speed_d, speed_a;
    int acc_d, acc_a;

    switch( speed )
    {
        case SLOW:
            speed_d = 400;
            acc_d = 500;
            acc_a = 10;
            speed_a = 10;
            break;

        case FUCKING_SLOW:
            speed_d = 400;
            acc_d = 50;
            acc_a = 1;
            speed_a = 10;
            break;

        case FAST:
        default:
            speed_d = 800;
            acc_d = 1000;
            acc_a = 20;
            speed_a = 20;
            break;
    }

    trajectory_set_acc( &sys.controls.robot.traj,
                        acc_mm2imp( &sys.controls.robot.traj, acc_d ),
                        acc_rd2imp( &sys.controls.robot.traj, acc_a ) );

    trajectory_set_speed( &sys.controls.robot.traj,
                          speed_mm2imp(&sys.controls.robot.traj, speed_d),
                          speed_rd2imp(&sys.controls.robot.traj, speed_a) );
}
