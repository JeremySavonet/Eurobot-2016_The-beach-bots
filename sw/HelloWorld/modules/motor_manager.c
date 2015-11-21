/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "../color.h"
#include "../comm/debug_manager.h"

#include "motor_manager.h"

#define DC_PWM_MAX_VALUE 20000

/*===========================================================================*/
/* PWM related.                                                              */
/*===========================================================================*/

static PWMConfig motor_pwm_cfg =
{
    1000000, // 1MHz PWM clock frequency
    20000,   // Initial PWM period 20ms ( 50hz (20ms) for standard servo/ESC, 400hz for fast servo/ESC (2.5ms = 2500) )
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

/*===========================================================================*/
/* QEI related.                                                              */
/*===========================================================================*/

static const QEIConfig motor_left_qei_cfg = 
{
    QEI_MODE_QUADRATURE,
    QEI_BOTH_EDGES,
    QEI_DIRINV_FALSE
};

static const QEIConfig motor_right_qei_cfg = 
{
    QEI_MODE_QUADRATURE,
    QEI_BOTH_EDGES,
    QEI_DIRINV_TRUE
};

int32_t motor_pwms[ NUM_MOTORS ];
pwmcnt_t motor_speeds[ NUM_MOTORS ];

void qei_manager_init( void )
{
    // Left encoder
    palSetPadMode( GPIOB, 4, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOB, 5, PAL_MODE_ALTERNATE( 2 ) );

    // Right encoder
    palSetPadMode( GPIOD, 12, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOD, 13, PAL_MODE_ALTERNATE( 2 ) );
    
    qeiStart( &MOTOR_LEFT_QEI_DRIVER, &motor_left_qei_cfg );
    qeiEnable( &MOTOR_LEFT_QEI_DRIVER );

    qeiStart( &MOTOR_RIGHT_QEI_DRIVER, &motor_right_qei_cfg );
    qeiEnable( &MOTOR_RIGHT_QEI_DRIVER );
}

void motor_manager_init( void )
{
    // Left/Right motors
    palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE( 3 ) );
    palSetPadMode( GPIOE, 6, PAL_MODE_ALTERNATE( 3 ) );
    
    pwmStart( &MOTOR_PWM_DRIVER, &motor_pwm_cfg );

    unsigned i;
    for( i = 0; i < NUM_MOTORS; ++i )
    {
        motor_set_speed( i, 0 );
    }
}

void motor_disable_pwm( unsigned motor )
{
    if( motor >= NUM_MOTORS )
    {
        return;
    }
    motor_speeds[ motor ] = 0;
    pwmDisableChannel( &MOTOR_PWM_DRIVER,
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
}

int32_t versatile_dc_get_encoder( void *device )
{
    uint16_t qei = qeiGetCount( (QEIDriver *) device );
    return qei;
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

void motor_set_speed( unsigned motor, pwmcnt_t speed )
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
    pwmEnableChannel( &MOTOR_PWM_DRIVER,
            motor,
            PWM_PERCENTAGE_TO_WIDTH( &MOTOR_PWM_DRIVER, speed ) );
}

pwmcnt_t motor_get_speed( unsigned motor )
{
    if( motor >= NUM_MOTORS )
    {
        return 0;
    }

    return motor_speeds[ motor ];
}
