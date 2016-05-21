/*
 * Manage board structure here
 *
 * Author: Jeremy S.
 * Date: 2015-10-03
 * Version: V1.0
 */

#include "system.h"
#include "color.h"

#include "comm/debug_manager.h"
#include "comm/usb_manager.h"

#include "modules/fatfs/fatfs_manager.h"
#include "modules/rf/esp8266/esp8266_manager.h"
#include "modules/rf/mrf24j40/mrf24j40.h"

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
        chThdSleepMilliseconds( is_fs_ready() ? 125 : 500 );
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

static void system_print_cli_msg( void )
{
    //Display boot sys info:
    CLI_PRINT( 1, KNRM "__   __                          _     _   _          ___    ___ \r\n" );
    CLI_PRINT( 1, KNRM "\\ \\ / /  ___   _ _   ___  __ _  | |_  (_) | |  ___   / _ \\  / __|\r\n" );
    CLI_PRINT( 1, KNRM " \\ V /  / -_) | '_| (_-< / _` | |  _| | | | | / -_) | (_) | \\__ \\\r\n" );
    CLI_PRINT( 1, KNRM "  \\_/   \\___| |_|   /__/ \\__,_|  \\__| |_| |_| \\___|  \\___/  |___/\r\n" );

    CLI_PRINT( 1, KGRN "Kernel:       %s\r\n", CH_KERNEL_VERSION );
    #ifdef CH_COMPILER_NAME
        CLI_PRINT( 1, KGRN "Compiler:     %s\r\n", CH_COMPILER_NAME );
    #endif
     CLI_PRINT( 1, KGRN "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME );
    #ifdef CH_CORE_VARIANT_NAME
        CLI_PRINT( 1, KGRN "Core Variant: %s\r\n", CH_CORE_VARIANT_NAME );
    #endif
    #ifdef CH_PORT_INFO
        CLI_PRINT( 1, KGRN "Port Info:    %s\r\n", CH_PORT_INFO );
    #endif
    #ifdef PLATFORM_NAME
        CLI_PRINT( 1, KGRN "Platform:     %s\r\n", PLATFORM_NAME );
    #endif
    #ifdef BOARD_NAME
        CLI_PRINT( 1, KGRN "Board:        %s\r\n", BOARD_NAME );
    #endif
    #ifdef __DATE__
    #ifdef __TIME__
        CLI_PRINT( 1, KGRN "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__ );
    #endif
    #endif

    // Set color cursor to normal
    CLI_PRINT( 1, KNRM "" );
}

// Init all peripherals
void system_init( void )
{
    // Inits IOs
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOD, 1, PAL_MODE_INPUT_PULLUP ); // strat color pin
    palSetPadMode( GPIOE, 7, PAL_MODE_OUTPUT_PUSHPULL ); //reset pin for zigbee

    // Inits debug
    debug_manager_init();

    // Welcome the user
    system_print_boot_msg();

    // Inits all the trajectory stuff, PID, odometry, etc...
#if 1
    versatile_cs_init( &sys.controls.robot );
    DPRINT( 1, "[*] Main control system ready\r\n" );
#endif

    // Inits all the sensors stuff, IR, US, ADXL, etc...
#if 1
    versatile_sensors_init( &sys.sensors );
    DPRINT( 1, "[*] Sensors system ready\r\n" );
#endif

    // Inits USB CLI
    usb_manager_init();
    DPRINT( 1, "[*] usb OTG system ready\r\n" );
    system_print_cli_msg();

    // Inits WiFi IoT
    esp8266_manager_init();
    DPRINT( 1, "[*] ESP8266 system ready\r\n" );

    // Inits WiFi IoT
    init_fatfs();
    DPRINT( 1, "[*] FatFs system ready\r\n" );
    
    // Init done => Board ready
    // Creates the blinker thread.
    chThdCreateStatic( wa_alive,
                       sizeof( wa_alive ),
                       NORMALPRIO,
                       Alive,
                       NULL );

    DPRINT( 1, "System ready\r\n" );
}
