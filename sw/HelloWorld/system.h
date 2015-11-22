/*
 * Manage board structure
 * Author: Jermy S.
 * Rev: 1.0
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// For robot structure
#include "versatile_cs.h"

#include "modules/sensors/infrared.h"

// Enum of state of the system
typedef enum
{
    state_system_unknown,         // System is in unknown state
    state_system_init,            // System is about to initialize
    state_system_working,         // System is about to working
    state_system_crash            // System is crashed
} system_state_t;

typedef struct
{
    //Add here all sensor strctures
    infrared_data_t ir_sensors; 
    
} system_sensors_t;

typedef struct
{
    //Add here all comm structure ( can_proto, spi_proto, i2c_proto... )
} system_comm_t;

typedef struct
{
    /* 
     * Contains the robot variables:
     * - robot_sys
     * - encoders
     * - position_manager
     * - control_sys
     * - pid...
     */
    /*
     * Contient toutes les variables globales.
     * Cette structure sert en quelque sorte de namespace pour les variables globales,
     * qui peuvent ensuite etre accedees en faisant robot.color par exemple.
     */
    versatile_robot_t robot;
    
    /*
     * Contains the io variables:
     * - buttons
     * - led...
     */
    //versatile_io_t io;
} system_controls_t;

typedef struct
{
    // Aggregate here all robot structures
    system_comm_t     comm;
    system_sensors_t  sensors;
    system_controls_t controls;

} system_t;

extern system_t sys;

void system_print_boot_msg( void );
void system_init( void );

#endif // _SYSTEM_H_
