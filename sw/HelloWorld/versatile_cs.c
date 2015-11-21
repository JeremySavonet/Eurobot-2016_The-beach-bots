/*
 * This files implements the control system loop wrappers.
 * The file provides all the implementation of the Control System Management (CSM).
 * A CSM is made of 3 parts : acceleration / deceleration ramp generator
 * (Quadramp), a position regulator (PID), and I/O interfaces (PWM & quadratures
 * encoders).
 * The quadramp tells the PID what position the wheel should be, depending on the
 * acceleration and time. This value is then fed as the consign value to the PID
 * regulator, with the encoder value as the measured position. The output of the
 * PID is then applied to the motor via the PWM.
 * The others functions computed here are the position manager, the trajectory
 * manager and the blocking detection system.
 */

#include <string.h>
#include <stdio.h>

#include "ch.h"
#include "chprintf.h"
#include "hal.h"

#include "color.h"

#include "comm/microshell.h"
#include "comm/debug_manager.h"

#include "modules/motor_manager.h"

#include "modules/robot/trajectory_manager/trajectory_manager_utils.h"

#include "versatile_cs.h"

#define   CS_TASK_PRIORITY            21
#define   ODOMETRY_TASK_PRIORITY      22

#define COMPILE_ON_ROBOT

//=================== VERSATILE CS/ODOMETRY TASKS DEFINTION ==================//

static THD_WORKING_AREA( wa_control_sys, 2048 );
static THD_WORKING_AREA( wa_odometry, 2048 );

static THD_FUNCTION( ControlSys, arg );
static THD_FUNCTION( Odometry, arg );

//============================================================================//

struct _rob robot;

void versatile_cs_init( void )
{
    robot.mode = BOARD_MODE_ANGLE_DISTANCE;

    /**************************************************************************/
    /*                                 Motors                                 */
    /**************************************************************************/
/*
#ifdef COMPILE_ON_ROBOT
    int i;
    for( i = 0; i < 8; i++ )
    {
        cvra_dc_set_encoder( HEXMOTORCONTROLLER_BASE, i, 0 );
        cvra_dc_set_pwm( HEXMOTORCONTROLLER_BASE, i, 0 );
    }
#endif
*/
    /**************************************************************************/
    /*                             Robot system                               */
    /**************************************************************************/
    rs_init( &robot.rs );
    rs_set_flags( &robot.rs, RS_USE_EXT );

    /*************************f************************************************/
    /*                         Encoders & PWMs                                */
    /**************************************************************************/

#ifdef COMPILE_ON_ROBOT
    rs_set_left_pwm( &robot.rs,
                     versatile_dc_set_pwm0,
                     MOTOR_CONTROLLER_BASE ); // MOTOR 0 on PWM9 channel 0

    rs_set_right_pwm( &robot.rs,
                      versatile_dc_set_pwm1,
                      MOTOR_CONTROLLER_BASE ); // MOTOR 1 on PWM9 channel 1

    rs_set_left_ext_encoder( &robot.rs,
                             versatile_dc_get_encoder,
                             MOTOR_ENCODER_BASE_LEFT,
                             1. ); // last arg = gain

    rs_set_right_ext_encoder( &robot.rs,
                              versatile_dc_get_encoder,
                              MOTOR_ENCODER_BASE_RIGHT,
                              -1. ); // last arg = gain
/*
   rs_set_left_mot_encoder( &robot.rs,
                             cvra_dc_get_encoder1,
                             HEXMOTORCONTROLLER_BASE,
                             -1. );

    rs_set_right_mot_encoder( &robot.rs,
                              cvra_dc_get_encoder2,
                              HEXMOTORCONTROLLER_BASE,
                              -1. );
*/
#endif

    /****************************************************************************/
    /*                          Position manager                                */
    /****************************************************************************/
    position_init( &robot.pos );

    // Links the position manager to the robot system.
    position_set_related_robot_system( &robot.pos, &robot.rs );

    position_set_physical_params( &robot.pos, 193.82313537598, 162.9746617261 );
    position_use_ext( &robot.pos );

    /****************************************************************************/
    /*                       Regulation de l'angle                              */
    /****************************************************************************/
    pid_init( &robot.angle_pid );
    pid_set_gains( &robot.angle_pid, 400, 0, 2000 );
    pid_set_maximums( &robot.angle_pid, 0, 5000, 30000 );
    pid_set_out_shift( &robot.angle_pid, 10 );

    quadramp_init( &robot.angle_qr );

    cs_init( &robot.angle_cs ); // Initialise le control system.
    cs_set_consign_filter( &robot.angle_cs,
                           quadramp_do_filter,
                           &robot.angle_qr ); // Met un filtre en acceleration.

    cs_set_correct_filter( &robot.angle_cs,
                           pid_do_filter,
                           &robot.angle_pid ); // Met le PID.

    cs_set_process_in( &robot.angle_cs,
                       rs_set_angle,
                       &robot.rs ); // Met la sortie sur le pwm virtuel de l'angle.

    cs_set_process_out( &robot.angle_cs,
                        rs_get_ext_angle,
                        &robot.rs ); // lecture codeur virtuel de l'angle

    cs_set_consign( &robot.angle_cs, 0 ); // Met une consigne nulle.

    /****************************************************************************/
    /*                      Regulation de la distance                           */
    /****************************************************************************/
    pid_init( &robot.distance_pid ); // Initialise le PID.
    pid_set_gains( &robot.distance_pid, 200, 0, 1000 ); // Regles les gains du PID.
    pid_set_maximums( &robot.distance_pid, 0, 5000, 30000 ); // pas de max sur l'entree, integral limite a 5000, sortie limitee a 4095 (PWM 12 bits).
    pid_set_out_shift( &robot.distance_pid, 10 ); // Divise la sortie par 1024.

    quadramp_init( &robot.distance_qr ); // Demarre le filtre en acceleration.

    cs_init( &robot.distance_cs);
    cs_set_consign_filter( &robot.distance_cs,
                           quadramp_do_filter,
                           &robot.distance_qr );

    cs_set_correct_filter( &robot.distance_cs,
                           pid_do_filter,
                           &robot.distance_pid );

    cs_set_process_in( &robot.distance_cs,
                       rs_set_distance,
                       &robot.rs ); // Met la sortie sur le pwm virtuel de la distance.

    cs_set_process_out( &robot.distance_cs,
                        rs_get_ext_distance,
                        &robot.rs ); // lecture codeur virtuel de la distance

    cs_set_consign( &robot.distance_cs, 0 ); //Met une consigne nulle.

    /****************************************************************************/
    /*                           Trajectory Manager (Trivial)                   */
    /****************************************************************************/
    trajectory_init( &robot.traj, ASSERV_FREQUENCY );
    trajectory_set_cs( &robot.traj, &robot.distance_cs, &robot.angle_cs );
    trajectory_set_robot_params( &robot.traj, &robot.rs, &robot.pos );

    // distance window, angle window, angle start
    trajectory_set_windows( &robot.traj, 15., 5.0, 10. ); // Prod

    // Angle BDM
    bd_init( &robot.angle_bd, &robot.angle_cs );
    bd_set_thresholds( &robot.angle_bd, 3000, 1 ); // thresold, duration.

    // Distance BDM
    bd_init( &robot.distance_bd, &robot.distance_cs );
    bd_set_thresholds( &robot.distance_bd, 3600, 1 ); // thresold, duration.

    robot.is_aligning = 0;

    // Initialisation deplacement:
    position_set( &robot.pos, 0, 0, 0 );

#if 1
    // Creates the control task.
    chThdCreateStatic( wa_control_sys,
                       sizeof( wa_control_sys ),
                       NORMALPRIO, //CS_TASK_PRIO
                       ControlSys,
                       NULL );

    // Creates the control task.
    chThdCreateStatic( wa_odometry,
                   sizeof( wa_odometry ),
                   NORMALPRIO, //ODOMETRY_TASK_PRIO
                   Odometry,
                   NULL );
#endif
}

