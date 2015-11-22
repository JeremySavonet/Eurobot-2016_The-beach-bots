/*
 * This files init all sensors linked to the system.
 * Run the thread to update sensor structures
 *
 * Author Jeremy S.
 * Rev: 1.0
 */

#include "modules/sensors/infrared.h"

#include "versatile_sensors.h"

//===================== VERSATILE SENSORS TASKS DEFINTION ====================//

static THD_WORKING_AREA( wa_sensors_sys, 512 );
static THD_FUNCTION( Sensors, arg );

void versatile_sensors_init( system_sensors_t * sensors )
{
    // Maybe good to have sensor mode ??    
#if 1
    // Creates the task to update sensor structures.
    chThdCreateStatic( wa_sensors_sys,
                       sizeof( wa_sensors_sys ),
                       NORMALPRIO, //CS_TASK_PRIO
                       Sensors,
                       NULL );
#endif
}

// Sensors manager thread, times are in milliseconds.
THD_FUNCTION( Sensors, arg )
{
    (void)arg;
    while( true )
    {
        // Update the structure
        infrared_get_data( &sys.sensors.ir_sensors );

        // TODO : adjust the frequency.
        // Wait 20 milliseconds (50 Hz)
        chThdSleepMilliseconds( 20 );
    }
}
