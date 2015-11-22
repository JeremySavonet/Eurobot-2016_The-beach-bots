/*
 * This module contains the set of commands manage by our CLI.
 * Author: Jeremy S.
 * Rev: 1.0
 */

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

// Plateform includes
#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "microshell.h"

// Command to start asserv
void cmd_start_asserv( int argc, char *argv[] );

// Command to stop asserv
void cmd_stop_asserv( int argc, char *argv[] ); 

// Command to set robot modes
void cmd_set_robot_mode( int argc, char *argv[] );

// Command to get robot values
void cmd_get_robot_position( int argc, char *argv[] );
void cmd_get_encoder( int argc, char *argv[] );
void cmd_set_pwm( int argc, char* argv[] );

// Command to get sensors values

/*
 * This method enables interactive access to the data
 * read from the infrarred sensor and allows the user
 * to print the last read data.
 *
 * Number of arguments for this command.
 * List of arguments.
 */
void cmd_print_ir_distance( int argc, char *argv[] );
void cmd_print_temp( int argc, char *argv[] ); 
void cmd_print_battery_level( int argc, char *argv[] ); 

#endif // _COMMANDS_H_
