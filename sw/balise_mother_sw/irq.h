#ifndef _IRQ_H
#define	_IRQ_H

#include "types.h"

#define IVT_MODE 0
#define AIVT_MODE 1
#define DEFAULT_PRIORITY 0x4444

void irq_off( void );
void irq_config( u8 mode );
void init_irq( void );

void trap( void );

#endif // _IRQ_H