/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#include "ch.h"
#include "hal.h"

#ifndef _MOTORS_H_
#define _MOTORS_H_

#define NUM_MOTORS 4
#define MAX_MOTOR_SPEED 10000

void MotorsInit( void );

void MotorDisablePwm( unsigned motor );
void MotorSetSpeed( unsigned motor, pwmcnt_t speed );
pwmcnt_t MotorGetSpeed( unsigned motor );

#endif /* _MOTORS_H_ */
