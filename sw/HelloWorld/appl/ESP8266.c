/*
 *  ESP8266 module driver.
 *  Talks to a ESP8266 mod-WiFi module over serial
 *
 *  Author: JS
 *  Date: 2015-10-25
 *  Version: V0.1
 */

#include <stdio.h>
#include <string.h>

#include "color.h"
#include "console.h" 
#include "ESP8266.h"
#include "test.h"


// Private functions
void sendCommand( char *p );


// Struct to config serial module 
static SerialConfig sd6Cfg =
{
    115200,              
    0,
    0,
    0
}; 

// Periodic thread to handle modwifi uart
static THD_WORKING_AREA( waRead, 128 );
static THD_FUNCTION( ESP8266, arg )
{
    (void)arg;
    chRegSetThreadName( "ESP8266" );
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

void ESP8266Init( void )
{
    // Used function : USART6_TX 
    palSetPadMode( GPIOC, 6, PAL_MODE_ALTERNATE( 8 ) ); 
    palSetPadMode( GPIOC, 7, PAL_MODE_ALTERNATE( 8 ) ); 
    
    sdStart( &SD6, &sd6Cfg );
    
    chThdCreateStatic( waRead, sizeof(waRead), NORMALPRIO, ESP8266, NULL );
}

void ESP8266RequestVersion( void )
{
    sendCommand( "AT+GMR\r\n" );
}

void ESP8266ListAccessPoint( void )
{
    sendCommand( "AT+CWLAP\r\n" );
}

void ESP8266SetMode( void )
{
    sendCommand( "AT+CWMODE=3\r\n" );
}

void ESP8266JoinAccessPoint( void )
{
    sendCommand( "AT+CWJAP=\"ssid\",\"passord\"\r\n" );
}

void ESP8266SetAccessPoint( void )
{
    sendCommand( "AT+CWSAP=\"esp_123\",\"1234567890\",5,3\r\n" );
}

void ESP8266EnableMultipleConnection( void )
{
    sendCommand( "AT+CIPMUX=1\r\n" );
}

void ESP8266ConfigureServer( void )
{
    sendCommand( "AT+CIPSERVER=1\r\n" );
}

void sendCommand( char *p ) 
{
    while (*p) chSequentialStreamPut( &SD6, *p++ );
}
