/*
 * Simple module to manage temperature sensors
 * Author: Jeremy S.
 * Rev : 1.0 
 */

#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

// Platform includes
#include "ch.h"
#include "hal.h"

typedef struct temperature_data temperature_data_t;

// This structure holds everything we need in order to manage temperature sensors data
struct temperature_data
{
    float temp0;
};

// This method initializes the temperature sensors struct.
void temperature_init( temperature_data_t *temp_data );

/*
 * This method transfers the temperature measured
 * by the onboard sensor to a data structure that
 * will be handled as part of a protocol.
 *
 * Pointer to a data structure to return data.
 */
void temperature_get_data( temperature_data_t *data );

#endif // _TEMPERATURE_H_
