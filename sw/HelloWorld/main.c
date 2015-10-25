/*
 * Simple demo program
 *
 * Description: 
 * A simple HelloWorld program that create a task that blinks a led
 *
 * Added support for:
 * 90s timer to stop all the thread
 * Uart 
 * UEXT reader thread
 * PWM on port D pin 12/13/14/15
 * ESP8266 API
 * CLI on USB2 OTG
 *
 * Author: JS
 * Date: 2015-10-03
 * Version: V0.1
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "chprintf.h"
#include "hal.h"
#include "shell.h"
#include "test.h"

#include "appl/console.h"
#include "appl/ESP8266.h"
#include "appl/motors.h"

/*===========================================================================*/
/* Defines                                                                   */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE( 2048 )
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE( 256 )

/*===========================================================================*/
/* Global static functions                                                   */
/*===========================================================================*/

static void debugInit( void );
static void runGame( void *p ); 

/*===========================================================================*/
/* Global static variables                                                   */
/*===========================================================================*/

static bool running = false;
static int gameTick = 0;
static thread_t *tp = NULL;
static virtual_timer_t gameTimer;

// Serial over USB Driver structure.
SerialUSBDriver SDU2;

// Endpoints to be used for USBD2.
#define USBD2_DATA_REQUEST_EP           1
#define USBD2_DATA_AVAILABLE_EP         1
#define USBD2_INTERRUPT_REQUEST_EP      2

/*===========================================================================*/
/* USB related stuff. */
/*===========================================================================*/

// USB Device Descriptor.
static const uint8_t vcom_device_descriptor_data[18] = 
{
    USB_DESC_DEVICE       (0x0110,        /* bcdUSB (1.1).                    */
                           0x02,          /* bDeviceClass (CDC).              */
                           0x00,          /* bDeviceSubClass.                 */
                           0x00,          /* bDeviceProtocol.                 */
                           0x40,          /* bMaxPacketSize.                  */
                           0x0483,        /* idVendor (ST).                   */
                           0x5740,        /* idProduct.                       */
                           0x0200,        /* bcdDevice.                       */
                           1,             /* iManufacturer.                   */
                           2,             /* iProduct.                        */
                           3,             /* iSerialNumber.                   */
                           1)             /* bNumConfigurations.              */
};

// Device Descriptor wrapper.
static const USBDescriptor vcom_device_descriptor = 
{
    sizeof vcom_device_descriptor_data,
    vcom_device_descriptor_data
};

