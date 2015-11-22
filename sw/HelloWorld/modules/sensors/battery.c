/*
 * Simple module to manage battery level
 * Author: Jeremy S.
 * Rev : 1.0 
 */

#include <string.h>
#include <math.h>

#include "battery.h"

// Private functions
void battery_read( void );

///////////////////////////////////////////////////////////////////////////////
// Data structures and configuation.
///////////////////////////////////////////////////////////////////////////////

#define BATTERY_ADC_GRP_NUM_CHANNELS    3
#define BATTERY_ADC_GRP_BUF_DEPTH       8

// Buffer for the data samples.
static adcsample_t battery_samples[ BATTERY_ADC_GRP_NUM_CHANNELS * 
                                    BATTERY_ADC_GRP_BUF_DEPTH ];
static float batt_level = 0;

static const ADCConversionGroup battery_sensor_adc_cfg = 
{
    FALSE, // No circular buffer mode.
    BATTERY_ADC_GRP_NUM_CHANNELS, // Number of the analog channels.
    NULL, // No callback function.
    NULL, // No error callback function.
    0, // CR1.
    ADC_CR2_SWSTART, // CR2.
    0,
    ADC_SMPR2_SMP_AN7( ADC_SAMPLE_3 ), // Sample times for channels 0 - 9.
    ADC_SQR1_NUM_CH( BATTERY_ADC_GRP_NUM_CHANNELS ), // ADC SQR1 register initialization data for conversion group sequence 13 - 16 + sequence length.
    0, // ADC SQR2 register initialization data for conversion group sequence 7 - 12.
    ADC_SQR3_SQ1_N( ADC_CHANNEL_IN7 ) // ADC SQR3 register initialization data for conversion group sequence 1 - 6.
};

void battery_get_data( battery_data_t *data )
{
    // Read temp adc
    battery_read();

    // Update structure
    if( NULL != data ) 
    {
        data->batt_level = batt_level;
    }
}

void battery_read( void )
{
    adcConvert( &ADCD3, 
                &battery_sensor_adc_cfg, 
                battery_samples, 
                BATTERY_ADC_GRP_BUF_DEPTH );

    int i = 0;
    int level = 0;
    for( ; i < BATTERY_ADC_GRP_NUM_CHANNELS * BATTERY_ADC_GRP_BUF_DEPTH; i++ ) 
    {
        level += (int) battery_samples[i];
    }

    // Average the last sampled values.
    level /= BATTERY_ADC_GRP_BUF_DEPTH;
    batt_level = level * ( 3.4 / 10325 );
}

void battery_init( battery_data_t * batt_data )
{
    // memset of temp structure
    memset( batt_data, 0, sizeof( battery_data_t ) );

    // Initializes the ADC driver 1 in order to access the analog values from the onboard temperature sensor.
    adcStart( &ADCD3, NULL );

    // Battery sensor initialization.
    palSetGroupMode( GPIOF, PAL_PORT_BIT( 9 ), 0, PAL_MODE_INPUT_ANALOG ); 
}

