/*
 * Beacon module.
 * This module handles data coming from zibee through our mrf24j40 api.
 * Data are then processed and passed to the strat manager.
 * Author: Jeremy S.
 * Rev: 1.0
 */

// Platform includes
#include "ch.h"
#include "hal.h"

// Use our API
#include "../rf/mrf24j40/mrf24j40.h"

#include "beacon.h"

// Private functions
void beacon_read( void );

// Public functions
// TODO: implements
void beacon_get_data( beacon_data_t * data )
{
    beacon_read();

    if( NULL != data )
    {
        // TODO: update the struct param
    }
}

// TODO: implements
void beacon_init( beacon_data_t * data )
{
}

// TODO: implements
void beacon_read( void )
{
}
