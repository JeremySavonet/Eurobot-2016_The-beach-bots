/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#include "dc_motor_manager.h"

/*===========================================================================*/
/* PWM related.                                                              */
/*===========================================================================*/

static PWMConfig motor_pwm_cfg =
{
    50000, // 50KHz PWM clock frequency
    1000,   // Initial PWM period 20ms ( 50hz (20ms) 
    NULL,    // No callback
    {
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_OUTPUT_DISABLED, NULL },
        { PWM_OUTPUT_DISABLED, NULL }
    },
    0,
#if STM32_PWM_USE_ADVANCED
    0,
#endif
    0
};

void dc_motor_manager_init( void )
{
    pwmStart( &DC_MOTOR_PWM_DRIVER, &motor_pwm_cfg );

    // Left/Right motors
    palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE( 3 ) );
    palSetPadMode( GPIOE, 6, PAL_MODE_ALTERNATE( 3 ) );

    uint8_t i;
    for( i = 0; i < DC_NUM_MOTORS; ++i )
    {
        versatile_dc_set_pwm( DC_MOTOR_CONTROLLER_BASE, i, 0 );
    }
}

void versatile_dc_disable_pwm( void *device, int channel )
{
    if( channel < 0 || channel > DC_NUM_MOTORS )
    {
        return;
    }

    pwmDisableChannel( device,
                       channel );
}

void versatile_dc_set_pwm( void *device, int channel, int32_t value )
{
    if( channel < 0 || channel > DC_NUM_MOTORS )
    {
        return;
    }

    if( value < DC_PWM_MIN_VALUE )
    {
        value = DC_PWM_MIN_VALUE;
    }

    if( value > DC_PWM_MAX_VALUE )
    {
        value = DC_PWM_MAX_VALUE;
    }

    pwmEnableChannel( device,
                      channel,
                      (pwmcnt_t) value );
}

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
