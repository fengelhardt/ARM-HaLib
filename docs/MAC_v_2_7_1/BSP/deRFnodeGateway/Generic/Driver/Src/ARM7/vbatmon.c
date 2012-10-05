/**
 * @file vbatmon.c
 *
 * @brief  Implementation of a battery voltage (VBAT) monitoring
 *
 * $Id: vbatmon.c,v 1.1.4.1 2011/07/08 16:59:09 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */
#include <board.h>
#include <adc/adc.h>
#include <pio/pio.h>

#include "bsp_config.h"
#include "vbatmon_interface.h"

/* === TYPES =============================================================== */

/* === MACROS / DEFINES ==================================================== */

// reference voltage in mV (Vcc)
#define VREF                      3300

// ADC frequency
#define BOARD_ADC_FREQ            500000

// Startup time max, return from Idle mode (in µs)
#define ADC_STARTUP_TIME_MAX      20

// Track and hold Acquisition Time min (in ns)
#define ADC_TRACK_HOLD_TIME_MIN   600


/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initializes the ADC. This includes reference voltage and channel
 * selection.
 */
static void Batmon_AdcInit(void)
{
    const Pin pin = PIN_VBATMON;

    PIO_Configure(&pin, 1);

    ADC_Initialize( AT91C_BASE_ADC,
                    AT91C_ID_ADC,
                    AT91C_ADC_TRGEN_DIS,
                    0,
                    AT91C_ADC_SLEEP_NORMAL_MODE,
                    AT91C_ADC_LOWRES_10_BIT,
                    BOARD_MCK,
                    BOARD_ADC_FREQ,
                    ADC_STARTUP_TIME_MAX,
                    2*ADC_TRACK_HOLD_TIME_MIN);
}


/**
 * @brief Performs one ADC measurement, then disables the ADC again
 * for power saving.
 *
 * @return the raw ADC measurement
 */
static unsigned int Batmon_AdcMeasure(void)
{
    unsigned int meas;

    // enable ADC, start conversion
    ADC_EnableChannel(AT91C_BASE_ADC, ADC_CHANNEL_0);
    ADC_StartConversion(AT91C_BASE_ADC);

    // wait for conversion termination
    while (!ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC), ADC_CHANNEL_0));

    // read measurement
    meas = ADC_GetConvertedData(AT91C_BASE_ADC, ADC_CHANNEL_0);

    // deactivate the ACD for power saving
    ADC_DisableChannel(AT91C_BASE_ADC, ADC_CHANNEL_0);

    return meas;
}


/**
 * @brief Measures the battery voltage. Requires an external reference voltage
 * on the VREF pin (X8:32 on the extension header). This may be i.e. achieved
 * by interconnecting this pin to VCC (X8:33). Also the VREF macro above must
 * be set accordingly.
 *
 * @param voltage - the battery voltage in millivolts
 */
void Batmon_GetBatteryVoltage(uint16_t* voltage)
{
    unsigned int adc_val;

    // initialize the ADC, then measure
    Batmon_AdcInit();
    adc_val = Batmon_AdcMeasure();

    // convert the measured value to real voltage (mV)
    // voltage divider: V_bat = 1040/220 * V_meas ~ 4.73 * Vmeas
    // max. resolution 10bit, V_ref/0x3FF = V_meas/adc_out
    // <-> V_bat  = 15.24*adc_out ~ (61*V_meas)/4
    *voltage = 4.73f * ((float)VREF /(float)0x3FF) *(float)adc_val;
}


/* EOF */
