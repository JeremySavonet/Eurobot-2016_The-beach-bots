#ifndef _SYSTEM_H
#define	_SYSTEM_H

#include <p24FJ128GA306.h>

#include "pin.h"
#include "types.h"

//=== FLAGS ===
#define WDT_DISABLED	0
#define WDT_ENABLED	1

//=== uC CLOCK CONSTANTS ===
#define FRC_FREQ       (8.0e6)          // FRC Fast RC clock frequency [Hz]
#define FOSC_FREQ      (4.0*FRC_FREQ)   // FOSC clock frequency [Hz] (32.0 MHz)
#define FCY_FREQ       (0.5*FOSC_FREQ)  // FCY instruction clock frequency (16.0 MHz)

//=== CONSTANTS ===
#define SLEEP_CREDIT   30
#define SLEEP_TIME1    100
#define SLEEP_TIME2    140

//=== PROTOTYPES ===
void global_init( void );

#endif //_SYSTEM_H
