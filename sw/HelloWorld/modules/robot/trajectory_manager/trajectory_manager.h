/*
 *  Copyright Droids Corporation, Microb Technology, Eirbot ( 2005 )
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Revision : $Id: trajectory_manager.h,v 1.4.4.10 2009-05-02 10:03:04 zer0 Exp $
 *
 */

/*
 * Interface to the trajectory manager.
 *
 * This module handles the high level trajectory management functions, like
 * going from one point to another in a smooth curve, etc...
 * 
 * This module only works with a control system made with control_system_manager,
 * position_manager, quadramp, robot_system. It uses the vect2 module.
 */

#ifndef _TRAJECTORY_MANAGER_H_
#define _TRAJECTORY_MANAGER_H_

#include "../../../common/math/vect2.h"
#include "../../../common/math/geometry/vect_base.h"
#include "../../../common/math/geometry/lines.h"

#include "../robot_system/robot_system.h"

// State of the trajectory manager.
enum trajectory_state 
{
    READY, // Ready, waiting for a trajectory.
    
    // simple trajectories
    RUNNING_A, //Turning without moving.
    RUNNING_D, // Going straight.
    RUNNING_AD, // Going forward and turning at the same time.
    
    // trajectories using events 
    RUNNING_XY_START,       // A trajectory order was given, preparing to go.
    RUNNING_XY_ANGLE,       // Doing a preliminary turn before starting to move.
    RUNNING_XY_ANGLE_OK,    // Angle is now ok, move forward.
    RUNNING_XY_F_START,     // Same as RUNNING_XY_START but forward only.
    RUNNING_XY_F_ANGLE,     // Same as RUNNING_XY_ANGLE but forward only.
    RUNNING_XY_F_ANGLE_OK,  // Same as RUNNING_XY_ANGLE_OK but forward only.
    RUNNING_XY_B_START,     // Same as RUNNING_XY_START but backward only.
    RUNNING_XY_B_ANGLE,     // Same as RUNNING_XY_ANGLE but backward only.
    RUNNING_XY_B_ANGLE_OK,  // Same as RUNNING_XY_B_ANGLE_OK but backward only.
    
    // circle 
    RUNNING_CIRCLE,          // Running a circle trajectory.
    
    // line 
    RUNNING_LINE,            // Following a line.
    
    // clitoid
    RUNNING_CLITOID_LINE,    // Running a clitoid ( line->circle->line ) in the line part.
    RUNNING_CLITOID_CURVE,   // Running a clitoid in the curve part.
};

// Movement target when running on a circle.
struct circle_target 
{
    vect2_cart center;   // center of the circle
    double radius;       // radius of the circle
    int32_t dest_angle;  // dst angle in inc
    int32_t dest_distance;  // dst angle in inc 

#define TRIGO   1 // Rotation is counterclockwise
#define FORWARD 2 // go forward or backward
    uint8_t flags;   // flags for this trajectory
};

// Movement target when running a line or a clitoid.
struct line_target 
{
    line_t line;  // The line to follow.
    double angle; // The angle of line.
    double advance; // The sampling factor.
    
    // only for clitoid
    point_t turn_pt; // The starting point of the clitoid.
    double Aa;  // The angular acceleration.
    double Va; // The angular speed.
    double alpha; // The angle to turn.
    double R; // The radius of the circular part.
};

// A complete instance of the trajectory manager.
struct trajectory
{
    enum trajectory_state state; // describe the type of target, and if we reached the target 
    
    union 
    {
        vect2_cart cart;             // target, if it is a x,y vector
        struct rs_polar pol;         // target, if it is a d,a vector
        struct circle_target circle; // target, if it is a circle 
        struct line_target line;     // target, if it is a line
    } target;                        // Target of the movement.
    
    double d_win; 	   // distance window ( for END_NEAR ) 
    double a_win_rad;  // angle window ( for END_NEAR ) 
    double a_start_rad;// in xy consigns, start to move in distance when a_target < a_start 
    
    double d_speed;  // distance speed consign 
    double a_speed;  // angle speed consign
    
    double d_acc;    // distance acceleration consign 
    double a_acc;    // angle acceleration consign
    
    struct robot_position *position; // associated robot_position
    struct robot_system *robot;      // associated robot_system
    struct cs *csm_angle;     // associated control system ( angle )
    struct cs *csm_distance;  // associated control system ( distance )
    
    double cs_hz;   // The frequency of the control system associated with this manager. 
    
    int8_t scheduler_task;    // id of current task ( -1 if no running task ) 
};

/* 
 * Structure initialization.
 *
 * [in] traj The trajectory manager to initialize.
 * [in] cs_hz The frequency of the control systems, in Hz. 
 */
void trajectory_init( struct trajectory *traj, double cs_hz );

/* 
 * Sets the control systems.
 *
 * This function tells the trajectory manager which control system to use for
 * angle and distance control.
 *
 * [in] traj Tje trajectory manager instance.
 * [in] cs_d, cs_a The control systems to use.
 */
void trajectory_set_cs( struct trajectory *traj, 
                        struct cs *cs_d,
                        struct cs * cs_a );

/*
 * Sets related robot params.
 *
 * Sets the robot_pos and robot_system used for the computation of the trajectory.
 * [in] traj The trajectory manager instance.
 * [in] rs The related robot system.
 * [in] pos The position manager instance of the robot. 
 */
void trajectory_set_robot_params( struct trajectory *traj,
                                  struct robot_system *rs,
                                  struct robot_position *pos ) ;

/*
 * Set speed consign.
 *
 * [in] traj The trajectory manager instance.
 * [in] d_speed The distance speed in imp / period.
 * [in] a_speed The angular speed in imp / period.
 */
void trajectory_set_speed( struct trajectory *traj, 
                           double d_speed, 
                           double a_speed );

/* 
 * Set acceleration consign. 
 *
 * [in] traj The trajectory manager instance.
 * [in] d_acc The distance acceleration in imp / period^2.
 * [in] a_acc The angular acceleration in imp / period^2.
 */
void trajectory_set_acc( struct trajectory *traj, double d_acc, double a_acc );

/* 
 * Set windows for trajectory.
 *
 * This function sets the windows for the trajectory. The distance window
 * is the distance for which we say we reached target point.
 * The angular window is the angle for which we say we have the correct heading
 * ( used only when doing angular trajectories ).
 * Finally the angular start windows is used at the beginning of an XY movement
 * to decide if we need to turn first before starting the curve.
 *
 * [in] traj The trajectory manager instance.
 * [in] d_win The distance window in mm.
 * [in] a_win_deg The angular window in degrees.
 * [in] a_start_deg The angular start window in degrees. 
 */
void trajectory_set_windows( struct trajectory *traj, 
                             double d_win,
                             double a_win_deg, 
                             double a_start_deg );

#endif // _TRAJECTORY_MANAGER_H_