//========================= VERSATILE CS/ODOMETRY TASKS ======================//

// Versatile CS manager thread, times are in milliseconds.
THD_FUNCTION( ControlSys, arg )
{
    (void)arg;
    chRegSetThreadName( "versatile cs manage\r\n" );
    while( true )
    {
        rs_update( &robot.rs );

        // Gestion de l'asservissement.
        if( robot.mode != BOARD_MODE_SET_PWM )
        {
            if( robot.mode == BOARD_MODE_ANGLE_DISTANCE ||
                robot.mode == BOARD_MODE_ANGLE_ONLY )
            {
                cs_manage( &robot.angle_cs );
            }
            else
            {
                rs_set_angle( &robot.rs, 0 ); // Sets a null angle PWM
            }

            if( robot.mode == BOARD_MODE_ANGLE_DISTANCE ||
                robot.mode == BOARD_MODE_DISTANCE_ONLY )
            {
                cs_manage( &robot.distance_cs );
            }
            else
            {
                rs_set_distance( &robot.rs, 0 ); // Sets a distance angle PWM
            }
        }

        // Gestion du blocage
        bd_manage( &robot.angle_bd );
        bd_manage( &robot.distance_bd );

        // Wait 10 milliseconds (100 Hz)
        chThdSleepMilliseconds( 10 );
    }
}

// Odometry manager thread, times are in milliseconds.
THD_FUNCTION( Odometry, arg )
{
    (void)arg;
    while( true )
    {
        position_manage(&robot.pos);

        // Wait 20 milliseconds (50 Hz)
        chThdSleepMilliseconds( 20 );
    }
}

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

void cmd_set_robot_mode_angle( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: angle\r\n" );
        return;
    }
    
    robot.mode = BOARD_MODE_ANGLE_ONLY; 
}

void cmd_set_robot_mode_distance( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: distance\r\n" );
        return;
    }
    
    robot.mode = BOARD_MODE_DISTANCE_ONLY; 
}

void cmd_set_robot_mode_free( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: free\r\n" );
        return;
    }
    robot.mode = BOARD_MODE_FREE; 
}

void cmd_set_robot_mode_all( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: all\r\n" );
        return;
    }
    robot.mode = BOARD_MODE_ANGLE_DISTANCE; 
}

void cmd_set_robot_mode_pwm( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    if( argc > 0 )
    {
        chprint( "Usage: mode_pwm\r\n" );
        return;
    }
    robot.mode = BOARD_MODE_SET_PWM; 
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
    chprint( "Position X: %d\r\n", position_get_x_float( &robot.pos ) );
    chprint( "Position Y: %d\r\n", position_get_y_float( &robot.pos ) );
    chprint( "Position A[rad]: %d\r\n", position_get_a_rad_float( &robot.pos ) );
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
