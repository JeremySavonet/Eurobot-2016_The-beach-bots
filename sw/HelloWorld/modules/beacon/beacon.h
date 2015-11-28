/*
 * Beacon module.
 * This module handles data coming from zibee through our mrf24j40 api.
 * Data are then processed and passed to the strat manager.
 * Author: Jeremy S.
 * Rev: 1.0
 */

#ifndef _BEACON_H_
#define _BEACON_H

#define MAX_NUMBER_OF_BEACONS 3

// Define a Beacon structure to pack all the data
typedef struct 
{
    uint8_t beacon_id;
} beacon_data_t;

void beacon_init( beacon_data_t * data );
void beacon_get_data( beacon_data_t * data );

#endif // _BEACON_H_
