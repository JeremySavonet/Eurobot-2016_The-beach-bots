#include "spi.h"
#include "system.h"

void init_spi1( void );
void init_spi2( void );

void init_spi( void )
{
    init_spi1();
    init_spi2();
}

void init_spi1( void )
{
    PMD1bits.SPI1MD = 0;
    SPI1STAT = 0;
    SPI1CON2 = 0;
    SPI1CON1 = 0b0000000100111011;
    // DISSCK = 0 : Internal Serial Clock is Enabled
    // DISSDO = 0 : SDO1 pin is controlled by the module
    // MODE16 = 0 : Communication is byte-wide (8 bits)
    // SMP = 0 : Input Data is sampled at the middle of data output time
    // CKE = 1 : Serial output data changes on transition from active clock state to Idle clock state
    // SSEN = 0 : SSx pin not used by module; pin controlled by port function
    // CKP = 0 : Idle state for clock is a low level; active state is a high level
    // MSTEN = 1 : Master Mode Enabled
    // SPRE = 6 : SEC = 1:2
    // PPRE = 3 : PRI = 1:1
    // => SPICLK_FREQ = 8.000 MHz

    IFS0bits.SPF1IF = 0;        // Reset flag IRQ SPI1 Fault
    IFS0bits.SPI1IF = 0;        // Reset flag IRQ SPI1 Event
    SPI1STATbits.SPIEN = 1;     // Active SPI1
}

u8 spi1_u8_write( u8 data )
{
    SPI1BUF = data;             // Put data in TX buff
    
    // Wait for a word in RX
    while( !SPI1STATbits.SPIRBF ) ;      
    
    return SPI1BUF;           // Read register free SPIRBF flag
}

u8 spi1_u8_read( void )
{
    return spi1_u8_write( 0 );
}

// Master 8bits 4 MHz
void init_spi2( void )
{
    PMD1bits.SPI2MD = 0;
    SPI2STAT = 0;
    SPI2CON2 = 0;
    SPI2CON1 = 0b0000000100111011;
    //SPI2CON1 = 0b0000000100111110;
    // DISSCK = 0 : Internal Serial Clock is Enabled
    // DISSDO = 0 : SDO2 pin is controlled by the module
    // MODE16 = 0 : Communication is byte-wide (8 bits)
    // SMP = 0 : Input Data is sampled at the middle of data output time
    // CKE = 1 : Serial output data changes on transition from active clock state to Idle clock state
    // SSEN = 0 : SSx pin not used by module; pin controlled by port function
    // CKP = 0 : Idle state for clock is a low level; active state is a high level
    // MSTEN = 1 : Master Mode Enabled
    // SPRE = 7 : SEC = 1:1
    // PPRE = 3 : PRI = 4:1
    // => SPICLK_FREQ = 4.000 MHz

    IFS2bits.SPF2IF = 0;        // ràz du flag IRQ SPI Fault
    IFS2bits.SPI2IF = 0;        // ràz du flag IRQ SPI Event

    IEC2bits.SPF2IE = 1;        // SPI fault IRQ on
    IEC2bits.SPI2IE = 0;        // SPI event IRQ off

    SPI2STATbits.SPIEN = 1;     // active SPI2
}

u8 spi2_u8_write( u8 data )
{
    // Wait for free space
    while( SPI2STATbits.SPITBF ); 
    
    SPI2BUF = data;             // Start emitting
    
    // Wait for end of transmission
    while( !SPI2STATbits.SPIRBF );      
    
    return ((u8) SPI2BUF);
}

u8 spi2_u8_read( void )
{
    return spi2_u8_write( 0 );
}

// IRQs
// On SPI irq, clear the flag
void __attribute__ ((__interrupt__, auto_psv)) _SPI2Interrupt(void)
{
    IFS2bits.SPI2IF = 0;
}

// On SPI err, restart the module
void __attribute__ ((__interrupt__, auto_psv)) _SPI2ErrInterrupt(void)
{
    init_spi2();
}

// On SPI err, restart the module
void __attribute__ ((__interrupt__, auto_psv)) _SPI1ErrInterrupt(void)
{
    init_spi1();
}