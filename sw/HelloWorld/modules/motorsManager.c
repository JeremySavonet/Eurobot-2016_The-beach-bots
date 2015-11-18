/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "../color.h"
#include "../comm/debugManager.h"

#include "motorsManager.h"

#define DC_PWM_MAX_VALUE 4095

static PWMConfig pwmcfg =
{
    1000000, // 1MHz PWM clock frequency
    20000,   // Initial PWM period 20ms ( 50hz (20ms) for standard servo/ESC, 400hz for fast servo/ESC (2.5ms = 2500) )
    NULL,    // No callback
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
    0,
#if STM32_PWM_USE_ADVANCED
    0,
#endif
    0
};

int32_t motor_pwms[ NUM_MOTORS ];
pwmcnt_t motor_speeds[ NUM_MOTORS ];

void motorsManagerInit( void )
{
    palSetPadMode( GPIOD, 12, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOD, 13, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOD, 14, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOD, 15, PAL_MODE_ALTERNATE( 2 ) );
    pwmStart( &PWMD4, &pwmcfg );

    unsigned i;
    for( i = 0; i < NUM_MOTORS; ++i )
    {
        MotorSetSpeed( i, 0 );
    }
}

void MotorDisablePwm( unsigned motor )
{
    if( motor >= NUM_MOTORS )
    {
        return;
    }
    motor_speeds[ motor ] = 0;
    pwmDisableChannel( &PWMD4,
                       motor );
}

void versatile_dc_set_pwm( void *device, int channel, int32_t value )
{
    if( channel < 0 || channel > 5 )
    {
        return;
    }

    if( value < -DC_PWM_MAX_VALUE )
    {
        value = -DC_PWM_MAX_VALUE;
    }

    if( value > DC_PWM_MAX_VALUE )
    {
        value = DC_PWM_MAX_VALUE;
    }

    pwmEnableChannel( device,
                      channel,
                      (pwmcnt_t) value );
    DPRINT( 3, KBLU "SAT PWM TO %d IN DC SET PWM FOR CHANNEL %d\r\n", value,
                                                                      channel );
}

int32_t versatile_dc_get_encoder( void *device, int channel )
{
    if( channel < 0 || channel > 5 )
    {
        return -1;
    }

    return qeiUpdateI( (QEIDriver *) device );
}

//TODO: this is temp function need to properly name those two functions
void versatile_dc_set_pwm0( void *device, int32_t value )
{
    versatile_dc_set_pwm( device, 0, value );
}

void versatile_dc_set_pwm1( void *device, int32_t value )
{
    versatile_dc_set_pwm( device, 1, value );
}

void versatile_dc_set_pwm_negative0( void *device, int32_t value )
{
    versatile_dc_set_pwm( device, 0, -value );
}

void versatile_dc_set_pwm_negative1( void *device, int32_t value )
{
    versatile_dc_set_pwm( device, 1, -value );
}

int32_t versatile_dc_get_encoder0( void *device )
{
    return versatile_dc_get_encoder( device, 0 );
}

int32_t versatile_dc_get_encoder1( void *device )
{
    return versatile_dc_get_encoder( device, 1 );
}

void MotorSetSpeed( unsigned motor, pwmcnt_t speed )
{
    if( motor >= NUM_MOTORS )
    {
        return;
    }
    if( speed > MAX_MOTOR_SPEED )
    {
        speed = MAX_MOTOR_SPEED;
    }

    motor_speeds[ motor ] = speed;
    pwmEnableChannel( &PWMD4,
            motor,
            PWM_PERCENTAGE_TO_WIDTH( &PWMD4, speed ) );
}

pwmcnt_t MotorGetSpeed( unsigned motor )
{
    if( motor >= NUM_MOTORS )
    {
        return 0;
    }

    return motor_speeds[ motor ];
}