// Configuration Descriptor tree for a CDC
static const uint8_t vcom_configuration_descriptor_data[67] = 
{
    /* Configuration Descriptor.*/
    USB_DESC_CONFIGURATION(67,            /* wTotalLength.                    */
                           0x02,          /* bNumInterfaces.                  */
                           0x01,          /* bConfigurationValue.             */
                           0,             /* iConfiguration.                  */
                           0xC0,          /* bmAttributes (self powered).     */
                           50),           /* bMaxPower (100mA).               */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE    (0x00,          /* bInterfaceNumber.                */
                           0x00,          /* bAlternateSetting.               */
                           0x01,          /* bNumEndpoints.                   */
                           0x02,          /* bInterfaceClass (Communications
                                             Interface Class, CDC section
                                             4.2).                            */
                           0x02,          /* bInterfaceSubClass (Abstract
                                           Control Model, CDC section 4.3).   */
                           0x01,          /* bInterfaceProtocol (AT commands,
                                             CDC section 4.4).                */
                           0),            /* iInterface.                      */
    /* Header Functional Descriptor (CDC section 5.2.3).*/
    USB_DESC_BYTE         (5),            /* bLength.                         */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x00),         /* bDescriptorSubtype (Header
                                             Functional Descriptor.           */
    USB_DESC_BCD          (0x0110),       /* bcdCDC.                          */
    /* Call Management Functional Descriptor. */
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x01),         /* bDescriptorSubtype (Call Management
                                             Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bmCapabilities (D0+D1).          */
    USB_DESC_BYTE         (0x01),         /* bDataInterface.                  */
    /* ACM Functional Descriptor.*/
    USB_DESC_BYTE         (4),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x02),         /* bDescriptorSubtype (Abstract
                                             Control Management Descriptor).  */
    USB_DESC_BYTE         (0x02),         /* bmCapabilities.                  */
    /* Union Functional Descriptor.*/
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x06),         /* bDescriptorSubtype (Union
                                             Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bMasterInterface (Communication
                                             Class Interface).                */
    USB_DESC_BYTE         (0x01),         /* bSlaveInterface0 (Data Class
                                             Interface).                      */
    /* Endpoint 2 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD2_INTERRUPT_REQUEST_EP|0x80,
                           0x03,          /* bmAttributes (Interrupt).        */
                           0x0008,        /* wMaxPacketSize.                  */
                           0xFF),         /* bInterval.                       */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE    (0x01,          /* bInterfaceNumber.                */
                           0x00,          /* bAlternateSetting.               */
                           0x02,          /* bNumEndpoints.                   */
                           0x0A,          /* bInterfaceClass (Data Class
                                             Interface, CDC section 4.5).     */
                           0x00,          /* bInterfaceSubClass (CDC section
                                             4.6).                            */
                           0x00,          /* bInterfaceProtocol (CDC section
                                             4.7).                            */
                           0x00),         /* iInterface.                      */
    /* Endpoint 3 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD2_DATA_AVAILABLE_EP,       /* bEndpointAddress.*/
                           0x02,          /* bmAttributes (Bulk).             */
                           0x0040,        /* wMaxPacketSize.                  */
                           0x00),         /* bInterval.                       */
    /* Endpoint 1 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD2_DATA_REQUEST_EP|0x80,    /* bEndpointAddress.*/
                           0x02,          /* bmAttributes (Bulk).             */
                           0x0040,        /* wMaxPacketSize.                  */
                           0x00)          /* bInterval.                       */
};

// Configuration Descriptor wrapper.
static const USBDescriptor vcom_configuration_descriptor = 
{
    sizeof vcom_configuration_descriptor_data,
    vcom_configuration_descriptor_data
};

