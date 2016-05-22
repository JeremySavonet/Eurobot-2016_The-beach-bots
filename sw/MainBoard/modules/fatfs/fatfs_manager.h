/*
 *  FatFS module driver.
 *  Allows the user to save/load data from the sd card slot.
 *
 *  Author: JS
 *  Date: 2016-05-19
 *  Version: V1.0
 */

#ifndef _FATFS_MANAGER_H_
#define _FATFS_MANAGER_H_

// FatFS include
#include "ff.h"

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

FRESULT scan_files( char *path );

void init_fatfs( void );
void fatfs_dispatch_event( void );

bool is_fs_ready( void );

#endif // _FATFS_MANAGER_H_
