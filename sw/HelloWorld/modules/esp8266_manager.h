/*
 *  ESP8266 module driver.
 *  Talks to a ESP8266 mod-WiFi module over serial 
 */

#ifndef _ESP8266_H_
#define _ESP8266_H_

// Public functions
// TODO : Handle response message. Add arguments to functions when needed
void esp8266_manager_init( void );
void esp8266_request_version( void );
void esp8266_list_AP( void );
void esp8266_join_AP( void );
void esp8266_set_mode( void );
void esp8266_set_AP( void );
void esp8266_enable_multiple_connection( void );
void esp8266_configure_server( void );

#endif /* _ESP8266_H_ */
