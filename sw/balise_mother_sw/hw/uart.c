#include "console.h"
#include "delay.h"
#include "p24FJ128GA306.h"
#include "pin.h"
#include "system.h"
#include "types.h"
#include "uart.h"

volatile char U1BUFF[256];
volatile u8 U1BUFF_RDPTR = 0;
volatile u8 U1BUFF_WRPTR = 0;

void init_uart1( void );
void init_uart2( void );
void uart2_power_off( void );

void init_uart( void )
{
    init_uart1();   
    
    // Console debug
    init_uart2();               
}

//UART 1
void init_uart1( void )
{
    u16 i;

    PMD1bits.U1MD = 0;
    Nop();
    U1STA = 0;
    U1MODE = 0;

    U1MODEbits.BRGH = 1;        // High Speed mode
    U1BRG = BAUDRATE_UART1_REG; // 4.0 divisor corresponds to BRGH=1

    U1MODEbits.UARTEN = 1;      // Enable UART; must be set before U1STAbits.UTXEN
    U1STAbits.UTXEN = 1;        // Enable UART Tx
    
    // wait at least one bit time before sending first char
    for( i = 0; i < (u16) (FCY_FREQ / UART_BAUDRATE); i++ )
    {
        asm("nop");
    }
    
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    IFS4bits.U1ERIF = 0;
}

void uart1_putchar( char c )
{
    u16 time_out1 = 0, time_out2 = 0;
    
    // Wait until TX buff is full
    while( U1STAbits.UTXBF == 1 )        
    {
        time_out1++;
        if( time_out1 > 1000 ) 
        {
            time_out1 = 0;
            time_out2++;
        }
        if( time_out2 > 3000 ) 
        {
            break;
        }
    }
    
    // Uart1 transmit enable
    U1STAbits.UTXEN = 1;        
    U1TXREG = c;
    
    // Wait until TX buff is empty to resolve datasheet errata
    while( !IFS0bits.U1TXIF );  
    
    // reset flag
    IFS0bits.U1TXIF = 0;        
}

char uart1_is_pressed( void )
{
    if( IFS0bits.U1RXIF == 1 )
    {
        return 1;
    }
    
    return 0;
}

char uart1_getchar( void )
{
    char temp;

    if( U1STAbits.URXDA == 0 )
    {
        return 0;
    } 
    else
    {
        temp = U1RXREG;
        IFS0bits.U1RXIF = 0;
    
        return temp;
    }
}

//UART 2
void init_uart2( void )
{
    u16 i;

    PMD1bits.U2MD = 0;
    Nop();
    U2STA = 0;
    U2MODE = 0;

    U2MODEbits.BRGH = 1;        // High Speed mode
    U2BRG = BAUDRATE_UART1_REG; // 4.0 divisor corresponds to BRGH=1

    U2MODEbits.UARTEN = 1;      // Enable UART; must be set before U1STAbits.UTXEN
    U2STAbits.UTXEN = 1;        // Enable UART Tx
    
    // wait at least one bit time before sending first char
    for( i = 0; i < (u16) (FCY_FREQ / UART_BAUDRATE); i++ )
    {
        asm("nop");
    }
    
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    IFS4bits.U2ERIF = 0;
}

void uart2_putchar( char Ch )
{
    u16 time_out1 = 0, time_out2 = 0;

    while( U2STAbits.UTXBF == 1 ) 
    {
        time_out1++;

        if( time_out1 > 1000 ) 
        {
            time_out1 = 0;
            time_out2++;
        }
        if( time_out2 > 3000 ) 
        {
            break;
        }
    }
    
    U2STAbits.UTXEN = 1;
    U2TXREG = Ch;
    
    while( !IFS1bits.U2TXIF );
    
    IFS1bits.U2TXIF = 0;
}

char uart2_is_pressed( void )
{
    if (IFS1bits.U2RXIF == 1 )
    {
        return 1;
    }
    
    return 0;
}

char uart2_getchar( void )
{
    char Temp;

    if( U2STAbits.URXDA == 0 )
    {
        return 0;
    } 
    else
    {
        Temp = U2RXREG;
        IFS1bits.U2RXIF = 0;
        
        return Temp;
    }
}

void uart2_power_off( void )
{
    while( U2STAbits.TRMT == 0 );
    
    U2MODEbits.UARTEN = 0;
    TRISBbits.TRISB5 = 0;
    IEC1bits.U2RXIE = 0;
}