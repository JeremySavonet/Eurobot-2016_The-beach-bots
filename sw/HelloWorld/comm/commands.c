/*
 * This module contains the set of commands manage by our CLI.
 * Author: Jeremy S.
 * Rev: 1.0
 */

#include <string.h>

// for system structure
#include "system.h" 

// for specifics commands or actions
#include "../modules/motor_manager.h"
#include "../modules/robot/trajectory_manager/trajectory_manager_core.h"

#include "commands.h"

/*===========================================================================*/
/* User commands for CLI                                                     */
/*===========================================================================*/
ShellCommand user_commands[] = {

    { "mode", cmd_set_robot_mode },
    { "position", cmd_get_robot_position },
    { "encoder", cmd_get_encoder },
    { "pwm", cmd_set_pwm },
    { "start", cmd_start_asserv },
    { "stop", cmd_stop_asserv },
    { "ir", cmd_print_ir_distance },
    { "temp", cmd_print_temp },
    { "batt_level", cmd_print_battery_level },
    { NULL, NULL }
};

void cmd_start_asserv( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: start\r\n" );
        return;
    }
}

void cmd_stop_asserv( int argc, char *argv[] ) 
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: stop\r\n" );
        return;
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

void cmd_get_robot_position( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: position\r\n" );
        return;
    }
    chprint( "Position X: %d\r\n", position_get_x_float( &sys.controls.robot.pos ) );
    chprint( "Position Y: %d\r\n", position_get_y_float( &sys.controls.robot.pos ) );
    chprint( "Position A[rad]: %d\r\n", position_get_a_rad_float( &sys.controls.robot.pos ) );
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
