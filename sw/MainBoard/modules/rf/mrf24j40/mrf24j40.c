/*
 * MRF24J40 generic driver module for our robot platform
 * This module allow you to send and receive data over zigbee.
 *
 * This module is used to handle data coming from our beacon module.
 *
 * Author: Jeremy S
 * Rev: 1.0
 */

// Platform includes
#include "ch.h"
#include "hal.h"

// MRF24J40 includes
#include "mrf24J40.h"
#include "mrf24J40_registers.h"

// Debug includes
#include "../../../color.h"
#include "../../../comm/debug_manager.h"

//====================  Chibios SPI HAL Configuration  =======================//
//============================================================================//

// aMaxPHYPacketSize = 127, from the 802.15.4-2006 standard.
static uint8_t rx_buf[127];

// essential for obtaining the data frame only
// bytes_MHR = 2 Frame control + 1 sequence number + 2 panid + 2 shortAddr Destination + 2 shortAddr Source
const int bytes_MHR = 9;
const int bytes_FCS = 2; // FCS length = 2
#define bytes_nodata ( bytes_MHR + bytes_FCS ) // no_data bytes in PHY payload,  header length + FCS

static int ignoreBytes = 0; // bytes to ignore, some modules behaviour.

static bool bufPHY = false; // flag to buffer all bytes in PHY Payload, or not

volatile uint8_t flag_got_rx;
volatile uint8_t flag_got_tx;

static rx_info_t rx_info;
static tx_info_t tx_info;

// SPI1 configuration structure.
static const SPIConfig spi1_config = {
    NULL,           // callback
    GPIOE,          // CS port
    3,   // CS pin
    SPI_CR1_BR_0 | SPI_CR1_BR_1 // Max speed 21 MHz, 8bits frames, MSB first
};

// Functions to read/write on spi driver 1
uint8_t read_short( uint8_t reg ) // reg = address
{
    char txbuf[2] = {reg<<1 & 0x7E, 0xFF};
    char rxbuf[2];
    spiSelect( &SPID1 );
    spiExchange( &SPID1, 2, txbuf, rxbuf );
    spiUnselect( &SPID1 );
    return rxbuf[1];
}

uint8_t read_long( uint16_t reg )
{
    uint8_t ahigh = reg >> 3;
    uint8_t alow = reg << 5;
 
    char txbuf[3] = {0x80 | ahigh, alow, 0xFF};
    char rxbuf[3];
    spiSelect( &SPID1 );
    spiExchange( &SPID1, 3, txbuf, rxbuf );
    spiUnselect( &SPID1 );
    return rxbuf[1];
}

uint8_t write_short( uint8_t reg, uint8_t val )
{
    char txbuf[2] = {(reg<<1 & 0x7E) | 0x01, val};
    char rxbuf[2];
    spiSelect( &SPID1 );
    spiExchange( &SPID1, 2, txbuf, rxbuf );
    spiUnselect( &SPID1 );
    return rxbuf[1];
}

uint8_t write_long( uint16_t reg, uint8_t val )
{
    uint8_t ahigh = reg >> 3;
    uint8_t alow = reg << 5;
    char txbuf[3] = { 0x80 | ahigh, alow | 0x10, val};
    char rxbuf[3];
    spiSelect( &SPID1 );
    spiExchange( &SPID1, 3, txbuf, rxbuf );
    spiUnselect( &SPID1 );
    return rxbuf[1];
}

//============================================================================//
//=====================  MRF24J40 public API functions =======================//
//============================================================================//

/* 
 * Init function for mrf24j40 module.
 * Configure hw I/Os.
 * Start the HAL SPI1 driver
 * Configure mrf24j40 module
 */
void mrf24j40_init( void )
{
    // First, init the SPI1 driver
    spiStart( &SPID1, &spi1_config );

    // SPI1 I/O pins setup.
    palSetPadMode( GPIOE, 3, PAL_MODE_OUTPUT_PUSHPULL | 
            PAL_STM32_OSPEED_HIGHEST); // CS
    palSetPadMode( GPIOA, 5, PAL_MODE_ALTERNATE( 5 ) |
            PAL_STM32_OSPEED_HIGHEST ); // SCK
    palSetPadMode( GPIOA, 6, PAL_MODE_ALTERNATE( 5 ) |
            PAL_STM32_OSPEED_HIGHEST ); // MISO
    palSetPadMode( GPIOA, 7, PAL_MODE_ALTERNATE( 5 ) |
            PAL_STM32_OSPEED_HIGHEST ); // MOSI
    palSetPad( GPIOE, 3 );

    // Then, init mrf24j40 module through SPI
    mrf24j40_initialize();
}

