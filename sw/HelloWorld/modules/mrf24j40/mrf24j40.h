/*
 * MRF24J40 generic driver module for our robot platform
 * This module allow you to send and receive data over zigbee.
 *
 * This module is used to handle data coming from our beacon module.
 *
 * Author: Jeremy S
 * Rev: 1.0
 */

#ifndef _MRF24J40_H_
#define _MRF24J40_H_

#include <stdint.h>
#include <stdbool.h>

typedef int8_t device_address_t[8];
typedef int8_t pan_id_t[2];

typedef struct _rx_info_t
{
    uint8_t frame_length;
    uint8_t rx_data[116]; //max data length = (127 aMaxPHYPacketSize - 2 Frame control - 1 sequence number - 2 panid - 2 shortAddr Destination - 2 shortAddr Source - 2 FCS)
    uint8_t lqi;
    uint8_t rssi;
} rx_info_t;

// Based on the TXSTAT register, but "better"
typedef struct _tx_info_t
{
    uint8_t tx_ok:1;
    uint8_t retries:2;
    uint8_t channel_busy:1;
} tx_info_t;

// Read/write control register functions
uint8_t read_short( uint8_t address );
uint8_t read_long( uint16_t address );

uint8_t write_short( uint8_t address, uint8_t data );
uint8_t write_long( uint16_t address, uint8_t data );

// MRF24J40 public API
void mrf24j40_init( void );
void mrf24j40_reset( void );
void mrf24j40_initialize( void );

uint16_t mrf24j40_get_pan( void );
void mrf24j40_set_pan( uint16_t panid );

void mrf24j40_address16_write( uint16_t address16 );
uint16_t mrf24j40_address16_read( void );

void mrf24j40_set_interrupts( void );

void mrf24j40_set_promiscuous( bool enabled );

// Set the channel, using 802.15.4 channel numbers (11..26)
void mrf24j40_set_channel( uint8_t channel );

void mrf24j40_rx_enable( void );
void mrf24j40_rx_disable( void );

/** If you want to throw away rx data */
void mrf24j40_rx_flush( void );

rx_info_t * mrf24j40_get_rxinfo( void );

tx_info_t * mrf24j40_get_txinfo( void );

uint8_t * mrf24j40_get_rxbuf( void );

int mrf24j40_rx_datalength( void );

void mrf24j40_set_ignoreBytes( int ib );

// Set bufPHY flag to buffer all int8_ts in PHY Payload, or not
void mrf24j40_set_bufferPHY( bool bp );

bool mrf24j40_get_bufferPHY( void );

// Set PA/LNA external control
void mrf24j40_set_palna( bool enabled );

void mrf24j40_send16( uint16_t dest16, char * data );

void mrf24j40_interrupt_handler( void );

void mrf24j40_check_flags( void (*rx_handler)(void), void (*tx_handler)(void) );

#endif // _MRF24J40_H_
