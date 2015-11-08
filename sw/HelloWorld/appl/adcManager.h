/*
 * Adc module manager
 * Author: Jeremy S.
 */

#ifndef _ADCMANAGER_H_
#define _ADCMANAGER_H_

void cmd_measure( int argc, char *argv[] );
void cmd_measureA( int argc, char *argv[] );
void cmd_measureDirect( int argc, char *argv[] );
void cmd_Vref( int argc, char *argv[] );
void cmd_Temperature( int argc, char *argv[] );

void cmd_measureCont( int argc, char *argv[] );
void cmd_measureRead( int argc, char *argv[] );
void cmd_measureStop( int argc, char *argv[] );

void adcManagerInit( void );

#endif // _ADCMANAGER_H_
