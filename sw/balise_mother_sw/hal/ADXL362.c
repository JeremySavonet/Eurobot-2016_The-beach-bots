#include <stdlib.h>
#include "adxl362.h"
#include "console.h"
#include "delay.h"
#include "global_variables.h"

// === GLOBAL VARIABLES ===
volatile s8 g_adxl_x = 0;
volatile s8 g_adxl_y = 0;
volatile s8 g_adxl_z = 0;

// === PROTOTYPES ===
void adxl362_write( u8 addr, u8 value );
u8 adxl362_read( u8 addr );

void adxl362_init( void )
{
    ADXL_CSn = 1;
    ADXL_3V3_EN = 1;
    delay_ms(10);

    adxl362_write( ACT_INACT_CTL, 0x00 );
    DPRINT( 4, "ACT_INACT_CTL : %02x", adxl362_read( ACT_INACT_CTL ) );
    
    // Set FIFO in stream mode
    adxl362_write( FIFO_CONTROL, 0x00 );
    DPRINT( 4, "FIFO_CONTROL : %02x", adxl362_read( FIFO_CONTROL ) );
    
    // Set threashold
    adxl362_write( THRESH_ACT_L, 0x10 );
    DPRINT( 4, "THRESH_ACT_L : %02x", adxl362_read( THRESH_ACT_L ) );

    adxl362_write( THRESH_ACT_H, 0x00 );
    DPRINT( 4, "THRESH_ACT_H : %02x", adxl362_read( THRESH_ACT_H ) );

    adxl362_write( THRESH_INACT_L, 0x50 );
    DPRINT( 4, "THRESH_INACT_L : %02x", adxl362_read( THRESH_INACT_L ) );

    adxl362_write( THRESH_INACT_H, 0x00 );
    DPRINT( 4, "THRESH_INACT_H : %02x", adxl362_read( THRESH_INACT_H ) );

    adxl362_write( TIME_INACT_L, 0x01 );
    DPRINT( 4, "TIME_INACT_L : %02x", adxl362_read( TIME_INACT_L ) );

    adxl362_write( ACT_INACT_CTL, 0x3F );
    DPRINT( 4, "ACT_INACT_CTL : %02x", adxl362_read( ACT_INACT_CTL ) );

    // Set Option for Interupt 1 pin for DATA_Ready signal
    adxl362_write( INTMAP1, 0x00 );
    DPRINT( 4, "INTMAP1 : %02x", adxl362_read( INTMAP1 ) );

    // Set Option for Interupt 2 pin for Activity signal
    adxl362_write( INTMAP2, 0x00 );
    DPRINT( 4, "INTMAP2 : %02x", adxl362_read( INTMAP2 ) );

    // Set Option for smaple rate
    adxl362_write( FILTER_CTL, 0x03 );
    DPRINT( 4, "FILTER_CTL : %02x", adxl362_read( FILTER_CTL ) );

    // Enable Measurment
    adxl362_write( POWER_CTL, 0x0A );
    DPRINT( 4, "POWER_CTL : %02x", adxl362_read( POWER_CTL ) );

    // Display info
    DPRINT( 4, "REG0 : %02X", adxl362_read( DEVID_AD ) );
    DPRINT( 4, "REG1 : %02X", adxl362_read( DEVID_MST ) );
    DPRINT( 4, "REG2 : %02X", adxl362_read( PARTID ) );
    DPRINT( 4, "REG3 : %02X", adxl362_read( REVID ) );

    DPRINT( 4, "adxl begin measurement..." );
    DPRINT( 4, "STATUS : %02X", adxl362_read( STATUS ) );

    g_adxl_mask = 10;           // Mask ADXL read after init
}

void adxl362_power_off( void )
{
    g_adxl_mask = 200;
    ADXL_3V3_EN = 0;
    ADXL_CSn = 0;
}

// Write :
// </CS down> <command byte 0X0A> <address byte> <additional data bytes> </CS up>
void adxl362_write( u8 addr, u8 value )
{
    TRISFbits.TRISF3 = 1;
    //SPI1STATbits.SPIEN = 1;

    ADXL_CSn = 0;
    spi2_u8_write(0x0A);
    spi2_u8_write(addr);
    spi2_u8_write(value);
    ADXL_CSn = 1;

    //SPI1STATbits.SPIEN = 0;
    TRISFbits.TRISF3 = 0;
    _LATF3 = 0;
}

// Read :
// </CS down> <command byte 0x0B> <address byte> <additional data bytes> </CS up>
u8 adxl362_read( u8 addr )
{
    u8 regValue = 0;

    TRISFbits.TRISF3 = 1;

    ADXL_CSn = 0;
    spi2_u8_write(0x0B);
    spi2_u8_write(addr);

    regValue = spi2_u8_write(0);
    ADXL_CSn = 1;

    TRISFbits.TRISF3 = 0;
    _LATF3 = 0;
    return regValue;
}

void adxl362_getXYZ8( s8 * x, s8 * y, s8 * z )
{
    TRISFbits.TRISF3 = 1;

    ADXL_CSn = 0;
    spi2_u8_write(0x0B);
    spi2_u8_write(XDATA);

    *x = (s8) spi2_u8_write(0);
    *y = (s8) spi2_u8_write(0);
    *z = (s8) spi2_u8_write(0);

    ADXL_CSn = 1;

    TRISFbits.TRISF3 = 0;
    _LATF3 = 0;
}

s8 adxl362_getXData( void )
{
    return adxl362_read( XDATA );
}

s8 adxl362_getYData( void )
{
    return adxl362_read( YDATA );
}

s8 adxl362_getZData( void )
{
    return adxl362_read( ZDATA );
}