void mrf24j40_reset( void )
{
    palClearPad( GPIOE, 7 ); // sets GPIOE 5 low
    chThdSleepMilliseconds( 10 );
    palSetPad( GPIOE, 7 ); // sets GPIOE 5 high
    chThdSleepMilliseconds( 10 );
}

void mrf24j40_initialize( void )
{
    mrf24j40_reset();
    write_short(MRF_PACON2, 0x98); // – Initialize FIFOEN = 1 and TXONTS = 0x6.
    write_short(MRF_TXSTBL, 0x95); // – Initialize RFSTBL = 0x9.

    write_long(MRF_RFCON0, 0x03); // – Initialize RFOPT = 0x03.
    write_long(MRF_RFCON1, 0x01); // – Initialize VCOOPT = 0x02.
    write_long(MRF_RFCON2, 0x80); // – Enable PLL (PLLEN = 1).
    write_long(MRF_RFCON6, 0x90); // – Initialize TXFIL = 1 and 20MRECVR = 1.
    write_long(MRF_RFCON7, 0x80); // – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
    write_long(MRF_RFCON8, 0x10); // – Initialize RFVCO = 1.
    write_long(MRF_SLPCON1, 0x21); // – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

    //  Configuration for nonbeacon-enabled devices (see Section 3.8 “Beacon-Enabled and
    //  Nonbeacon-Enabled Networks”):
    write_short(MRF_BBREG2, 0x80); // Set CCA mode to ED
    write_short(MRF_CCAEDTH, 0x60); // – Set CCA ED threshold.
    write_short(MRF_BBREG6, 0x40); // – Set appended RSSI value to RXFIFO.
    mrf24j40_set_interrupts();
    mrf24j40_set_channel(12);
    // max power is by default.. just leave it...
    // Set transmitter power - See “REGISTER 2-62: RF CONTROL 3 REGISTER (ADDRESS: 0x203)”.
    write_short(MRF_RFCTL, 0x04); //  – Reset RF state machine.
    write_short(MRF_RFCTL, 0x00); // part 2
    chThdSleepMilliseconds(2); // delay at least 192usec
}


uint16_t mrf24j40_get_pan( void ) 
{
    uint8_t panh = read_short( MRF_PANIDH );
    return panh << 8 | read_short( MRF_PANIDL );
}

void mrf24j40_set_pan( uint16_t panid ) 
{
    write_short( MRF_PANIDH, panid >> 8 );
    write_short( MRF_PANIDL, panid & 0xff );
}

void mrf24j40_address16_write( uint16_t address16 ) 
{
    write_short( MRF_SADRH, address16 >> 8 );
    write_short( MRF_SADRL, address16 & 0xff);
}

uint16_t mrf24j40_address16_read( void ) 
{
    uint8_t a16h = read_short( MRF_SADRH );
    return a16h << 8 | read_short( MRF_SADRL );
}

// Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
void mrf24j40_send16( uint16_t dest16, char * data ) 
{
    uint8_t len = strlen( data ); // get the length of the char* array
    int i = 0;

    write_long( i++, bytes_MHR ); // header length
    // +ignoreBytes is because some module seems to ignore 2 bytes after the header?!.
    // default: ignoreBytes = 0;
    write_long( i++, bytes_MHR + ignoreBytes + len );

    // 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
    write_long( i++, 0b01100001 ); // first byte of Frame Control
    
    // 16 bit source, 802.15.4 (2003), 16 bit dest,
    write_long( i++, 0b10001000 ); // second byte of frame control
    write_long( i++, 1 );  // sequence number 1

    uint16_t panid = mrf24j40_get_pan();

    write_long( i++, panid & 0xff );  // dest panid
    write_long( i++, panid >> 8 );
    write_long( i++, dest16 & 0xff );  // dest16 low
    write_long( i++, dest16 >> 8 ); // dest16 high

    uint16_t src16 = mrf24j40_address16_read();
    write_long( i++, src16 & 0xff ); // src16 low
    write_long( i++, src16 >> 8 ); // src16 high

    // All testing seems to indicate that the next two bytes are ignored.
    //2 bytes on FCS appended by TXMAC
    i += ignoreBytes;
    for( int q = 0; q < len; q++ ) 
    {
        write_long( i++, data[q] );
    }
    // ack on, and go!
    write_short( MRF_TXNCON, (1<<MRF_TXNACKREQ | 1<<MRF_TXNTRIG) );
}

void mrf24j40_set_interrupts( void ) 
{
    // interrupts for rx and tx normal complete
    write_short( MRF_INTCON, 0b11110110 );
}

