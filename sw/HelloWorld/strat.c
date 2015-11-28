/*
 * Start module: Here is the fsm of the robot
 * Author: Jeremy S.
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

static int game_tick = 0;
static bool running = false;

static THD_WORKING_AREA( wa_strat, 128 );
static THD_FUNCTION( Strat, arg );

void do_job( void )
{
    DPRINT( 1, KBLU "Running...\r\n" ); 
}

void strat_begin( void  )
{
    // start game strat 
    if( !running )
    {
        running = true;
        DPRINT( 1, KBLU "Start\r\n" ); 
        chThdCreateStatic( wa_strat,
                           sizeof( wa_strat ),
                           NORMALPRIO,
                           Strat,
                           NULL );
    }
    else
    {
        DPRINT( 1, KRED "Game already running\r\n" );
    }
}

// Strat thread
static THD_FUNCTION( Strat, arg )
{
    (void)arg;
    chRegSetThreadName( "strat" );
    
    systime_t time = chVTGetSystemTimeX();   
 
    while( ++game_tick < GAME_RUNNING_TIME ) 
    {
        time += MS2ST( 1000); // Next deadline
        do_job();
        chThdSleepUntil( time );
    }
    
    running = false;
    game_tick = 0;
    
    DPRINT( 1, KBLU "Stop\r\n" ); 
}

int strat_get_time( void )
{
    return game_tick;
}

void strat_timer_reset( void )
{
    if( 0 != game_tick )
    {
        game_tick = GAME_RUNNING_TIME;
    }
}

//============================================================================//
//================================== UTILS ===================================//
//============================================================================//

void strat_autopos( int16_t x, int16_t y, int16_t a, int16_t epaisseurRobot )
{
    //TODO
}

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
        if( trajectory_in_window( &sys.controls.robot.traj, d_near, RAD( 5.0 ) ) )
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
    //TODO
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
