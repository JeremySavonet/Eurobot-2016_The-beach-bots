#include "gpio.h"

void init_io( void )
{
    volatile unsigned int *reg_addr;

    // Reset : All I/O digital
    for( reg_addr = &ANCFG; reg_addr <= &ANSG; reg_addr++ )      
    {
        *reg_addr = 0;
    }
    
    // Reset internal pull-up & internal pull-down
    for( reg_addr = &CNPD1; reg_addr <= &CNPU6; reg_addr++ )    
    {
        *reg_addr = 0;
    }

    // No open drain
    ODCB = 0;
    ODCC = 0;
    ODCD = 0;
    ODCE = 0;
    ODCF = 0;
    ODCG = 0;

    // All pins to 0 in output
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;

    // All pins in output excepted those ones :
    TRISB = 0b0000000000000000;        
    TRISC = 0b0000000000000000;
    TRISD = 0b0000000000000000;     
    TRISE = 0b0000000000000000;        
    TRISF = 0b0000000000000000;        
    TRISG = 0b0000000000000000;        

    // Analog input
    ANSB = 0b0000000000000000;
}