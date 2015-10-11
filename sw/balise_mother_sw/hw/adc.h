#ifndef _ADC_H
#define	_ADC_H

#include "global_variables.h"
#include "system.h"
#include "types.h"

#define     ADC_ON()                AD1CON1bits.ADON = 1
#define     ADC_OFF()               AD1CON1bits.ADON = 0
#define     ADC_start_sampling()    AD1CON1bits.SAMP = 1
#define     ADC_start_conversion()  AD1CON1bits.SAMP = 0

// === A/D Conversion Clock ===
// Tad = Tcy(ADCS+1)
#define     ADCS_400khz             (FCY_FREQ/400000)-1 // = 40
#define     ADCS_500khz             (FCY_FREQ/500000)-1
#define     ADCS_600khz             (FCY_FREQ/600000)-1
#define     ADCS_700khz             (FCY_FREQ/700000)-1

// === A/D Negative Voltage Reference ===
#define     ADC_PVR_Vref()          AD1CON2bits.PVCFG = 1
#define     ADC_PVR_AVdd()          AD1CON2bits.PVCFG = 0

// === A/D Negative Voltage Reference ===
#define     ADC_NVR_Vref()          AD1CON2bits.NVCFG0 = 1
#define     ADC_NVR_AVss()          AD1CON2bits.NVCFG0 = 0

// === A/D Channels ===
#define     VBG_channel             28
#define     AVSS_channel            29
#define     AVDD_channel            30
#define     VBAT_channel            31

// === PROTOTYPES ===
void init_adc( void );
s16 read_adc( u8 channel );

#endif //_ADC_H