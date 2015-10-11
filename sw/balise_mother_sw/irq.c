#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc.h"
#include "ADXL362.h"
#include "console.h"
#include "delay.h"
#include "global_variables.h"
#include "gpio.h"
#include "irq.h"
#include "pwm.h"
#include "system.h"

void irq_off( void )
{
    volatile unsigned int *reg_addr;

    // Clear all IRQ control/status registers
    INTCON1 = 0;
    INTCON2 = 0;
    INTTREG = 0;

    // Reset all IECx and IFSx registers
    for( reg_addr = &IFS0; reg_addr <= &IEC7; reg_addr++ )
    {
        *reg_addr = 0;
    }
    
    // Reset priority to zero for all IPCx registers
    for( reg_addr = &IPC0; reg_addr <= &IPC29; reg_addr++ )
    {
        *reg_addr = DEFAULT_PRIORITY;
    }
}

void irq_config( u8 mode )
{
    // INTCON1
    // nested IRQ ON
    INTCON1bits.NSTDIS = 0;     

    // INTCON2
    // DISI instruction is active
    INTCON2bits.DISI = 1;       

    switch( mode )
    {
        case AIVT_MODE:
            // AIVT for BL
            INTCON2bits.ALTIVT = 1;     
            break;
            
        case IVT_MODE:
        default:
            // IVT for APPL
            INTCON2bits.ALTIVT = 0;    
            break;
    }
}

void init_irq( void )
{
    // Disable all user interrupts
    SRbits.IPL = 7;             

    //IRQ T1 priority
    IPC0bits.T1IP = 4;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    //IRQ T2 priority
    // IRQ TMR2 in priority level 2 group
    IPC1bits.T2IP = 2; 
    // Reset IRQ TMR2 flag
    IFS0bits.T2IF = 0;          
    IEC0bits.T2IE = 1;

    //IRQ T3 priority
    // IRQ TMR3 priority level 4 group
    IPC2bits.T3IP = 4;  
    // Reset IRQ TMR3 flag
    IFS0bits.T3IF = 0;          
    IEC0bits.T3IE = 1;

    // ADXL IRQ
    IFS0bits.INT0IF = 0;
    IPC0bits.INT0IP = 1;
    IEC0bits.INT0IE = 0;
    INTCON2bits.INT0EP = 0;

    // UART IRQ
    IPC16bits.U1ERIP = 5;
    IFS4bits.U1ERIF = 0;
    IEC4bits.U1ERIE = 1;

    // Clear UART1 Receive and Transmit interrupt flags
    IFS0bits.U1RXIF = 0;
    // Interrupt level of reception
    IPC2bits.U1RXIP = 5;        
    // Enable UART1 Receive interrupt
    IEC0bits.U1RXIE = 1;

    IFS1bits.U2RXIF = 0;
    // Interrupt level of reception
    IPC7bits.U2RXIP = 5;        
    // Enable UART2 Receive interrupt
    IEC1bits.U2RXIE = 1;

    IPC16bits.U2ERIP = 5;
    IFS4bits.U2ERIF = 0;
    IEC4bits.U2ERIE = 1;

    ClrWdt();
    
    // Enable all user interrupts
    SRbits.IPL = 0;             
}

// UART1 Receive interupt handler : Manage BC118 response
void __attribute__ ((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    static u8 i = 0;
    
    // buffer size is 256 so i will wrap
    IFS0bits.U1RXIF = 0;

    while( U1STAbits.URXDA ) 
    {
        U1BUFF[i] = U1RXREG;
        
        if( U1BUFF[i] == '\r' )
        {
            U1BUFF_WRPTR = i + 1;
        }
       
        i++;
    }
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U1ErrInterrupt(void)
{
    U1STAbits.PERR = 0;
    U1STAbits.FERR = 0;
    U1STAbits.OERR = 0;

    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    IFS4bits.U1ERIF = 0;
}

// UART2 : Receive interupt handler : Debug printf
void __attribute__ ((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
    u8 c;
    static u8 i = 0;            
    static char buff[32]; 

    IFS1bits.U2RXIF = 0;

    while( U2STAbits.URXDA ) 
    {
        c = U2RXREG;
        
        if( c == '\r' )
        {
            // Ending the buffer
            buff[(sizeof(buff) - 1)] = '\0';   
            // Flushing the buffer
            memset( buff, 0, sizeof( buff ) );      
            i = 0;              
        } 
        else 
        {
            buff[i++] = c;
            
            // Pic can't handle the message
            if( i >= (sizeof(buff) - 1) ) 
            {       
                i = (sizeof(buff) - 1);
                // Flushing the buffer
                memset( buff, 0, sizeof( buff ) ); 
            }
        }
    }
}

void __attribute__ ((__interrupt__, no_auto_psv)) _U2ErrInterrupt(void)
{
    U2STAbits.PERR = 0;
    U2STAbits.FERR = 0;
    U2STAbits.OERR = 0;

    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    IFS4bits.U2ERIF = 0;
}

// Period 1ms
void __attribute__ ((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    // Clear watchdog
    ClrWdt();                   
    IFS0bits.T1IF = 0;
}

/*** IRQ priority 3 ***/
void __attribute__ ((__interrupt__, __no_auto_psv__)) _T2Interrupt(void)
{                               // IRQ @ 33 ms
    IFS0bits.T2IF = 0;
}

void __attribute__ ((__interrupt__, __no_auto_psv__)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
}

//IRQ to manage INT0 ADXL362
void _ISRFAST __attribute__ ((interrupt, auto_psv)) _INT0Interrupt(void)
{
    Nop();
    IFS0bits.INT0IF = 0;
}

//IRQ to manage INT2 ADXL362
void _ISRFAST __attribute__ ((interrupt, auto_psv)) _INT3Interrupt(void)
{
    IFS3bits.INT3IF = 0;
}

// In case of unexpected IRQ, stop properly
void trap( void )
{
    SRbits.IPL = 7;
    ClrWdt();
    asm( "RESET" );
}

void __attribute__ ((__interrupt__, auto_psv)) _ReservedTrap0(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _OscillatorFail(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _AddressError(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _StackError(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _MathError(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _ReservedTrap5(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _ReservedTrap6(void)
{
    trap();
}

void __attribute__ ((__interrupt__, auto_psv)) _ReservedTrap7(void)
{
    trap();
}