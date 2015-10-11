#ifndef _GLOBAL_VARIABLES_H
#define	_GLOBAL_VARIABLES_H

#include "utils/types.h"

extern volatile u8 g_adxl_mask;
extern volatile u16 g_duty_cycle;

void global_variables_init( void );

#endif //_GLOBAL_VARIABLES_H