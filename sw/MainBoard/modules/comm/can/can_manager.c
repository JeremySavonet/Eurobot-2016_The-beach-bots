/*
 *  CAN comm module driver.
 *  Allows the user to exchange data over CAN bus.
 *
 *  Author: JS
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#include "ch.h"
#include "hal.h"

#include "can_manager.h"

// Settings
#define CANDx		CAND1

// Threads
static THD_WORKING_AREA( can_thread_wa, 2048 );
static THD_FUNCTION( can_thread, arg );

/*
 * 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 * See section 22.7.7 on the STM32 reference manual.
 */
static const CANConfig cancfg =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
    CAN_BTR_TS1(8) | CAN_BTR_BRP(6)
};

void can_manager_init( void )
{
    palSetPadMode( GPIOD,
                   0,
                   PAL_MODE_ALTERNATE( 9 ) |
                   PAL_STM32_OTYPE_PUSHPULL |
                   PAL_STM32_OSPEED_MID1 );
    
    palSetPadMode( GPIOD,
                   1,
                   PAL_MODE_ALTERNATE( 9 ) |
                   PAL_STM32_OTYPE_PUSHPULL |
                   PAL_STM32_OSPEED_MID1 );
    
    canStart( &CANDx, &cancfg );
    
    chThdCreateStatic( can_thread_wa,
                       sizeof( can_thread_wa ),
                       NORMALPRIO + 1,
                       can_thread,
                       NULL );
}

static THD_FUNCTION( can_thread, arg )
{
    (void)arg;
    chRegSetThreadName( "CAN" );
    
    event_listener_t el;
    CANRxFrame rxmsg;
    uint8_t buffer[9];

    chEvtRegister( &CANDx.rxfull_event, &el, 0 );
    
    while( !chThdShouldTerminateX() )
    {
        if( chEvtWaitAnyTimeout( ALL_EVENTS, MS2ST( 100 ) ) == 0 )
        {
            continue;
        }
        
        while( canReceive( &CANDx, 
                           CAN_ANY_MAILBOX, 
                           &rxmsg, 
                           TIME_IMMEDIATE ) == MSG_OK )
        {
            buffer[ 0 ] = rxmsg.SID;
            for( int i = 0; i < rxmsg.DLC; i++ )
            {
                buffer[ i + 1 ] = rxmsg.data8[ i ];
            }
            
            // TODO: Process packet
        }
    }
    
    chEvtUnregister( &CAND1.rxfull_event, &el );
}

void can_transmit( uint32_t id, uint8_t *data, uint8_t len )
{
    CANTxFrame txmsg;
    
    txmsg.IDE = CAN_IDE_STD;
    txmsg.EID = id;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = len;
    
    for( int i = 0; i < len; i++ )
    {
        txmsg.data8[ i ] = data[ i ];
    }
    
    canTransmit( &CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST( 100 ) );
}
