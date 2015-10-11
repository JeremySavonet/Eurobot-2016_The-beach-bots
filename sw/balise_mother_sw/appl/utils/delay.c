#include "system.h"

void delay_us( u16 delay )
{
    s32 prev_timer1, timer1, end_time, time, prev_time;

    prev_timer1 = (s32) TMR1;
    end_time = prev_timer1 + (s32) (((u32) delay) << 1);

    time = prev_time = prev_timer1;
    
    while( time < end_time ) 
    {
        timer1 = (s32) TMR1;
        time += timer1 - prev_timer1;
        if (time < prev_time)
            time += (s32) PR1;
        prev_time = time;
        prev_timer1 = timer1;
    }
}

void delay_ms( u16 delay )
{
    for( ; delay; delay-- )
    {    
        delay_us(1000);
    }
}