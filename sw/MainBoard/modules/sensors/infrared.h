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

// This structure holds everything we need in order to manage IR sensors data
struct infrared_data
{
    float ir_d0;
    float ir_d1;
    float ir_d2;
};

// This method initializes the infrared sensors struct.
void infrared_init( infrared_data_t *ir_data );

/*
 * This method transfers the distances measured
 * by the infrared sensors to a data structure that
 * will be handled as part of a protocol.
 *
 * Pointer to a data structure to return data.
 */
void infrared_get_data( infrared_data_t *data );

#endif // _INFRARED_H_
