#include "battery.h"
#include "console.h"
#include "adc.h"
#include "delay.h"

// === GLOBAL VARIABLES ===
volatile u8 g_batt_state_level = 2;

// Function to monitor battery level
u16 battery_state( void )
{
    u16 batt_level = 0;
    
    return batt_level;
}