/*
 * Some usefull function to debug
 */
#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

// Static functions to print on debug uart SD3
void print( char *p ) 
{
    while (*p) chSequentialStreamPut( &SD3, *p++ );
}
