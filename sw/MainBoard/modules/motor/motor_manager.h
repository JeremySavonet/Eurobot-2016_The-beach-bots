/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#ifndef _MOTOR_MANAGER_H_
#define _MOTOR_MANAGER_H_

// Platform includes
#include "ch.h"
#include "hal.h"

#define DC_PWM_MAX_VALUE 20000

#define NUM_MOTORS 2
#define MAX_MOTOR_SPEED 10000

#define MOTOR_CONTROLLER_BASE ( (PWMDriver *) &PWMD9 )

#define MOTOR_PWM_DRIVER PWMD9

#define MOTOR_ENCODER_BASE_LEFT  ( (QEIDriver *) &QEID3 )
#define MOTOR_ENCODER_BASE_RIGHT  ( (QEIDriver *) &QEID4 )

#define MOTOR_LEFT_QEI_DRIVER QEID3
#define MOTOR_RIGHT_QEI_DRIVER QEID4

void qei_manager_init( void );
void motor_manager_init( void );

void versatile_dc_disable_pwm( void *device, int channel );

void versatile_dc_set_pwm( void *device, int channel, int32_t value );
        
int32_t versatile_dc_get_encoder( void *device );

void versatile_dc_set_pwm0( void *device, int32_t value );
void versatile_dc_set_pwm1( void *device, int32_t value );

void versatile_dc_set_pwm_negative0( void *device, int32_t value );
void versatile_dc_set_pwm_negative1( void *device, int32_t value );

#endif // _MOTOR_MANAGER_H_
