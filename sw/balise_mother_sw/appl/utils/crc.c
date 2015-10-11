#include <stdint.h>
#include <stdlib.h>
#include "crc.h"
#include "console.h"

u16 crc_calc( const char *data, u8 len )
{
    u16 crc = 0xffff;
    u8 c;
    const u8 *p = (const u8 *)data;
    
    while( --len ) 
    {
        c = *p++;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
        c >>= 4;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
    }
    
    return ~crc & 0xffff;
}
