/*
 * This module contains the set of commands manage by our CLI.
 * Author: Jeremy S.
 * Rev: 1.0
 */

#include <math.h>
#include <string.h>

// for system structure
#include "system.h" 

// for specifics commands or actions
#include "../modules/motor/motor_manager.h"
#include "../modules/robot/trajectory_manager/trajectory_manager_core.h"

// for strat
#include "../strat.h"

#include "commands.h"

/*===========================================================================*/
/* User commands for CLI                                                     */
/*===========================================================================*/
ShellCommand user_commands[] = {

    { "mode", cmd_set_robot_mode },
    { "position", cmd_robot_position },
    { "encoder", cmd_get_encoder },
    { "pwm", cmd_set_pwm },
    { "god", cmd_go_forward },
    { "goa", cmd_turn_angle },
    { "pida", cmd_set_or_get_pida_gains },
    { "pidd", cmd_set_or_get_pidd_gains },

    { "start", cmd_start_strat },
    { "stop", cmd_stop_strat },
    { "uptime", cmd_get_uptime },
    { "game_elapsed", cmd_get_game_elapsed_time },
    { "ir", cmd_print_ir_distance },
    { "temp", cmd_print_temp },
    { "batt_level", cmd_print_battery_level },
    { NULL, NULL }
};

void cmd_start_strat( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: start\r\n" );
        return;
    }
    
    strat_begin();
}

void cmd_stop_strat( int argc, char *argv[] ) 
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: stop\r\n" );
        return;
    }
    
    strat_timer_stop(); 
}

void cmd_get_uptime( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: uptime\r\n" );
        return;
    }
    int now = ST2MS( chVTGetSystemTimeX() );  
    chprint( "System uptime: %ds\r\n", now / 1000 );
}

void cmd_get_game_elapsed_time( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: game_elapsed\r\n" );
        return;
    }
    
    int running_time = strat_get_time();
    if( running_time != 0 )
    {
        chprint( "End game in %ds\r\n", 
            GAME_RUNNING_TIME - strat_get_time() ); 
    }
    else
    {
        chprint( "Game not started\r\n" );
    }
}

void cmd_set_robot_mode( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc != 1 )
    {
        chprint( "Usage: mode args ( angle distance free all pwm )\r\n" );
        return;
    }

    if( !strcmp( "angle", argv[0] ) )
    {
        sys.controls.robot.mode = BOARD_MODE_ANGLE_ONLY;
    }
    else if( !strcmp( "distance", argv[0] ) )
    {
        sys.controls.robot.mode = BOARD_MODE_DISTANCE_ONLY;
    }
    else if( !strcmp( "free", argv[0] ) )
    {
        sys.controls.robot.mode = BOARD_MODE_FREE;
    }
    else if( !strcmp( "all", argv[0]  ) )
    {
        sys.controls.robot.mode = BOARD_MODE_ANGLE_DISTANCE;
    }
    else if( !strcmp( "pwm", argv[0] ) )
    {
        sys.controls.robot.mode = BOARD_MODE_SET_PWM;
    }
    else
    {
        chprint( "Usage: mode args ( angle distance free all pwm )\r\n" );
        return;
    }
    
    // Ending the start to avoid problems
    trajectory_hardstop( &sys.controls.robot.traj );
}

void cmd_robot_position( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( 0 == argc )
    {
        chprint( "Position X: %d\r\n", 
            position_get_x_float( &sys.controls.robot.pos ) );
        chprint( "Position Y: %d\r\n", 
            position_get_y_float( &sys.controls.robot.pos ) );
        chprint( "Position A[rad]: %d\r\n", 
            position_get_a_rad_float( &sys.controls.robot.pos ) );
    }
    else if( 3 == argc )
    {
        float x, y, a;
        x = atof( argv[0] );
        y = atof( argv[1] );
        a = atof( argv[2] );

        position_set( &sys.controls.robot.pos, x, y, a );
    }
    else
    {
        chprint( "Usage: position (to get the actual position)\r\n" );
        chprint( "Usage: position x y a[deg] (to set the position)\r\n" );
    }
}

void cmd_get_encoder( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc != 1 )
    {
        chprint( "Usage: encoder channel\r\n" );
        return;
    }
   
    int channel = atoi( argv[0] );
    
    if( 0 == channel )
    {
        chprint( "Encoder %d: %d\r\n", channel, 
            versatile_dc_get_encoder( MOTOR_ENCODER_BASE_LEFT ) );
    }
    else if( 1 == channel )
    {
        chprint( "Encoder %d: %d\r\n", channel, 
            versatile_dc_get_encoder( MOTOR_ENCODER_BASE_RIGHT ) );
    }
    else
    {
        chprint( "Not a valid encoder channel\r\n" );
    }
}

