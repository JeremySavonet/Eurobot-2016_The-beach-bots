/*
 * Simple module to manage battery level
 * Author: Jeremy S.
 * Rev : 1.0 
 */

#ifndef _BATTERY_H_
#define _BATTERY_H_

// Platform includes
#include "ch.h"
#include "hal.h"

typedef struct battery_data battery_data_t;

// This structure holds everything we need in order to manage the battery level
struct battery_data
{
    float batt_level;
};

// This method initializes the battery data struct.
void battery_init( battery_data_t *batt_data );

/*
 * This method transfers the battery measured
 * by the dedicated adc to a data structure that
 * will be handled as part of a protocol.
 *
 * Pointer to a data structure to return data.
 */
void battery_get_data( battery_data_t *data );

#endif // _BATTERY_H_
