/*
 *  Console module used to debug
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define DEBUG
#define DLEVEL 3
#ifdef DEBUG
#define DPRINT( level, fmt )     if (DLEVEL >= (level)) print( fmt"\r\n" )
#else
#define DPRINT( level, fmt )
#endif

// Public functions
void print( char *p );

#endif /* _CONSOLE_H_ */
