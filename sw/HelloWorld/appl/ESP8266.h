/*
 *  ESP8266 module driver.
 *  Talks to a ESP8266 mod-WiFi module over serial 
 */

#include "ch.h"
#include "hal.h"

#ifndef _ESP8266_H_
#define _ESP8266_H_

// Public functions
// TODO : Handle response message. Add arguments to functions when needed
void ESP8266Init( void );
void ESP8266RequestVersion( void );
void ESP8266ListAccessPoint( void );
void ESP8266JoinAccessPoint( void );
void ESP8266SetMode( void );
void ESP8266SetAccessPoint( void );
void ESP8266EnableMultipleConnection( void );
void ESP8266ConfigureServer( void );

#endif /* _ESP8266_H_ */
