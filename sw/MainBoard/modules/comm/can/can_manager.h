/*
 *  CAN comm module driver.
 *  Allows the user to exchange data over CAN bus.
 *
 *  Author: JS
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#ifndef _CAN_MANAGER_H_
#define _CAN_MANAGER_H_

#include <stdint.h>

void can_manager_init( void );
void can_transmit( uint32_t id, uint8_t *data, uint8_t len );

#endif // _CAN_MANAGER_H_
