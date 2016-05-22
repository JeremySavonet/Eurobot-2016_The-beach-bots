/*
 *  QEI driver module
 *  See : DC Motor and Brushless Motor Manager module
 *
 *  Author: Jeremy S.
 *  Date: 2016-05-22
 *  Version: V1.0
 */

#ifndef _QEI_MANAGER_H_
#define _QEI_MANAGER_H_

// Platform includes
#include "ch.h"
#include "hal.h"

#define MOTOR_ENCODER_BASE_LEFT  ( (QEIDriver *) &QEID3 )
#define MOTOR_ENCODER_BASE_RIGHT  ( (QEIDriver *) &QEID4 )

#define MOTOR_LEFT_QEI_DRIVER QEID3
#define MOTOR_RIGHT_QEI_DRIVER QEID4

void qei_manager_init( void );
int32_t versatile_qei_get_encoder( void *device );

#endif // _QEI_MANAGER_H_ 
