/*
 * Manage board structure here
 *
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

// Enum of state of the system
typedef enum
{
    SYSTEM_UNKNOWN,         // System is in unknown state
    SYSTEM_INIT,            // System is about to initialize
    SYSTEM_WORKING,         // System is about to working
    SYSTEM_CRASH            // System is crashed
} SYSTEM_STATE;

typedef struct
{
    //Add here all sensor strctures
    
} SYSTEM_SENSORS;

typedef struct
{
    //Add here all comm structure(shell, uart, can, spi, i2c...)
} SYSTEM_COMM;

typedef struct
{
    //Add here all control structures

} SYSTEM_CONTROLS;

typedef struct
{
    // Aggregate here all robot structures
    SYSTEM_COMM     comm;
    SYSTEM_SENSORS  sensors;
    SYSTEM_CONTROLS controls;

} SYSTEM;

extern SYSTEM sys;

void systemPrintBootMsg( void );
void debugManagerInit( void );
void initSystem( void );

#endif // _SYSTEM_h
