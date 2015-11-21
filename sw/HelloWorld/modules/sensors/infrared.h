/*
 * Simple module to manage IR sensors
 * Author: Jeremy S.
 * Rev : 1.0 
 */

#ifndef _INFRARED_H_
#define _INFRARED_H_

// Platform includes
#include "ch.h"
#include "hal.h"

typedef struct infrared_data infrared_data_t;
/*
 * This structure holds everything we need in order to manage IR sensors data
 *
 */
struct infrared_data
{
    int8_t address;
    int distance;

    infrared_data_t *next;
};

// This method initializes the infrared sensors struct.
void infrared_init( infrared_data_t *ir_data );

// Return the associated thread for handling the infrared sensors.
thread_t* infrared_get_thread( void );

/*
 * This method transfers the distances measured
 * by the infrared sensors to a data structure that
 * will be handled as part of a protocol.
 *
 * Pointer to a data structure to return data.
 */
void infrared_get_data( infrared_data_t *data );

/*
 * This method enables interactive access to the data
 * read from the infrarred sensor and allows the user
 * to print the last read data.
 *
 * Number of arguments for this command.
 * List of arguments.
 */
void cmd_print_ir_distance( int argc, char *argv[] );

#endif // _INFRARED_H_
