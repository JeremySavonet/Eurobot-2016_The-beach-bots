/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#ifndef _MOTORS_H_
#define _MOTORS_H_

#define NUM_MOTORS 2
#define MAX_MOTOR_SPEED 10000

// Platform includes
#include "ch.h"
#include "hal.h"

#define MOTOR_CONTROLLER_BASE ( (PWMDriver *) &PWMD9 )

#define MOTOR_PWM_DRIVER PWMD9

#define MOTOR_ENCODER_BASE_LEFT  ( (QEIDriver *) &QEID3 )
#define MOTOR_ENCODER_BASE_RIGHT  ( (QEIDriver *) &QEID4 )

#define MOTOR_LEFT_QEI_DRIVER QEID3
#define MOTOR_RIGHT_QEI_DRIVER QEID4

void qei_manager_init( void );
void motor_manager_init( void );

void motor_disable_pwm( unsigned motor );
void motor_set_speed( unsigned motor, pwmcnt_t speed );
pwmcnt_t motor_get_speed( unsigned motor );

void versatile_dc_set_pwm( void *device, int channel, int32_t value );

int32_t versatile_dc_get_encoder( void *device );

void versatile_dc_set_pwm0( void *device, int32_t value );
void versatile_dc_set_pwm1( void *device, int32_t value );

void versatile_dc_set_pwm_negative0( void *device, int32_t value );
void versatile_dc_set_pwm_negative1( void *device, int32_t value );

#endif /* _MOTORS_H_ */
