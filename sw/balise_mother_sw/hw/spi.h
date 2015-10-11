#ifndef _SPI_H
#define	_SPI_H

#include "types.h"

void init_spi( void );

u8 spi1_u8_write( u8 data );
u8 spi2_u8_write( u8 data );

#endif //_SPI_H