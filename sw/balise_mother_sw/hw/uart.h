#ifndef _UART_H
#define _UART_H

#include <stdarg.h>
#include <stdio.h>

#include "pin.h"
#include "system.h"
#include "types.h"

#define UART_BAUDRATE 		(9600.0)

#define BAUDRATE_UART1_REG  (((FCY_FREQ)/(4*(u32)UART_BAUDRATE))-1)

#define UART2_ON()          U2MODEbits.UARTEN = 1;
#define UART2_OFF()         U2MODEbits.UARTEN = 0;

#define UART1_ON()          U1MODEbits.UARTEN = 1;
#define UART1_OFF()         U1MODEbits.UARTEN = 0;

// Global variables
extern volatile char U1BUFF[256];
extern volatile u8 U1BUFF_RDPTR;
extern volatile u8 U1BUFF_WRPTR;

// === PROTOTYPES ===
void init_uart( void );

// UART1
void uart1_putchar( char );
char uart1_is_pressed( void );
char uart1_getchar( void );

//UART2: DEBUG console_printf()
void uart2_putchar( char );
char uart2_is_pressed( void );
char uart2_getchar( void );
void uart2_power_off(void);

#endif //_UART_H