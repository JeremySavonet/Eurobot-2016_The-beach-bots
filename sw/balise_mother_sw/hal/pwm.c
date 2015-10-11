#include "pwm.h"
#include "pin.h"
#include "console.h"
#include "global_variables.h"

void do_pwm( void )
{
    static u16 time = 0;
    time++;

    if(time > PWM_PERIOD)
    {
        time = 0;
    }
    
    if( g_duty_cycle == 0 )
    {
        LED4 = 0;
    }
    else if( g_duty_cycle == PWM_PERIOD )
    {
        LED4 = 1;
    }
    else if( time < g_duty_cycle ) 
    {
        LED4 = 1;
    } 
    else {
        LED4 = 0;
    }
}