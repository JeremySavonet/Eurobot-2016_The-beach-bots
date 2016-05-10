/*
 * This files init all sensors linked to the system.
 * Run the thread to update sensor structures
 *
 * Author Jeremy S.
 * Date: 2015-10-03
 * Rev: 1.0
 */

#ifndef _VERSATILE_SENSORS_H_
#define _VERSATILE_SENSORS_H_

// Platform include
#include "ch.h"

#include "system.h"

void versatile_sensors_init( system_sensors_t * sensors );

#endif // _VERSATILE_SENSORS_H_
