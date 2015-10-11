#ifndef _PIN_H
#define	_PIN_H

#include "system.h"

// Adxl 362
#define ADXL_INT1                   _RB12
#define ADXL_INT2                   _RB13
#define ADXL_CSn                    _LATB14
#define ADXL_3V3_EN                 _LATE5

// Leds
#define LED1                        _LATE4
#define LED3                        _LATE6
#define LED4                        _LATE7

// Version board
#define VER_B0                      _RE0        //LSB
#define VER_B1                      _RE1
#define VER_B2                      _RE2
#define VER_B3                      _RE3        //MSB
#define PCB_VER                     (PORTE & 0xf)

#define BAT                         11

#endif //_PIN_H