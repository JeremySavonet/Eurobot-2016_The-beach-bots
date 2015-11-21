/*
 *  esp8266 module driver.
 *  Talks to a esp8266 mod-WiFi module over serial
 *
 *  Author: JS
 *  Date: 2015-10-25
 *  Version: V0.1
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "../color.h"
#include "../comm/debug_manager.h"

#include "esp8266_manager.h"

// Private functions
void send_command( char *p );

// Struct to config serial module 
static SerialConfig sd6_cfg =
{
    115200,              
    0,
    0,
    0
}; 

// Periodic thread to handle modwifi uart
static THD_WORKING_AREA( waRead, 128 );
static THD_FUNCTION( esp8266, arg )
{
    (void)arg;
    chRegSetThreadName( "esp8266" );
    event_listener_t serialListener;

    /* Registering on the serial driver 6 as event 1, interested in
     * error flags and data-available only, other flags will not wakeup
     * the thread.
     */
    chEvtRegisterMaskWithFlags(
        (struct event_source_t *)chnGetEventSource( &SD6 ),
        &serialListener,
        EVENT_MASK( 1 ),
        SD_FRAMING_ERROR | SD_PARITY_ERROR |
        CHN_INPUT_AVAILABLE );

    while( true )
    {
        // Waiting for any of the events we're registered on.
        eventmask_t evt = chEvtWaitAny( ALL_EVENTS );
 
        // Serving events.
        if( evt & EVENT_MASK(1) ) 
        {
          /* Event from the serial interface, getting serial
           * flags as first thing.
           */
          eventflags_t flags = chEvtGetAndClearFlags( &serialListener );
 
            //Handling errors first.
            if( flags & (SD_FRAMING_ERROR | SD_PARITY_ERROR) )
            {   
                DPRINT( 4, KRED "FRAMING/PARITY ERROR" );
            }
            if( flags & CHN_INPUT_AVAILABLE )
            {
                char c; 
                c = sdGet( &SD6 );
                sdPut( &SD3, c );
            }   
        }  
    }
}

void esp8266_manager_init( void )
{
    // Used function : USART6_TX 
    palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE( 8 ) ); 
    palSetPadMode( GPIOC, 7, PAL_MODE_ALTERNATE( 8 ) ); 
    
    sdStart( &SD6, &sd6_cfg );
    
    chThdCreateStatic( waRead, sizeof(waRead), NORMALPRIO, esp8266, NULL );
}

void esp8266_request_version( void )
{
    send_command( "AT+GMR\r\n" );
}

void esp8266_list_AP( void )
{
    send_command( "AT+CWLAP\r\n" );
}

void esp8266_set_mode( void )
{
    send_command( "AT+CWMODE=3\r\n" );
}

void esp8266_join_AP( void )
{
    send_command( "AT+CWJAP=\"ssid\",\"passord\"\r\n" );
}

void esp8266_setAP( void )
{
    send_command( "AT+CWSAP=\"esp_123\",\"1234567890\",5,3\r\n" );
}

void esp8266_enable_multiple_connection( void )
{
    send_command( "AT+CIPMUX=1\r\n" );
}

void esp8266_configure_server( void )
{
    send_command( "AT+CIPSERVER=1\r\n" );
}

void send_command( char *p ) 
{
    while (*p) chSequentialStreamPut( &SD6, *p++ );
}
