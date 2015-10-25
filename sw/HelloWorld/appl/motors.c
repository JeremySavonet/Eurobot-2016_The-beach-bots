/*
 *  Low level motors driver module
 *  Manages motor speeds and sends them via PWM
 *  See : Trajectory Manager module
 */

#include "ch.h"
#include "hal.h"

#include "motors.h"

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

pwmcnt_t motor_speeds[ NUM_MOTORS ];

void MotorsInit( void )
{   
    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 15, PAL_MODE_ALTERNATE(2));
    pwmStart(&PWMD4, &pwmcfg);

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