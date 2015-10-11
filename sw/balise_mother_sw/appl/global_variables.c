#include "global_variables.h"
#include "system.h"

volatile u8 g_adxl_mask;
volatile u16 g_duty_cycle;

void global_variables_init( void )
{
    g_adxl_mask = 0;
    g_duty_cycle = 0;
}