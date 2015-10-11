_CONFIG1(JTAGEN_OFF & GCP_ON & GWRP_ON & LPCFG_ON & ICS_PGx3 & WINDIS_OFF & FWDTEN_WDT_SW & FWPSA_PR128 & WDTPS_PS8)
// JTAGEN_OFF :             JTAG port is disabled
// GCP_ON :                 Code protection is enabled for the entire program memory space
// GWRP_ON :                Writes to program memory are not allowed
// LPCFG_ON :               Low-voltage/retention regulator is always enabled
// ICS_PGx3 :               Emulator functions are shared with PGEC3/PGED3
// WINDIS_OFF :             Standard Watchdog Timer is enabled
// FWDTEN_WDT_SW :          WDT is enabled and controlled in firmware by the SWDTEN bit
// FWPSA_PR128 :            Prescaler ratio of 1:128
// WDTPS_PS32 :             1:32 Postscaler (128ms)

_CONFIG2(IESO_OFF & ALTVREF_ALT_AV_ALT_CV & FNOSC_FRCPLL & FCKSM_CSDCMD & OSCIOFCN_ON & IOL1WAY_OFF & BOREN1_EN & POSCMD_NONE)
// IESO_OFF :               IESO mode (Two-Speed Start-up) is disabled
// ALTVREF_ALT_AV_ALT_CV :  Comparator Voltage reference input VREF+ is RB0, VREF- is RB1, ADC VREF+ is RB0 and ADC VREF- is RB1
// FNOSC_FRCPLL :           Fast RC Oscillator with Postscaler and PLL module (FRCPLL)
// FCKSM_CSDCMD :           Clock switching and Fail-Safe Clock Monitor are disabled
// OSCIOFCN_ON :            OSCO/CLKO/RC15 functions as port I/O (RC15)
// IOL1WAY_OFF :            The IOLOCK bit can be set and cleared as needed, provided the unlock sequence has been completed
// BOREN1_EN :              BOR is enabled (CW3<12>, BOREN = 1)
// POSCMD_NONE :            Primary Oscillator mode is disabled

_CONFIG3(WPFP_WPFP127 & VBTBOR_OFF & SOSCSEL_OFF & WDTWIN_PS25_0 & BOREN_ON & WPDIS_WPDIS & WPEND_WPENDMEM)
//_CONFIG3( WPEND_WPSTARTMEM & WPCFG_WPCFGEN & WPDIS_WPEN & BOREN_ON & WDTWIN_PS25_0 & SOSCSEL_OFF & VBTBOR_OFF & WPFP_WPFP63 )
// WPEND_WPSTARTMEM : Protected code segment lower boundary is at the bottom of the program memory (000000h); upper boundary is the code page specified by WPFP<6:0>
// WPCFG_WPCFGEN : Last page and Flash Configuration Words are write-protected provided WPDIS = 0
// WPDIS_WPEN : Segmented code protection is enabled; protected segment is defined by the WPEND, WPCFG and WPFPx Configuration bits
// BOREN_ON : BOR is enabled (all modes except Deep Sleep) (BOREN1 = 1)
// WDTWIN_PS25_0 : Watchdog Timer Window Width Select bits
// SOSCSEL_OFF : Digital (SCLKI) mode : Ensure that the SCLKI pin is made a digital input while using this configuration
// VBTBOR_OFF : VBAT BOR is disabled
// WPFP_WPFP63 : For the 62K devices: PIC24FJ64GA310, PIC24FJ64GA308 and PIC24FJ64GA306, bit 6 should be maintained as ???0???.

_CONFIG4(DSSWEN_OFF & DSWDTEN_OFF & DSBOREN_OFF & DSWDTOSC_LPRC & DSWDTPS_DSWDTPS8)
// DSSWEN_ON : Deep Sleep operation is enabled and controlled by the DSEN bit
// DSWDTEN_ON : Deep Sleep WDT is enabled
// DSBOREN_ON : BOR is enabled in Deep Sleep mode
// DSWDTOSC_LPRC : Deep Sleep Watchdog Timer Clock Select bit : Clock source is LPRC
// DSWDTPS_DSWDTPS8 : Deep Sleep Watchdog Timer Postscale Select bits 1:8,192 (264.3 ms)