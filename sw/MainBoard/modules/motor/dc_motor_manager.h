/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#ifndef _DC_MOTOR_MANAGER_H_
#define _DC_MOTOR_MANAGER_H_

// Platform includes
#include "ch.h"
#include "hal.h"

#define DC_PWM_MIN_VALUE 0
#define DC_PWM_MAX_VALUE 100

#define DC_NUM_MOTORS 2
#define DC_MAX_MOTOR_SPEED 1000

#define DC_MOTOR_CONTROLLER_BASE ( (PWMDriver *) &PWMD9 )

#define DC_MOTOR_PWM_DRIVER PWMD9

void dc_motor_manager_init( void );

void versatile_dc_disable_pwm( void *device, int channel );

void versatile_dc_set_pwm( void *device, int channel, int32_t value );
        
void versatile_dc_set_pwm0( void *device, int32_t value );
void versatile_dc_set_pwm1( void *device, int32_t value );

void versatile_dc_set_pwm_negative0( void *device, int32_t value );
void versatile_dc_set_pwm_negative1( void *device, int32_t value );

#endif // _DC_MOTOR_MANAGER_H_