// use the 802.15.4 channel numbers..
void mrf24j40_set_channel( uint8_t channel ) 
{
    write_long( MRF_RFCON0, (((channel - 11) << 4) | 0x03) );
}

/**
 * Call this from within an interrupt handler connected to the MRFs output
 * interrupt pin.  It handles reading in any data from the module, and letting it
 * continue working.
 * Only the most recent data is ever kept.
 */
void mrf24j40_interrupt_handler( void ) 
{
    uint8_t last_interrupt = read_short( MRF_INTSTAT );
    
    if( last_interrupt & MRF_I_RXIF ) 
    {
        flag_got_rx++;
        
        // read out the packet data...
        mrf24j40_rx_disable();
        
        // read start of rxfifo for, has 2 bytes more added by FCS. frame_length = m + n + 2
        uint8_t frame_length = read_long( 0x300 );

        // buffer all bytes in PHY Payload
        if( bufPHY )
        {
            int rb_ptr = 0;
            for( int i = 0; i < frame_length; i++ ) 
            { // from 0x301 to (0x301 + frame_length -1)
                rx_buf[rb_ptr++] = read_long( 0x301 + i );
            }
        }

        // buffer data bytes
        int rd_ptr = 0;
        
        // from (0x301 + bytes_MHR) to (0x301 + frame_length - bytes_nodata - 1)
        for( int i = 0; i < mrf24j40_rx_datalength(); i++ ) 
        {
            rx_info.rx_data[rd_ptr++] = read_long( 0x301 + bytes_MHR + i );
        }

        rx_info.frame_length = frame_length;
        
        // same as datasheet 0x301 + (m + n + 2) <-- frame_length
        rx_info.lqi = read_long( 0x301 + frame_length );
        
        // same as datasheet 0x301 + (m + n + 3) <-- frame_length + 1
        rx_info.rssi = read_long( 0x301 + frame_length + 1 );

        mrf24j40_rx_enable();
    }
    if( last_interrupt & MRF_I_TXNIF ) 
    {
        flag_got_tx++;
        uint8_t tmp = read_short( MRF_TXSTAT );
        // 1 means it failed, we want 1 to mean it worked.
        tx_info.tx_ok = !( tmp & ~(1 << TXNSTAT) );
        tx_info.retries = tmp >> 6;
        tx_info.channel_busy = ( tmp & (1 << CCAFAIL) );
    }
}

// Call this function periodically, it will invoke your nominated handlers
void mrf24j40_check_flags( void (*rx_handler)(void), void (*tx_handler)(void) )
{
    // TODO - we could check whether the flags are > 1 here, indicating data was lost?
    if( flag_got_rx ) 
    {
        flag_got_rx = 0;
        rx_handler();
    }
    if( flag_got_tx ) 
    {
        flag_got_tx = 0;
        tx_handler();
    }
}

// Set RX mode to promiscuous, or normal
void mrf24j40_set_promiscuous( bool enabled ) 
{
    if( enabled ) 
    {
        write_short( MRF_RXMCR, 0x01 );
    } 
    else
    {
        write_short( MRF_RXMCR, 0x00 );
    }
}

rx_info_t * mrf24j40_get_rxinfo( void ) 
{
    return &rx_info;
}

tx_info_t * mrf24j40_get_txinfo( void ) 
{
    return &tx_info;
}

uint8_t * mrf24j40_get_rxbuf( void ) 
{
    return rx_buf;
}

int mrf24j40_rx_datalength( void ) 
{
    return rx_info.frame_length - bytes_nodata;
}

void mrf24j40_set_ignoreBytes( int ib ) 
{
    // some modules behaviour
    ignoreBytes = ib;
}

// Set bufPHY flag to buffer all bytes in PHY Payload, or not
void mrf24j40_set_bufferPHY( bool bp ) 
{
    bufPHY = bp;
}

bool mrf24j40_get_bufferPHY( void ) 
{
    return bufPHY;
}

// Set PA/LNA external control
void mrf24j40_set_palna( bool enabled ) 
{
    if( enabled ) 
    {
        write_long( MRF_TESTMODE, 0x07 ); // Enable PA/LNA on MRF24J40MB module.
    }
    else
    {
        write_long( MRF_TESTMODE, 0x00 ); // Disable PA/LNA on MRF24J40MB module.
    }
}

void mrf24j40_rx_flush( void ) 
{
    write_short( MRF_RXFLUSH, 0x01 );
}

void mrf24j40_rx_disable( void ) 
{
    write_short( MRF_BBREG1, 0x04 );  // RXDECINV - disable receiver
}

void mrf24j40_rx_enable( void ) 
{
    write_short( MRF_BBREG1, 0x00 );  // RXDECINV - enable receiver
}
