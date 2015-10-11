#include "timer.h"

void init_timer1( void );
void init_timer2( void );
void init_timer3( void );

void init_timers( void )
{
    init_timer1();
    init_timer2();
    init_timer3();
}

void init_timer1( void )
{
    // TIMER1 based on FCY_FREQ (16.0 MHz) divided by 8 => 2.0 MHz
    PMD1bits.T1MD = 0;
    
    // reset T1 config
    T1CON = 0;                  
    
    // Appli calculated period
    PR1 = (u16) ((TIMER1_PERIOD / (8.0 / FCY_FREQ)) - 1.0);     

    // LPRC Oscillator
    T1CON = 0b1000000000010000;        
}

void init_timer2( void )
{
    PMD1bits.T2MD = 0;

    T2CON = 0;
    PR2 = 625;

    T2CON = 0b1000000000110000;
}

//Use for LED PWM
void init_timer3( void )
{
    PMD1bits.T3MD = 0;

    T3CON = 0;
    PR3 = (u16) ((TIMER3_PERIOD / (8.0 / FCY_FREQ)) - 1.0);

    T3CON = 0b1000000000010000;
}