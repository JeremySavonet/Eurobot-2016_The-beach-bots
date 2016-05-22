/*
 *  Low level esc motors driver module
 *  Manages motor speeds and sends them via PPM signals
 *  See : Trajectory Manager module
 *
 *  Author: Jeremy S.
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#ifndef _BRUSHLESS_MOTOR_MANAGER_H_
#define _BRUSHLESS_MOTOR_MANAGER_H_

// Platform includes
#include "ch.h"
#include "hal.h"

#define BRUSHLESS_NUM_MOTORS 2

#define BRUSHLESS_MOTOR_CONTROLLER_BASE ( (PWMDriver *) &PWMD9 )

#define BRUSHLESS_MOTOR_PWM_DRIVER PWMD9

#define ESC_UPDATE_RATE     400 // Hz (50 Hz std servo/esc) ( 400 Hz fast servo/esc)
#define TIM_CLOCK           1000000 // Hz

void brushless_motor_manager_init( void );

void versatile_esc_disable_ppm( void *device, int channel );

void versatile_esc_set_ppm( void *device, int channel, int32_t pulse_width );
        
void versatile_esc_set_ppm_all( int32_t pulse_width );
void versatile_esc_set_ppm0( void *device, int32_t value );
void versatile_esc_set_ppm1( void *device, int32_t value );

void versatile_esc_set_ppm_negative0( void *device, int32_t value );
void versatile_esc_set_ppm_negative1( void *device, int32_t value );

#endif // _BRUSHLESS_MOTOR_MANAGER_H_
