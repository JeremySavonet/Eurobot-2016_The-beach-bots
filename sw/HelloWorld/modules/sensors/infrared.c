/*
 * Simple module to manage IR sensors
 * Author: Jeremy S.
 * Rev : 1.0 
 */

#include <string.h>
#include <math.h>

#include "infrared.h"

// Private functions
void ir_read( void ); 

///////////////////////////////////////////////////////////////////////////////
// Data structures and configuation.
///////////////////////////////////////////////////////////////////////////////

#define MAX_NUMBER_OF_INFRAREDS         10
#define INFRARED_ADC_GRP_NUM_CHANNELS   3
#define INFRARED_ADC_GRP_BUF_DEPTH      128

// Buffer for the data samples.
static adcsample_t infrared_samples[ INFRARED_ADC_GRP_NUM_CHANNELS * 
                                     INFRARED_ADC_GRP_BUF_DEPTH ];
static float dIR0 = 0;
static float dIR1 = 0;
static float dIR2 = 0;

// Configuration for the analog inputs.
static const ADCConversionGroup infrared_sensors_adc_cfg = 
{
    FALSE, // No circular buffer mode.
    INFRARED_ADC_GRP_NUM_CHANNELS, // Number of the analog channels.
    NULL, // No callback function.
    NULL, // No error callback function.
    0, // CR1.
    ADC_CR2_SWSTART, // CR2.
    0, 
    ADC_SMPR2_SMP_AN4( ADC_SAMPLE_3 ) | ADC_SMPR2_SMP_AN5( ADC_SAMPLE_3 ) | ADC_SMPR2_SMP_AN6( ADC_SAMPLE_3 ), // Sample times for channels 0 - 9.
    ADC_SQR1_NUM_CH( INFRARED_ADC_GRP_NUM_CHANNELS ), // ADC SQR1 register initialization data for conversion group sequence 13 - 16 + sequence length.
    0, // ADC SQR2 register initialization data for conversion group sequence 7 - 12.
    ADC_SQR3_SQ1_N( ADC_CHANNEL_IN4 ) | ADC_SQR3_SQ2_N( ADC_CHANNEL_IN5 ) | ADC_SQR3_SQ3_N( ADC_CHANNEL_IN6 ) // ADC SQR3 register initialization data for conversion group sequence 1 - 6.
};

void infrared_get_data( infrared_data_t * data ) 
{
    ir_read();

    if( NULL != data ) 
    {
        int8_t i = 0;
        struct infrared_data * curr = data;

        for( i = 0; i < MAX_NUMBER_OF_INFRAREDS; i++ ) 
        {
            switch( i ) 
            {
                case 0:
                    curr->ir_d0 = dIR0;
                    break;
                case 1:
                    curr->ir_d1 = dIR1;
                    break;
                case 2:
                    curr->ir_d2 = dIR2;
                    break;
            }
        }
    }
}

void ir_read( void ) 
{
    adcConvert( &ADCD3, 
                &infrared_sensors_adc_cfg, 
                infrared_samples, 
                INFRARED_ADC_GRP_BUF_DEPTH );

    int i = 0;
    int d0 = 0;
    int d1 = 0;
    int d2 = 0;

    for( ; i < INFRARED_ADC_GRP_NUM_CHANNELS * INFRARED_ADC_GRP_BUF_DEPTH; i += 3 ) 
    {
        d0 += (int) infrared_samples[i];
        d1 += (int) infrared_samples[i+1];
        d2 += (int) infrared_samples[i+2];
    }

    // Average the last INFRARED_ADC_GRP_BUF_DEPTH sampled values.
    d0 /= INFRARED_ADC_GRP_BUF_DEPTH;
    dIR0 = 24574 * powf( d0, -0.9057 ); 

    d1 /= INFRARED_ADC_GRP_BUF_DEPTH;
    dIR1 = 24574 * powf( d1, -0.9057 ); 

    d2 /= INFRARED_ADC_GRP_BUF_DEPTH;
    dIR2 = 24574 * powf( d2, -0.9057 ); 
}

void infrared_init( infrared_data_t * ir_data ) 
{
    // mem set de la struct ???
    memset( ir_data, 0, sizeof( infrared_data_t ) );

    // Initializes the ADC driver 3 
    adcStart( &ADCD3, NULL );

    // Infrared distance sensor 0 is connected to pin PF6 (IN4).   
    palSetGroupMode( GPIOF, PAL_PORT_BIT( 6 ), 0, PAL_MODE_INPUT_ANALOG);
    
    // Infrared distance sensor 1 is connected to pin PF7 (IN5).   
    palSetGroupMode( GPIOF, PAL_PORT_BIT( 7 ), 0, PAL_MODE_INPUT_ANALOG);

    // Infrared distance sensor 2 is connected to pin PF8 (IN6).   
    palSetGroupMode( GPIOF, PAL_PORT_BIT( 8 ), 0, PAL_MODE_INPUT_ANALOG);
}
