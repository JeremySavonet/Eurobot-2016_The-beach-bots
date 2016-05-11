/*
 * This files init all sensors linked to the system.
 * Run the thread to update sensor structures
 *
 * Author Jeremy S.
 * Date: 2015-10-03
 * Rev: 1.0
 */

#include "modules/sensors/battery.h"
#include "modules/sensors/infrared.h"
#include "modules/sensors/temperature.h"

#include "versatile_sensors.h"

//===================== VERSATILE SENSORS TASKS DEFINTION ====================//

static THD_WORKING_AREA( wa_sensors_sys, 512 );
static THD_FUNCTION( Sensors, arg );

void versatile_sensors_init( system_sensors_t * sensors )
{
    // Maybe good to have sensor mode ??
    battery_init( &sensors->batt_sensor );
    infrared_init( &sensors->ir_sensors );
    temperature_init( &sensors->temp_sensors );

#if 1
    // Creates the task to update sensor structures.
    chThdCreateStatic( wa_sensors_sys,
                       sizeof( wa_sensors_sys ),
                       NORMALPRIO, // CS_TASK_PRIO
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
        battery_get_data( &sys.sensors.batt_sensor );
        infrared_get_data( &sys.sensors.ir_sensors );
        temperature_get_data( &sys.sensors.temp_sensors );

        // TODO : adjust the frequency.
        // Wait 20 milliseconds (50 Hz)
        chThdSleepMilliseconds( 20 );
    }
}
