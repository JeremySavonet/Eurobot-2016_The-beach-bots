/*
 * Manage board structure here
 *
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

typedef struct
{
    //Add here all sensor strctures
    
} SYSTEM_SENSORS;

typedef struct
{
    //Add here all control structures

} SYSTEM_CONTROLS;

typedef struct
{
    // Aggregate here all robot structures
    SYSTEM_SENSORS  sensors;
    SYSTEM_CONTROLS controls;

} SYSTEM;

extern SYSTEM sys;

#endif // _SYSTEM_h
