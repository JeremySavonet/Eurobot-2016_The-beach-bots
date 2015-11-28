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

int get_game_elapsed_time( void )
{
    return game_tick;
}

void strat_stop( void )
{
    if( 0 != game_tick )
    {
        game_tick = GAME_RUNNING_TIME;
    }
}
