/*
 * Simple module to manage temperature sensors
 * Author: Jeremy S.
 * Rev : 1.0
 */

#include <string.h>
#include <math.h>

#include "temperature.h"

// Private functions
void temperature_read( void );

///////////////////////////////////////////////////////////////////////////////
// Data structures and configuation.
///////////////////////////////////////////////////////////////////////////////

#define TEMPERATURE_ADC_GRP_NUM_CHANNELS    1
#define TEMPERATURE_ADC_GRP_BUF_DEPTH       8
#define V25                                 0.760
#define AVG_SLOPE                           25.0

// Buffer for the data samples.
static adcsample_t temperature_samples[ TEMPERATURE_ADC_GRP_NUM_CHANNELS *
                                        TEMPERATURE_ADC_GRP_BUF_DEPTH ];
static float temp = 0;

static const ADCConversionGroup temperature_sensors_adc_cfg =
{
    FALSE, // No circular buffer mode.
    TEMPERATURE_ADC_GRP_NUM_CHANNELS, // Number of the analog channels.
    NULL, // No callback function.
    NULL, // No error callback function.
    0, // CR1.
    ADC_CR2_SWSTART, // CR2.
    ADC_SMPR1_SMP_SENSOR( ADC_SAMPLE_144 ), // Sample times for channels 10 - 18, here sample time for channel SENSOR.
    0, // Sample times for channels 0 - 9.
    ADC_SQR1_NUM_CH( TEMPERATURE_ADC_GRP_NUM_CHANNELS ), // ADC SQR1 register initialization data for conversion group sequence 13 - 16 + sequence length.
    0, // ADC SQR2 register initialization data for conversion group sequence 7 - 12.
    ADC_SQR3_SQ1_N( ADC_CHANNEL_SENSOR ) // ADC SQR3 register initialization data for conversion group sequence 1 - 6.
};

void temperature_get_data( temperature_data_t *data )
{
    // Read temp adc
    temperature_read();

    // Update structure
    if( NULL != data )
    {
        data->temp0 = temp;
    }
}

void temperature_read( void )
{
    adcConvert( &ADCD1,
                &temperature_sensors_adc_cfg,
                temperature_samples,
                TEMPERATURE_ADC_GRP_BUF_DEPTH );

    int i = 0;
    int temperature = 0;
    for( ; i < TEMPERATURE_ADC_GRP_NUM_CHANNELS * TEMPERATURE_ADC_GRP_BUF_DEPTH; i++ )
    {
        temperature += (int) temperature_samples[i];
    }

    // Average the last TEMPERATURE_ADC_GRP_BUF_DEPTH sampled values.
    temperature /= TEMPERATURE_ADC_GRP_BUF_DEPTH;

    // See http://www.st.com/web/en/resource/technical/document/reference_manual/DM00031020.pdf, page 289.
    temperature =  ( temperature * 3300 / 65535 );
    temp = ( temperature / 1000.0 );
    temp = ( ( temp - V25 ) / AVG_SLOPE ) + 25.0 ;
}

void temperature_init( temperature_data_t * temp_data )
{
    // memset of temp structure
    memset( temp_data, 0, sizeof( temperature_data_t ) );

    // Initializes the ADC driver 1 in order to access the analog values from the onboard temperature sensor.
    adcStart( &ADCD1, NULL );

    // Temperature sensor initialization.
    adcSTM32EnableTSVREFE();
}
