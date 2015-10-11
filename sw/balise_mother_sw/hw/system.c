#include "adc.h"
#include "ADXL362.h"
#include "delay.h"
#include "global_variables.h"
#include "gpio.h"
#include "irq.h"
#include "spi.h"
#include "system.h"
#include "timer.h"
#include "uart.h"

//=== PROTOTYPES ===
void init_PLL( void );
void disable_peripherals( void );
void DMA_off( void );
void PPS_off( void );
void PPS_init( void );

void global_init( void )
{
    irq_off();
    irq_config( IVT_MODE );

    global_variables_init();
    disable_peripherals();

    init_io();
    init_PLL();                 // Config main and aux OSC
    init_timers();
    DMA_off();
    PPS_off();
    PPS_init();
    init_spi();
    init_uart();
    init_adc();
    init_irq();
}

void init_PLL( void )
{
    u16 timeout;

    // Main OSC : internal 8.0 MHz + PLL to go to 32.0 MHz
    OSCTUN = 0;

    CLKDIV = 0;
    // ROI = 0b0            Interrupts have no effect on the DOZEN bit
    // DOZE<2:0> = 0b000    CPU Peripheral Clock Ratio Select bits 1:1
    // DOZEN = 0b0          CPU peripheral clock ratio set to 1:1
    // RCDIV<2:0> = 0b000   RCDIV<2:0>: FRC Postscaler Select bits : 000 = 8 MHz (divide-by-1)

    timeout = 0xFF;
    
    // Wait for PLL lock
    while( ( OSCCONbits.LOCK != 1 ) && ( timeout != 0 ) )    
    {
        timeout--;
    }
}

void disable_peripherals(void)
{
    // T1
    T1CON = 0;
    PR1 = 0;

    // T2
    T2CON = 0;
    PR2 = 0;

    // SPI1 : ZIGBEE
    SPI1STAT = 0;
    SPI1CON2 = 0;

    // SPI2 : ADXL
    SPI2STAT = 0;
    SPI2CON2 = 0;

    // UART1
    U1STA = 0;
    U1MODE = 0;

    // UART2
    U2STA = 0;
    U2MODE = 0;

    // Disable all modules
    PMD1 = 0xffff;
    PMD2 = 0xffff;
    PMD3 = 0xffff;
    PMD4 = 0xffff;
    PMD6 = 0xffff;
    PMD7 = 0xffff;
}

void DMA_off( void )
{
    volatile unsigned int *reg_addr;

    for( reg_addr = &DMACON; reg_addr <= &DMACNT5; reg_addr++ )
    {
        *reg_addr = 0;
    }
}

void PPS_off( void )
{
    volatile unsigned int *reg_addr;

    // Reset all RPINR, PROR0 registers
    for( reg_addr = &RPINR0; reg_addr <= &RPINR31; reg_addr++ )
    {
        *reg_addr = 0xFFFF;
    }
    
    for( reg_addr = &RPOR0; reg_addr <= &RPOR15; reg_addr++ )
    {
        *reg_addr = 0;
    }
}

void PPS_init( void )
{
    __builtin_write_OSCCONL( OSCCON & 0xbf );

    // SPI1 : bus master interne MRF24J40 : ZIGBEE
    // SPI1_CLK on SCK1OUT (fonction code 8) linked to pin 51 = RP22
    RPOR11bits.RP22R = 8;
    // SPI1_MOSI on SD01 (fonction code 7) linked to  pin 52 = RP25
    RPOR12bits.RP25R = 7;
    // SPI1_MISO on SDI1 localized in RPINR23 linked from pin 50 = RP23
    RPINR20bits.SDI1R = 23;

    // INT1 : IRQ MRF24J40
    RPINR0bits.INT1R = 20;

    // INT For ADXL : TODO need to be updated for V201
    RPINR1bits.INT2R = 27;

    // SPI2 : bus master interne ADXL362
    // SPI2_CLK on SCK2OUT (fonction code 11) linked to pin 30 = RP29
    RPOR14bits.RP29R = 11;
    // SPI2_MOSI on SD02 (fonction code 10) linked to pin 34 = RP30
    RPOR15bits.RP30R = 10;
    // SPI2_MISO on SDI2 localized in RPINR22 linked from pin 33 = RPI16
    RPINR22bits.SDI2R = 16;

    /************************/
    /*       UART           */
    /************************/

    // UART1 : debug sur J6
    // RS232_RX on U1RX localized in RPINR18 linked from pin RP8
    RPINR18bits.U1RXR = 8;
    // RS232_TX on U1TX (fonction code 3) linked to pin RP9
    RPOR4bits.RP9R = 3;

    //UART2
    RPINR19bits.U2RXR = 18;     // Assign U2RX to pin RP18
    RPOR14bits.RP28R = 5;       // Assign U2TX To pin RP28

    __builtin_write_OSCCONL( OSCCON | 0x40 );
}