// U.S. English language identifier.
static const uint8_t vcom_string0[] = 
{
    USB_DESC_BYTE(4),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

// Vendor string.
static const uint8_t vcom_string1[] = 
{
    USB_DESC_BYTE(38),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
};

// Device Description string.
static const uint8_t vcom_string2[] = 
{
    USB_DESC_BYTE(56),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'C', 0, 'h', 0, 'i', 0, 'b', 0, 'i', 0, 'O', 0, 'S', 0, '/', 0,
    'R', 0, 'T', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0,
    'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0, ' ', 0, 'P', 0,
    'o', 0, 'r', 0, 't', 0
};

// Serial Number string.
static const uint8_t vcom_string3[] = 
{
    USB_DESC_BYTE(8),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    '0' + CH_KERNEL_MAJOR, 0,
    '0' + CH_KERNEL_MINOR, 0,
    '0' + CH_KERNEL_PATCH, 0
};

// Strings wrappers array.
static const USBDescriptor vcom_strings[] = 
{
    {sizeof vcom_string0, vcom_string0},
    {sizeof vcom_string1, vcom_string1},
    {sizeof vcom_string2, vcom_string2},
    {sizeof vcom_string3, vcom_string3}
};

/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor( USBDriver *usbp,
                                            uint8_t dtype,
                                            uint8_t dindex,
                                            uint16_t lang ) 
{
    (void)usbp;
    (void)lang;
    
    switch( dtype ) 
    {
    case USB_DESCRIPTOR_DEVICE:
        return &vcom_device_descriptor;
    case USB_DESCRIPTOR_CONFIGURATION:
        return &vcom_configuration_descriptor;
    case USB_DESCRIPTOR_STRING:
        if( dindex < 4 )
        {
          return &vcom_strings[dindex];
        }
    }

    return NULL;
}

// IN EP1 state.
static USBInEndpointState ep1instate;

// OUT EP1 state.
static USBOutEndpointState ep1outstate;

// EP1 initialization structure (both IN and OUT).
static const USBEndpointConfig ep1config = 
{
    USB_EP_MODE_TYPE_BULK,
    NULL,
    sduDataTransmitted,
    sduDataReceived,
    0x0040,
    0x0040,
    &ep1instate,
    &ep1outstate,
    2,
    NULL
};

// IN EP2 state.
static USBInEndpointState ep2instate;

// EP2 initialization structure (IN only).
static const USBEndpointConfig ep2config = 
{
    USB_EP_MODE_TYPE_INTR,
    NULL,
    sduInterruptTransmitted,
    NULL,
    0x0010,
    0x0000,
    &ep2instate,
    NULL,
    1,
    NULL
};

// Handles the USB driver global events.
static void usb_event( USBDriver *usbp, usbevent_t event ) 
{
    switch( event )
    {
    case USB_EVENT_RESET:
        return;
    case USB_EVENT_ADDRESS:
        return;
    case USB_EVENT_CONFIGURED:
        chSysLockFromISR();

        /* Enables the endpoints specified into the configuration.
         * Note, this callback is invoked from an ISR so I-Class functions
         * must be used.
         */
        usbInitEndpointI( usbp, USBD2_DATA_REQUEST_EP, &ep1config );
        usbInitEndpointI( usbp, USBD2_INTERRUPT_REQUEST_EP, &ep2config );

        /* Resetting the state of the CDC subsystem.*/
        sduConfigureHookI( &SDU2 );

        chSysUnlockFromISR();
        return;
    case USB_EVENT_SUSPEND:
        return;
    case USB_EVENT_WAKEUP:
        return;
    case USB_EVENT_STALLED:
        return;
    }

    return;
}

// USB driver configuration.
static const USBConfig usbcfg = 
{
    usb_event,
    get_descriptor,
    sduRequestsHook,
    NULL
};

// Serial over USB driver configuration.
static const SerialUSBConfig serusbcfg = 
{
    &USBD2,
    USBD2_DATA_REQUEST_EP,
    USBD2_DATA_AVAILABLE_EP,
    USBD2_INTERRUPT_REQUEST_EP
};

/*===========================================================================*/
/* CLI functions here                                                        */
/* TODO : Nice to have little macro to add new cli function                  */
/*===========================================================================*/

static void cmd_mem( BaseSequentialStream *chp, int argc, char *argv[] ) 
{
    size_t n, size;

    (void)argv;
    if (argc > 0) {
      chprintf(chp, "Usage: mem\r\n");
      return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads( BaseSequentialStream *chp, int argc, char *argv[] ) 
{
    static const char *states[] = {CH_STATE_NAMES};
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
      chprintf(chp, "Usage: threads\r\n");
      return;
    }
    chprintf(chp, "    addr    stack prio refs     state time\r\n");
    tp = chRegFirstThread();
    do {
      chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n",
              (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
              (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
              states[tp->p_state]);
      tp = chRegNextThread(tp);
    } while (tp != NULL);
}

static void cmd_test( BaseSequentialStream *chp, int argc, char *argv[] ) 
{
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
      chprintf(chp, "Usage: test\r\n");
      return;
    }
    tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
                             TestThread, chp);
    if (tp == NULL) {
      chprintf(chp, "out of memory\r\n");
      return;
    }
    chThdWait(tp);
}

/*===========================================================================*/
/* Global structures                                                         */
/*===========================================================================*/

// Struct to config serial module for debug 
static SerialConfig uartCfg =
{
    115200,              
    0,
    0,
    0
};

static const ShellCommand commands[] = 
{
    { "mem",     cmd_mem },
    { "threads", cmd_threads },
    { "test",    cmd_test },
    { NULL, NULL }
};

static const ShellConfig shellCfg1 = 
{
    (BaseSequentialStream *)&SDU2,
    commands
};

/*===========================================================================*/
/* Application threads                                                       */
/*===========================================================================*/

// Green LED blinker thread, times are in milliseconds.
static THD_WORKING_AREA( waThread1, 128 );
static THD_FUNCTION( Thread1, arg )
{
    (void)arg;
    chRegSetThreadName( "blinker" );
    while( true )
    {
        DPRINT( 3, "Running..." );
 
        palTogglePad( GPIOC, GPIOC_LED );
        chThdSleepMilliseconds( 500 );

        // Wait 90s and stop all the thread properly
        if( gameTick >= 5 )
        {
            msg_t msg = NULL;
            chThdExit( msg );
        }
    }
}

// Killer thread : wait for all sigterm signals 
static THD_WORKING_AREA( waThread2, 128 );
static THD_FUNCTION( Thread2, arg )
{
    (void)arg;
    chRegSetThreadName( "killer" );
   
    // Wait here all thread to terminate properly 
    chThdWait( tp );

    MotorSetSpeed( 0, 0 );
    MotorSetSpeed( 1, 0 );
    MotorSetSpeed( 2, 0 );
    MotorSetSpeed( 3, 0 );
    DPRINT( 1, "Stop..." );
    palSetPad( GPIOC, GPIOC_LED );  
}

int main( void )
{
    static thread_t *shelltp = NULL;
 
    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();

    // Init IOs
    palSetPadMode( GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL );
        
    // Init CLI
    // Initializes a serial-over-USB CDC driver.
    sduObjectInit( &SDU2 );
    sduStart( &SDU2, &serusbcfg );

    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus( serusbcfg.usbp );
    chThdSleepMilliseconds( 1500 );
    usbStart( serusbcfg.usbp, &usbcfg );
    usbConnectBus( serusbcfg.usbp ); 
 
    shellInit(); 
    
    // Init debug UART
    debugInit();

    // Init ESP8266 WiFi module
    ESP8266Init();
    
    // Init Motors module
    MotorsInit();

    // Init done => Board ready
    palClearPad( GPIOC, GPIOC_LED ); 
    DPRINT( 1, "Ready..." );

    chThdSleepMilliseconds( 600 ); /* Iddle thread */
    
    ESP8266RequestVersion();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */
    
    ESP8266SetMode();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */
    
    ESP8266EnableMultipleConnection();  
    chThdSleepMilliseconds( 100 ); /* Iddle thread */
    
    ESP8266ConfigureServer();  
    chThdSleepMilliseconds( 100 ); /* Iddle thread */
    
    ESP8266SetAccessPoint();
    chThdSleepMilliseconds( 1000 ); /* Iddle thread */
    
    ESP8266JoinAccessPoint();
    chThdSleepMilliseconds( 100 ); /* Iddle thread */

    // Global main loop
    while( true )
    {
        if( !shelltp && ( SDU2.config->usbp->state == USB_ACTIVE ) )
        {
            shelltp = shellCreate( &shellCfg1, SHELL_WA_SIZE, NORMALPRIO );
        }
        else if( chThdTerminatedX( shelltp ) ) 
        {
            chThdRelease( shelltp );  /* Recovers memory of the previous shell.   */
            shelltp = NULL;           /* Triggers spawning of a new shell.        */
        }

        // Wait start button
        if ( palReadPad( GPIOA, GPIOA_BUTTON_WKUP ) != 0 && running == false ) 
        {
            // Start game timer
            chVTSet( &gameTimer, MS2ST( 1000 ), runGame, NULL );
            
            // Creates the blinker thread.
            tp = chThdCreateStatic( waThread1,
                                    sizeof( waThread1 ),
                                    NORMALPRIO,
                                    Thread1,
                                    NULL );

            // Set motors speed to 50% duty cycle
            pwmcnt_t speedM4 = 5000;  
            MotorSetSpeed( 0, speedM4 );
            MotorSetSpeed( 1, speedM4 );
            MotorSetSpeed( 2, speedM4 );
            MotorSetSpeed( 3, speedM4 );
            
            // Start killer thread
            chThdCreateStatic( waThread2,
                    sizeof( waThread2 ),
                    NORMALPRIO,
                    Thread2,
                    NULL ); 

            running = true;
        }
        
        chThdSleepMilliseconds( 100 ); /* Iddle thread */
    }
}

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

void debugInit( void )
{
    // Configure UART3 for debug 115200 8N1 
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) ); 
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) ); 
    sdStart( &SD3, &uartCfg ); 
}

// Game running loop 
void runGame( void *p ) 
{
    // Restarts the timer
    chSysLockFromISR();
    chVTSetI( &gameTimer, MS2ST( 1000 ), runGame, p );
    chSysUnlockFromISR();

    if( ++gameTick < 90 ) {}
}
