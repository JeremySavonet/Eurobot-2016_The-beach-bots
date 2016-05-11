/*
 * This files implements the control system loop wrappers.
 * The file provides all the implementation of the
 * Control System Management (CSM).
 * A CSM is made of 3 parts : acceleration / deceleration ramp generator
 * (Quadramp), a position regulator (PID), and I/O interfaces (PWM & quadratures
 * encoders).
 * The quadramp tells the PID what position the wheel should be,
 * depending on the acceleration and time.
 * This value is then fed as the consign value to the PID regulator,
 * with the encoder value as the measured position. The output of the PID is
 * then applied to the motor via the PWM.
 * The others functions computed here are the position manager, the trajectory
 * manager and the blocking detection system.
 */

#include "ch.h"
#include "hal.h"

#include "color.h"
#include "comm/debug_manager.h"

#include "modules/motor_manager.h"

#include "modules/robot/trajectory_manager/trajectory_manager_utils.h"

#include "system.h" //for system structure

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

void versatile_cs_init( versatile_robot_t * robot )
{
    robot->mode = BOARD_MODE_ANGLE_DISTANCE;

    /**************************************************************************/
    /*                                 Motors                                 */
    /**************************************************************************/
#ifdef COMPILE_ON_ROBOT
    // Init motors and QEI first to avoid logic level issues
    // on motors at startup
    motor_manager_init();

    qei_manager_init();
#endif

    /**************************************************************************/
    /*                             Robot system                               */
    /**************************************************************************/
    rs_init( &robot->rs );
    rs_set_flags( &robot->rs, RS_USE_EXT );

    /*************************f************************************************/
    /*                         Encoders & PWMs                                */
    /**************************************************************************/

#ifdef COMPILE_ON_ROBOT
    rs_set_left_pwm( &robot->rs,
                     versatile_dc_set_pwm0,
                     MOTOR_CONTROLLER_BASE ); // MOTOR 0 on PWM9 channel 0

    rs_set_right_pwm( &robot->rs,
                      versatile_dc_set_pwm1,
                      MOTOR_CONTROLLER_BASE ); // MOTOR 1 on PWM9 channel 1

    rs_set_left_ext_encoder( &robot->rs,
                             versatile_dc_get_encoder,
                             MOTOR_ENCODER_BASE_LEFT,
                             1. ); // last arg = gain

    rs_set_right_ext_encoder( &robot->rs,
                              versatile_dc_get_encoder,
                              MOTOR_ENCODER_BASE_RIGHT,
                              -1. ); // last arg = gain
#endif

    /**************************************************************************/
    /*                          Position manager                              */
    /**************************************************************************/
    position_init( &robot->pos );

    // Links the position manager to the robot system.
    position_set_related_robot_system( &robot->pos, &robot->rs );

    position_set_physical_params( &robot->pos,
                                  193.82313537598,
                                  162.9746617261 );
    position_use_ext( &robot->pos );

    /**************************************************************************/
    /*                       Regulation de l'angle                            */
    /**************************************************************************/
    pid_init( &robot->angle_pid );
    pid_set_gains( &robot->angle_pid, 400, 0, 2000 );
    pid_set_maximums( &robot->angle_pid, 0, 5000, 30000 );
    pid_set_out_shift( &robot->angle_pid, 10 );

    quadramp_init( &robot->angle_qr );

    cs_init( &robot->angle_cs ); // Initialise le control system.
    cs_set_consign_filter( &robot->angle_cs,
                           quadramp_do_filter,
                           &robot->angle_qr ); // Met un filtre en acceleration.

    cs_set_correct_filter( &robot->angle_cs,
                           pid_do_filter,
                           &robot->angle_pid ); // Met le PID.

    cs_set_process_in( &robot->angle_cs,
                       rs_set_angle,
                       &robot->rs ); // Met la sortie sur le pwm virtuel de l'angle.

    cs_set_process_out( &robot->angle_cs,
                        rs_get_ext_angle,
                        &robot->rs ); // lecture codeur virtuel de l'angle

    cs_set_consign( &robot->angle_cs, 0 ); // Met une consigne nulle.

    /**************************************************************************/
    /*                      Regulation de la distance                         */
    /**************************************************************************/
    pid_init( &robot->distance_pid ); // Initialise le PID.
    pid_set_gains( &robot->distance_pid, 200, 0, 1000 ); // Regles les gains du PID.
    pid_set_maximums( &robot->distance_pid, 0, 5000, 30000 ); // pas de max sur l'entree, integral limite a 5000, sortie limitee a 4095 (PWM 12 bits).
    pid_set_out_shift( &robot->distance_pid, 10 ); // Divise la sortie par 1024.

    quadramp_init( &robot->distance_qr ); // Demarre le filtre en acceleration.

    cs_init( &robot->distance_cs);
    cs_set_consign_filter( &robot->distance_cs,
                           quadramp_do_filter,
                           &robot->distance_qr );

    cs_set_correct_filter( &robot->distance_cs,
                           pid_do_filter,
                           &robot->distance_pid );

    cs_set_process_in( &robot->distance_cs,
                       rs_set_distance,
                       &robot->rs ); // Met la sortie sur le pwm virtuel de la distance.

    cs_set_process_out( &robot->distance_cs,
                        rs_get_ext_distance,
                        &robot->rs ); // lecture codeur virtuel de la distance

    cs_set_consign( &robot->distance_cs, 0 ); //Met une consigne nulle.

    /**************************************************************************/
    /*                           Trajectory Manager (Trivial)                 */
    /**************************************************************************/
    trajectory_init( &robot->traj, ASSERV_FREQUENCY );
    trajectory_set_cs( &robot->traj, &robot->distance_cs, &robot->angle_cs );
    trajectory_set_robot_params( &robot->traj, &robot->rs, &robot->pos );

    // distance window, angle window, angle start
    trajectory_set_windows( &robot->traj, 15., 5.0, 10. ); // Prod

    // Angle BDM
    bd_init( &robot->angle_bd, &robot->angle_cs );
    bd_set_thresholds( &robot->angle_bd, 3000, 1 ); // thresold, duration.

    // Distance BDM
    bd_init( &robot->distance_bd, &robot->distance_cs );
    bd_set_thresholds( &robot->distance_bd, 3600, 1 ); // thresold, duration.

    robot->is_aligning = 0;

    // Initialisation deplacement:
    position_set( &robot->pos, 0, 0, 0 );

#if 1
    // Creates the control task.
    chThdCreateStatic( wa_control_sys,
                       sizeof( wa_control_sys ),
                       NORMALPRIO, // CS_TASK_PRIO
                       ControlSys,
                       NULL );

    // Creates the control task.
    chThdCreateStatic( wa_odometry,
                   sizeof( wa_odometry ),
                   NORMALPRIO, // ODOMETRY_TASK_PRIO
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
        rs_update( &sys.controls.robot.rs );

        // Gestion de l'asservissement.
        if( sys.controls.robot.mode != BOARD_MODE_SET_PWM )
        {
            if( sys.controls.robot.mode == BOARD_MODE_ANGLE_DISTANCE ||
                sys.controls.robot.mode == BOARD_MODE_ANGLE_ONLY )
            {
                cs_manage( &sys.controls.robot.angle_cs );
            }
            else
            {
                rs_set_angle( &sys.controls.robot.rs, 0 ); // Sets a null angle PWM
            }

            if( sys.controls.robot.mode == BOARD_MODE_ANGLE_DISTANCE ||
                sys.controls.robot.mode == BOARD_MODE_DISTANCE_ONLY )
            {
                cs_manage( &sys.controls.robot.distance_cs );
            }
            else
            {
                rs_set_distance( &sys.controls.robot.rs, 0 ); // Sets a distance angle PWM
            }
        }

        // Gestion du blocage
        bd_manage( &sys.controls.robot.angle_bd );
        bd_manage( &sys.controls.robot.distance_bd );

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
        position_manage( &sys.controls.robot.pos );

        // Wait 20 milliseconds (50 Hz)
        chThdSleepMilliseconds( 20 );
    }
}
