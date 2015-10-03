/*
* Simple demo program
*
* Description: A simple helloworld program that blink a led
* Author: JS
* Date: 2015-10-03
* Version: V0.1
*/

#include "ch.h"
#include "hal.h"
#include "test.h"

int main( void )
{

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  while( true )
  {
    chThdSleepMilliseconds( 500 );
  }

  return 0;
}
