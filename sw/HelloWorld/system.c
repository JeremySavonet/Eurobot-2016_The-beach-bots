/*
 * Manage board structure here
 *
 */

#include "system.h"
#include "color.h"

#include "comm/debug_manager.h"
#include "comm/usb_manager.h"

#include "modules/esp8266_manager.h"
#include "modules/motor_manager.h"

#include "lwipthread.h"
#include "modules/web/web.h"

#include "versatile_cs.h"
#include "versatile_sensors.h"

/*===========================================================================*/
/* Declare here board structure                                              */
/*===========================================================================*/
system_t sys;

// Green LED blinker thread, times are in milliseconds.
static THD_WORKING_AREA( wa_alive, 128 );
static THD_FUNCTION( Alive, arg )
{
    (void)arg;
    chRegSetThreadName( "alive" );
    while( true )
    {
        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );
    }
}

void system_print_boot_msg( void )
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
void system_init( void )
{
    // Init motors and QEI first to avoid logic level issues 
    // on motors at startup
    motor_manager_init();
    
    qei_manager_init(); 
    
    // then, init other managers
    lwipInit( NULL );

    // Creates the HTTP thread (it changes priority internally).
    chThdCreateStatic( wa_http_server, 
                       sizeof(wa_http_server), 
                       NORMALPRIO + 1,
                       http_server, 
                       NULL );

    usb_manager_init();

    debug_manager_init();

    esp8266_manager_init();

    // Init IOs
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );

    system_print_boot_msg();

    // Inits all the trajectory stuff, PID, odometry, etc...
#if 1
    versatile_cs_init( &sys.controls.robot );
    DPRINT( 1, "Main control system ready\r\n" );
#endif

    // Inits all the sensors stuff, IR, US, ADXL, etc...
#if 1
    versatile_sensors_init( &sys.sensors );
    DPRINT( 1, "Sensors system ready\r\n" );
#endif

    // Init done => Board ready
    // Creates the blinker thread.
    chThdCreateStatic( wa_alive,
                       sizeof( wa_alive ),
                       NORMALPRIO,
                       Alive,
                       NULL );
    
    DPRINT( 1, "System ready\r\n" );
}
