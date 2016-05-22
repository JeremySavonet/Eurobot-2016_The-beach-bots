/*
 *  QEI driver module
 *  See : DC Motor and Brushless Motor Manager module
 *
 *  Author: Jeremy S.
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#include "qei_manager.h"

/*===========================================================================*/
/* QEI related.                                                              */
/*===========================================================================*/

static const QEIConfig motor_left_qei_cfg =
{
    QEI_MODE_QUADRATURE,
    QEI_BOTH_EDGES,
    QEI_DIRINV_FALSE
};

static const QEIConfig motor_right_qei_cfg =
{
    QEI_MODE_QUADRATURE,
    QEI_BOTH_EDGES,
    QEI_DIRINV_TRUE
};

void qei_manager_init( void )
{
    // Left encoder
    palSetPadMode( GPIOB, 4, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOB, 5, PAL_MODE_ALTERNATE( 2 ) );

    // Right encoder
    palSetPadMode( GPIOD, 12, PAL_MODE_ALTERNATE( 2 ) );
    palSetPadMode( GPIOD, 13, PAL_MODE_ALTERNATE( 2 ) );

    qeiStart( &MOTOR_LEFT_QEI_DRIVER, &motor_left_qei_cfg );
    qeiStart( &MOTOR_RIGHT_QEI_DRIVER, &motor_right_qei_cfg );

    qeiEnable( &MOTOR_LEFT_QEI_DRIVER );
    qeiEnable( &MOTOR_RIGHT_QEI_DRIVER );
}

int32_t versatile_qei_get_encoder( void *device )
{
    uint16_t qei = qeiGetCount( (QEIDriver *) device );
    return qei;
}
