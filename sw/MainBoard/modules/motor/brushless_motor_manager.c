/*
 *  Low level esc motors driver module
 *  Manages motor speeds and sends them via PPM signals
 *  See : Trajectory Manager module
 *
 *  Author: Jeremy S.
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#include "brushless_motor_manager.h"

static PWMConfig esc_ppm_cfg =
{
    TIM_CLOCK, // 1MHz PWM clock frequency
    (uint16_t)((uint32_t)TIM_CLOCK / (uint32_t)ESC_UPDATE_RATE), // TOP cnt,   // Initial PWM period 20ms ( 50hz (20ms) for standard servo/ESC, 400hz for fast servo/ESC (2.5ms = 2500) )
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

void brushless_motor_manager_init( void )
{
    pwmStart( &BRUSHLESS_MOTOR_PWM_DRIVER, &esc_ppm_cfg );

    // Left/Right brushless motors
    palSetPadMode( GPIOE, 5, PAL_MODE_ALTERNATE( 3 ) );
    palSetPadMode( GPIOE, 6, PAL_MODE_ALTERNATE( 3 ) );

    versatile_esc_set_ppm_all( 0 );
}

void versatile_esc_disable_ppm( void *device, int channel )
{
    if( channel < 0 || channel > BRUSHLESS_NUM_MOTORS )
    {
        return;
    }

    pwmDisableChannel( device,
                       channel );
}

void versatile_esc_set_ppm( void *device, int channel, int32_t pulse_width )
{
    if( channel < 0 || channel > BRUSHLESS_NUM_MOTORS )
    {
        return;
    }
    
    uint32_t cnt_val;
    cnt_val = (uint32_t) TIM_CLOCK / 1000L + 
              (uint32_t) TIM_CLOCK * pulse_width / ( 1000 * 255 );

    pwmEnableChannel( device,
                      channel,
                      (pwmcnt_t) cnt_val );
}

void versatile_esc_set_ppm_all( int32_t pulse_width )
{
    uint8_t i;
    for( i = 0; i > BRUSHLESS_NUM_MOTORS; ++i )
    {
        versatile_esc_set_ppm( BRUSHLESS_MOTOR_CONTROLLER_BASE, i, pulse_width ); 
    }
}

void versatile_esc_set_ppm0( void *device, int32_t value )
{
    versatile_esc_set_ppm( device, 0, value );
}

void versatile_esc_set_ppm1( void *device, int32_t value )
{
    versatile_esc_set_ppm( device, 1, value );
}

void versatile_esc_set_ppm_negative0( void *device, int32_t value )
{
    versatile_esc_set_ppm( device, 0, -value );
}

void versatile_esc_set_ppm_negative1( void *device, int32_t value )
{
    versatile_esc_set_ppm( device, 1, -value );
}