void cmd_set_pwm( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    if( argc != 2 )
    {
        chprint( "Usage: pwm channel value\r\n" );
        return;
    }

    int channel = atoi( argv[0] );
    int32_t value = atoi( argv[1] );

    if( 0 == channel )
    {
        chprint( "Set pwm on channel %d to value: %d\r\n", channel, value );
        versatile_dc_set_pwm0( MOTOR_CONTROLLER_BASE, value );
    }
    else if( 1 == channel )
    {
        chprint( "Set pwm on channel %d to value: %d\r\n", channel, value );
        versatile_dc_set_pwm1( MOTOR_CONTROLLER_BASE, value );
    }
    else
    {
        chprint( "Not a valid motor channel\r\n" );
    }
}

void cmd_go_forward( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    if( argc != 1 )
    {
        chprint( "Usage: god distance\r\n" );
        return;
    }
    int distance = atoi( argv[0] );
    trajectory_d_rel( &sys.controls.robot.traj, distance );
    chprint( "Trajectory: go forward= %d\r\n", distance );
}

void cmd_turn_angle( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    if( argc != 1 )
    {
        chprint( "Usage: goa angle[deg]\r\n" );
        return;
    }
    int angle = atoi( argv[0] );
    trajectory_a_rel( &sys.controls.robot.traj, angle );
    chprint( "Trajectory: turn angle= %d\r\n", angle );
}

void cmd_set_or_get_pida_gains( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;
    
    if( argc == 0 )
    {
        chprint( "pida gains:\r\n" );
        chprint( "kp: %d\r\n", sys.controls.robot.angle_pid.gain_P );
        chprint( "ki: %d\r\n", sys.controls.robot.angle_pid.gain_I );
        chprint( "kd: %d\r\n", sys.controls.robot.angle_pid.gain_D );
    }
    else if( argc == 3 )
    {
        chprint( "Set pida gains..." );
        sys.controls.robot.angle_pid.gain_P = atoi( argv[0] );
        sys.controls.robot.angle_pid.gain_I = atoi( argv[1] );
        sys.controls.robot.angle_pid.gain_D = atoi( argv[2] );
        chprint( "Done\r\n" );
    }
    else
    {
        chprint( "Usage get: pida\r\n" );
        chprint( "Usage set: pida kp ki kd\r\n" );
        return;
    }
}

void cmd_set_or_get_pidd_gains( int argc, char* argv[] )
{
    (void)argc;
    (void)argv;

    if( argc == 0 )
    {
        chprint( "pidd gains:\r\n" );
        chprint( "kp: %d\r\n", sys.controls.robot.distance_pid.gain_P );
        chprint( "ki: %d\r\n", sys.controls.robot.distance_pid.gain_I );
        chprint( "kd: %d\r\n", sys.controls.robot.distance_pid.gain_D );
    }
    else if( argc == 3 )
    {
        chprint( "Set pidd gains..." );
        sys.controls.robot.distance_pid.gain_P = atoi( argv[0] );
        sys.controls.robot.distance_pid.gain_I = atoi( argv[1] );
        sys.controls.robot.distance_pid.gain_D = atoi( argv[2] );
        chprint( "Done\r\n" );
    }
    else
    {
        chprint( "Usage get: pidd\r\n" );
        chprint( "Usage set: pidd kp ki kd\r\n" );
        return;
    }
}

void cmd_print_ir_distance( int argc, char *argv[] ) 
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: ir\r\n" );
        return;
    }

    chprint( "Infrared distances: d0 = %.3f, d1 = %.3f,  d2 = %.3f\r\n", 
        sys.sensors.ir_sensors.ir_d0, 
        sys.sensors.ir_sensors.ir_d1,
        sys.sensors.ir_sensors.ir_d2 );
}

void cmd_print_temp( int argc, char *argv[] ) 
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: temp\r\n" );
        return;
    }
        
    chprint( "Temp: t = %.3f\r\n", sys.sensors.temp_sensors.temp0 ); 
}

void cmd_print_battery_level( int argc, char *argv[] ) 
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: batt_level\r\n" );
        return;
    }
        
    chprint( "Battery level: v = %.3f\r\n", sys.sensors.batt_sensor.batt_level ); 
}
