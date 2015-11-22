/*
 * This module contains the set of commands manage by our CLI.
 * Author: Jeremy S.
 * Rev: 1.0
 */

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "chprintf.h"

#include "microshell.h"

#include "../versatile_cs.h"

// temp : add ref to modules. Deprecated all of this will be accessible via
// robot struct

#include "../modules/sensors/infrared.h"
#include "../modules/motor_manager.h"

// Command to start asserv
void cmd_start_asserv( int argc, char *argv[] );

// Command to stop asserv
void cmd_stop_asserv( int argc, char *argv[] ); 

// Command to set robot modes
void cmd_set_robot_mode_angle( int argc, char *argv[] );
void cmd_set_robot_mode_distance( int argc, char *argv[] );
void cmd_set_robot_mode_free( int argc, char *argv[] );
void cmd_set_robot_mode_all( int argc, char *argv[] );
void cmd_set_robot_mode_pwm( int argc, char *argv[] );

// Command to get robot values
void cmd_get_robot_position( int argc, char *argv[] );
void cmd_get_encoder( int argc, char *argv[] );
void cmd_set_pwm( int argc, char* argv[] );

#endif // _COMMANDS_H_
