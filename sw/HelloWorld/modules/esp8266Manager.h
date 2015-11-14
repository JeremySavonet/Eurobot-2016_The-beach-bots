/*
 *  ESP8266 module driver.
 *  Talks to a ESP8266 mod-WiFi module over serial 
 */

#ifndef _ESP8266_H_
#define _ESP8266_H_

// Public functions
// TODO : Handle response message. Add arguments to functions when needed
void esp8266ManagerInit( void );
void esp8266RequestVersion( void );
void esp8266ListAccessPoint( void );
void esp8266JoinAccessPoint( void );
void esp8266SetMode( void );
void esp8266SetAccessPoint( void );
void esp8266EnableMultipleConnection( void );
void esp8266ConfigureServer( void );

#endif /* _ESP8266_H_ */
