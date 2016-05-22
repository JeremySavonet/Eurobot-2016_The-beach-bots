/*
 *  FatFS module driver.
 *  Allows the user to save/load data from the sd card slot.
 *
 *  Author: JS
 *  Date: 2016-05-19
 *  Version: V1.0
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "../../color.h"
#include "../../comm/debug_manager.h"

#include "fatfs_manager.h"

 // Card monitor timer.
static virtual_timer_t tmr;

// Debounce counter.
static unsigned cnt;

// Card event sources.
static event_source_t inserted_event, removed_event;

/*
 * Insertion monitor timer callback function.
 *
 * pointer to the p BaseBlockDevice object
 *
 */
static void tmrfunc( void *p ) 
{
    BaseBlockDevice *bbdp = p;
    
    chSysLockFromISR();
    if( cnt > 0 ) 
    {
        if( blkIsInserted( bbdp ) ) 
        {
            if( --cnt == 0 ) 
            {
                chEvtBroadcastI( &inserted_event );
            }
        }
        else
        {
            cnt = POLLING_INTERVAL;
        }
    }
    else if( ! blkIsInserted( bbdp ) ) 
    {
        cnt = POLLING_INTERVAL;
        chEvtBroadcastI( &removed_event );
    }
    
    chVTSetI( &tmr, MS2ST( POLLING_DELAY ), tmrfunc, bbdp );
    chSysUnlockFromISR();
}

/*
 * Polling monitor start.
 *
 * pointer to an object implementing p BaseBlockDevice
 *
 */
static void tmr_init( void *p ) 
{
    chEvtObjectInit( &inserted_event );
    chEvtObjectInit( &removed_event );
    chSysLock();
    cnt = POLLING_INTERVAL;
    chVTSetI( &tmr, MS2ST( POLLING_DELAY ), tmrfunc, p );
    chSysUnlock();
}

// FS object.
static FATFS SDC_FS;

// FS mounted and ready.
static bool fs_ready = FALSE;

// Generic large buffer.
static uint8_t fbuff[ 1024 ];

FRESULT scan_files( char *path ) 
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
    
#if _USE_LFN
    fno.lfname = 0;
    fno.lfsize = 0;
#endif
  
    res = f_opendir( &dir, path );
    if( res == FR_OK ) 
    {
        i = strlen( path );
        while( true )
        {
            res = f_readdir( &dir, &fno );
            if( res != FR_OK || fno.fname[ 0 ] == 0 )
                break;
            
            if( fno.fname[ 0 ] == '.' )
                continue;
            
            fn = fno.fname;
        
            if( fno.fattrib & AM_DIR ) 
            {
                path[ i++ ] = '/';
                strcpy( &path[ i ], fn );
                res = scan_files( path );
          
                if( res != FR_OK )
                    break;
                
                path[ --i ] = 0;
            }
            else 
            {
                CLI_PRINT( 1, "%s/%s\r\n", path, fn );
            }
        }
    }
    
    return res;
}

// Card insertion event.
static void InsertHandler( eventid_t id ) 
{
    FRESULT err;
    
    (void)id;
    // On insertion SDC initialization and FS mount.
    if( sdcConnect( &SDCD1 ) )
      return;
    
    err = f_mount( &SDC_FS, "/", 1 );
    if( FR_OK != err ) 
    {
        sdcDisconnect( &SDCD1 );
        return;
    }
    
    DPRINT( 2, KYEL "[ FatFS ] Card inserted\r\n" );
    fs_ready = TRUE;
}

// Card removal event.
static void RemoveHandler( eventid_t id ) 
{
    (void)id;
    sdcDisconnect( &SDCD1 );
    
    DPRINT( 2, KYEL "[ FatFS ] Card removed\r\n" );
    fs_ready = FALSE;
}

static const evhandler_t evhndl[] = 
{
    InsertHandler,
    RemoveHandler
};

event_listener_t el0, el1;
  
void init_fatfs( void )
{
    // Activates the SDC driver 1 using default configuration.
    sdcStart( &SDCD1, NULL );
    
    // Activates the card insertion monitor.
    tmr_init( &SDCD1 );
    
    chEvtRegister( &inserted_event, &el0, 0 );
    chEvtRegister( &removed_event, &el1, 1 );
}

void fatfs_dispatch_event( void )
{
    chEvtDispatch( evhndl, chEvtWaitOneTimeout( ALL_EVENTS, MS2ST( 500 ) ) );
}

bool is_fs_ready( void )
{
    return fs_ready;
}
