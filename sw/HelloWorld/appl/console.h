/*
 *  Console module used to debug
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "chprintf.h"
#include "hal.h"
#include "test.h"

#define DEBUG
#define DLEVEL 3
#ifdef DEBUG
#define DPRINT( level, ... ) \
    do { \
        if( DLEVEL >= (level) ) \
            chprintf( (BaseSequentialStream * )&SD3, __VA_ARGS__); \
   } while (0)
#else
#define DPRINT( level, ... )
#endif

#endif // _CONSOLE_H_
