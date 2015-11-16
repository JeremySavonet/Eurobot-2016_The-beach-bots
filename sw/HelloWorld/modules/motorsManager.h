/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#ifndef _MOTORS_H_
#define _MOTORS_H_

#define NUM_MOTORS 4
#define MAX_MOTOR_SPEED 10000

void motorsManagerInit( void );

void MotorDisablePwm( unsigned motor );
void MotorSetSpeed( unsigned motor, pwmcnt_t speed );
pwmcnt_t MotorGetSpeed( unsigned motor );

void versatile_dc_set_pwm( void *device, int32_t value );
void versatile_dc_set_pwm_negative( void *device, int32_t value );

#endif /* _MOTORS_H_ */
