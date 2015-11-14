/*
 * Manage board structure here
 *
 */

#include "system.h"
#include "color.h"

#include "comm/debugManager.h"
#include "comm/microrl.h"
#include "comm/microshell.h"
#include "comm/usbManager.h"

#include "modules/adcManager.h"
#include "modules/esp8266Manager.h"
#include "modules/motorsManager.h"

/*===========================================================================*/
/* Declare here board structurev                                             */
/*===========================================================================*/
SYSTEM sys;

void systemPrintBootMsg( void )
{
    //Display boot sys info:
    DPRINT( 1, KNRM "__   __                          _     _   _          ___    ___ \r\n" );
    DPRINT( 1, KNRM "\\ \\ / /  ___   _ _   ___  __ _  | |_  (_) | |  ___   / _ \\  / __|\r\n" );
    DPRINT( 1, KNRM " \\ V /  / -_) | '_| (_-< / _` | |  _| | | | | / -_) | (_) | \\__ \\\r\n" );
    DPRINT( 1, KNRM "  \\_/   \\___| |_|   /__/ \\__,_|  \\__| |_| |_| \\___|  \\___/  |___/\r\n" );

    DPRINT( 1, KGRN "Kernel:       %s\r\n", CH_KERNEL_VERSION );
    #ifdef CH_COMPILER_NAME
        DPRINT( 1, KGRN "Compiler:     %s\r\n", CH_COMPILER_NAME );
    #endif
     DPRINT( 1, KGRN "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME );
    #ifdef CH_CORE_VARIANT_NAME
        DPRINT( 1, KGRN "Core Variant: %s\r\n", CH_CORE_VARIANT_NAME );
    #endif
    #ifdef CH_PORT_INFO
        DPRINT( 1, KGRN "Port Info:    %s\r\n", CH_PORT_INFO );
    #endif
    #ifdef PLATFORM_NAME
        DPRINT( 1, KGRN "Platform:     %s\r\n", PLATFORM_NAME );
    #endif
    #ifdef BOARD_NAME
        DPRINT( 1, KGRN "Board:        %s\r\n", BOARD_NAME );
    #endif
    #ifdef __DATE__
    #ifdef __TIME__
        DPRINT( 1, KGRN "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__ );
    #endif
    #endif
    
    // Set color cursor to normal
    DPRINT( 1, KNRM "" );
}

// Init all peripherals
void initSystem( void )
{
    // First, init all managers
    usbManagerInit();
    
    debugManagerInit();
    
    esp8266ManagerInit();
    
    motorsManagerInit();
    
    adcManagerInit();

    // Init IOs
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );

    // Init done => Board ready
    palClearPad( GPIOC, GPIOC_LED );
   
    systemPrintBootMsg();
    DPRINT( 1, "System ready\r\n" );
}
