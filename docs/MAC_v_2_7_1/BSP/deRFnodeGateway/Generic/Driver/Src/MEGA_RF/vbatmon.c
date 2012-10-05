/**
 * @file vbatmon.c
 *
 * @brief  Implementation of a battery voltage (VBAT) monitoring
 *
 * $Id: vbatmon.c,v 1.1.4.1 2011/07/08 16:59:08 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */
#include <avr/io.h>

#include "pal.h"                    // F_CPU
#include "vbatmon_interface.h"


/* === TYPES =============================================================== */

/* === MACROS / DEFINES ==================================================== */
#define VREF                     1600      // reference voltage in mV

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initializes the ADC. This includes Reference voltage and channel
 * selection.
 */
static void Batmon_AdcInit(void)
{
    // Select internal 1.6V reference voltage -> left AVDREF pin open!
    // (most precise approach due to voltage divider)!
    ADMUX |= (1 << REFS1) | (1 << REFS0);

    // Select external reference voltage (REFS1:0):=0b00
    //ADMUX |= (0 << REFS1) | (0 << REFS0);

    // wait until reference voltage is ok (only for _internal_ Vref
    // and only after ADC was enabled) -> may be skipped
    //while(!(ADCSRB & (1 << AVDDOK))) ;

    // Analog channel and gain selection, MUX spread over ADCSRA and ADCSRB
    // Select ADC channel 0 -> MUX5:0 = 0b00000 -> no changes necessary

    // select prescaler for 500 kHz frequency
#if (F_CPU == (16000000UL))
    ADCSRA |= (1 << ADPS2) | (1 <<ADPS0);       // Prescaler = 32
#elif (F_CPU == (8000000UL))
    ADCSRA |= (1 << ADPS2);                     // Prescaler = 16
#elif (F_CPU == (4000000UL))
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);      // Prescaler = 8
#elif (F_CPU == (1000000UL))
    ADCSRA |= (1 << ADPS0);                     // Prescaler = 2
#else
#error "unsupported F_CPU"
#endif

    // put into free running mode (ADTS2:0 = 0b000) -> no changes required
}


/**
 * @brief Performs one ADC measurement, then disables the ADC again
 * for power saving.
 *
 * @return the raw ADC measurement
 */
static uint16_t Batmon_AdcMeasure(void)
{
    uint16_t meas;

    // enable ADC
    ADCSRA |= (1 << ADEN);

    // Start Conversion, Clear ADIF
    ADCSRA |= (1 << ADSC);

    // wait for completion
    while (!(ADCSRA & (1 << ADIF))) ;

    // get measurement
    meas = ADC;

    // disable ADC
    ADCSRA &= ~(1 << ADEN);

    return meas;
}


/**
 * @brief Measures the battery voltage. Uses an MCU internal generated
 * reference voltage since its the most precise. The VREF pins must be
 * left open.
 *
 * @param voltage - the battery voltage in millivolts
 */
void Batmon_GetBatteryVoltage(uint16_t* voltage)
{
    uint16_t adc_val;

    // initialize ADC, then measure
    Batmon_AdcInit();
    adc_val = Batmon_AdcMeasure();

    // convert the measured value to real voltage (mV)
    // voltage divider: V_bat = 1040/220 * V_meas ~ 4.73 * Vmeas
    // max. resolution 10bit, V_ref/0x3FF = V_meas/adc_out
    // <-> V_bat = 15.24*adc_out ~ (61*V_meas)/4
    *voltage = 4.73f * ((float)VREF /(float)0x3FF) *(float)adc_val;
}


/* EOF */
