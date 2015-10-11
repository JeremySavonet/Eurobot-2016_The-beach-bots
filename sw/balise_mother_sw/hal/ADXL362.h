#ifndef _ADXL362_H
#define	_ADXL362_H

#include "spi.h"
#include "system.h"

/*
 * Read and Write operations
 * => The SPI port uses a multibyte structure wherein the first byte is a command.
 *
 * READ & WRITE Register commands
 * </CS down> <command byte 0X0A or 0x0B> <address byte> <additional data bytes> </CS up>
 *
 * READ FIFO Register commands
 * </CS down> <command byte 0X0D> <data byte> <data byte>... </CS up>
 */

// === ADXL362 registers map ===

#define WRITE_REGISTER             0x0A
#define READ_REGISTER              0x0B
#define READ_FIFO                  0x0D

#define DEVID_AD                   0x00
#define DEVID_MST                  0x01
#define PARTID                     0x02
#define REVID                      0x03
#define XDATA                      0x08
#define YDATA                      0x09
#define ZDATA                      0x0A
#define STATUS                     0x0B
#define FIFO_ENTRIES_L             0x0C
#define FIFO_ENTRIES_H             0x0D
#define XDATA_L                    0x0E
#define XDATA_H                    0x0F
#define YDATA_L                    0x10
#define YDATA_H                    0x11
#define ZDATA_L                    0x12
#define ZDATA_H                    0x13
#define TEMP_L                     0x14
#define TEMP_H                     0x15
#define SOFT_RESET                 0x1F
#define THRESH_ACT_L               0x20
#define THRESH_ACT_H               0x21
#define TIME_ACT                   0x22
#define THRESH_INACT_L             0x23
#define THRESH_INACT_H             0x24
#define TIME_INACT_L               0x25
#define TIME_INACT_H               0x26
#define ACT_INACT_CTL              0x27
#define FIFO_CONTROL               0x28
#define FIFO_SAMPLES               0x29
#define INTMAP1                    0x2A
#define INTMAP2                    0x2B
#define FILTER_CTL                 0x2C
#define POWER_CTL                  0x2D
#define SELF_TEST                  0x2E

#define ADXL362_INT1_PIN 1
#define ADXL362_INT2_PIN 2

// === GLOBAL VARIABLES ===
extern volatile s8 g_adxl_x;
extern volatile s8 g_adxl_y;
extern volatile s8 g_adxl_z;

// === PROTOTYPES ===
void adxl362_init( void );
void adxl362_power_off( void );
void adxl362_getXYZ8( s8 * x, s8 * y, s8 * z );

u8 adxl362_reversed( void );
s8 adxl362_getXData( void );
s8 adxl362_getYData( void );
s8 adxl362_getZData( void );

#endif //_ADXL362_H