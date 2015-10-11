#ifndef _BATTERY_H
#define	_BATTERY_H

#include "system.h"

// === Low_Battery threshold ===
#define     LOW_BAT_VALUE       ((u16)(1.24*4096.0/2.5))
#define     ULTRA_LOW_BAT_VALUE ((u16)(1.24*4096.0/2.35))

// === GLOBAL VARIABLES ===
extern volatile u8 g_batt_state_level;

u16 battery_state( void );

#endif //_BATTERY_H