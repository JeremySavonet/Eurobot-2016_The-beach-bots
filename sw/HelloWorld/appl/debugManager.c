/*
 * Debug manager module
 * Author: Jeremy S.
 */

#include "debugManager.h"

// Struct to config serial module for debug
static SerialConfig uartCfg =
{
    115200,
    0,
    0,
    0
};

void debugManagerInit( void )
{
    // Configure UART3 for debug 115200 8N1
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) );
    sdStart( &SD3, &uartCfg );
}

