#include "adc.h"
#include "delay.h"

void init_adc( void )
{
    volatile unsigned int *reg_addr;
    PMD1bits.ADC1MD = 0;

    // Reset all ADC registers
    for( reg_addr = &ADC1BUF0; reg_addr <= &AD1DMBUF; reg_addr++ )       
    {
        *reg_addr = 0;
    }
    
    AD1CON1 = 0b0010010000000000;
    AD1CON3 = 0x0002;           // Manual Sample, Tad = 3Tcy

    AD1CON1bits.ADON = 1;       // Turn ADC ON
}

s16 read_adc( u8 channel )
{
    AD1CHS = channel;           // CH_A.IN+ = AN11  CH_A.IN- = AVSS

    AD1CON1bits.SAMP = 1;
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    AD1CON1bits.SAMP = 0;

    // Conversion done?
    while( !AD1CON1bits.DONE );                          

    return ADC1BUF0;
}
