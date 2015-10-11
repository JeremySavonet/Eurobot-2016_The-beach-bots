#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "uart.h"

// === PROTOTYPE ===
void console_printf(const char *format, ...);

#define DEBUG
#define DLEVEL 2
#ifdef DEBUG
#define DPRINT(level, fmt, args...)     if (DLEVEL >= (level)) console_printf(fmt"\r\n", ##args)
#else
#define DPRINT(level, fmt, args...)
#endif

// UART 1
#define CONSOLE_init1()	 init_uart1()
#define ConsolePut1( x ) uart1_putchar( x )
#define ConsoleGet1()	 uart1_getchar()
#define ConsoleFull1()	 uart1_is_pressed()

// UART 2 : CONSOLE DEBUG
#define CONSOLE_init2()	 init_uart2()
#define ConsolePut2( x ) uart2_putchar( x )
#define ConsoleGet2()	 uart2_getchar()
#define ConsoleFull2()	 uart2_is_pressed()

#endif //_CONSOLE_H