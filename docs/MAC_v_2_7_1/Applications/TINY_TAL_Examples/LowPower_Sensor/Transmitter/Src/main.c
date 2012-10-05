/**
 * @file main.c
 *
 * @addtogroup macExamples
 * @{
 * @addtogroup macExamples_LowPower_Sensor LowPower_Sensor
 * @{
 * @brief Demo application showing power efficient wireless sensor implementation
 *
 * @addtogroup macExamples_LowPower_Sensor_Transmitter Transmitter
 * @{
 *
 * @brief  Main of TINY_TAL Example - LowPower_Sensor, Transmitter part
 *
 * This application example demonstrates how low power sensor devices may be
 * implemented. It uses the sensors available on dresden elektronik 'deRFnode/
 * deRFgateway' development boards. The example cyclically reads the three
 * sensors current parameters plus the current battery voltage, then transmits
 * them all over the air. While not being used, as much peripherals as possible
 * are put to sleep and MCU clock is reduced / halted.
 *
 * The example uses three LEDs:
 *  - the first indicates the application initialization phase
 *  - the second, the first sensor access phase
 *  - the third the final radio transmission phase.
 *
 * Since the MAC stack does not provide power saving itself, we implemented
 * an own power saving module. Even if this performs well (device current
 * consumption <1mA during sleep), there are potentials left for further
 * power saving. All these were left out for clarity.
 *
 *
 * $Id: main.c,v 1.1.4.1 2011/07/14 18:37:09 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 */
/*
 * Copyright (c) 2010, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */

/* stdlib */
#include <stdint.h>
#include <stdlib.h>

/* stack */
#include "pal.h"
#include "pal_timer.h"
#include "tiny_tal.h"
#include "tiny_tal_internal.h"
#include "app_config.h"
#include "ieee_const.h"

/* BSP */
#include "pwr_mgmt.h"
#include "bsp_config.h"

/* === MACROS / DEFINES ==================================================== */

/* ensure that only supported platforms may compile this example */
#if ((PAL_GENERIC_TYPE != MEGA_RF) && (PAL_GENERIC_TYPE != ARM7))
#error "unsupported platform"
#endif

#if (TAL_TYPE == AT86RF212)
    #ifdef CHINESE_BAND
        #define DEFAULT_CHANNEL         (0)
        #define DEFAULT_CHANNEL_PAGE    (5)
    #else
        #define DEFAULT_CHANNEL         (1)
        #define DEFAULT_CHANNEL_PAGE    (0)
    #endif  /* #ifdef CHINESE_BAND */
#else
#define DEFAULT_CHANNEL                 (12)
#endif

#define DEFAULT_PAN_ID                  (0xCAFE)
#define DST_PAN_ID                      (DEFAULT_PAN_ID)
#define SRC_PAN_ID                      (DEFAULT_PAN_ID)
#define OWN_SHORT_ADDR                  (0x0001)
#define DST_SHORT_ADDR                  (0x0001)

/* Frame overhead due to selected address scheme incl. FCS */
#if (DST_PAN_ID == SRC_PAN_ID)
#define FRAME_OVERHEAD                  (11)
#else
#define FRAME_OVERHEAD                  (13)
#endif

#define PAYLOAD_LENGTH                  (2+3+2+9)

/* transmission interval in us */
#define TRX_INTERVAL                    (2000000)

/* transmission mode (CSMA_UNSLOTTED or NO_CSMA_NO_IFS) */
#define TRX_MODE                        CSMA_UNSLOTTED

/* === TYPES =============================================================== */

/* state machines internal state */
typedef enum state_tag
{
    INIT,                  // initialize
    MEASURE,               // perform measurement
    TRANSMIT               // transmit sensor data

} state_t;

/* === GLOBALS ============================================================= */

/* sensor values */
static temperature_t temperature;
static luminosity_t luminosity;
static acceleration_t acceleration;
static uint16_t vbat;

/* state machines internal state */
static volatile state_t state=INIT;

/* transmit frame buffer */
static uint8_t tx_buffer[LARGE_BUFFER_SIZE];

/* === PROTOTYPES ========================================================== */
static void handle_state_machine(void);
static void flash_init(void);
static void sensor_interface_init(void);
static void sensors_init(void);
static void init_trx(void);
static void prepare_frame(void);
static void update_frame(void);
static void timer_cb(void*);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the application
 */
int main(void)
{
    while(1)
    {
        handle_state_machine();
    }

    return 0;
}

/**
 * @brief Processes the state machine
 */
static void handle_state_machine(void)
{
    uint32_t timeout;

    switch(state)
    {
        case INIT:
            // initialize on-board peripherals and the PAL layer itself
            pal_calibrate_rc_osc();
            pal_init();
            pal_led_init();
            pal_led(LED_0, LED_ON);
            pal_global_irq_enable();

            // initialize the serial flash and sensor devices
            flash_init();
            sensor_interface_init();
            sensors_init();

            // prepare the static frame contents
            prepare_frame();

#if (PAL_GENERIC_TYPE == ARM7)
            // leave a chance for exchanging the firmware before node powers down
            for(uint8_t i=0; i<200; i++)
                pal_timer_delay(50000);
#endif

            // reduce the power consumption
            configure_power_saving();

            pal_led(LED_0, LED_OFF);
            state = MEASURE;
            break;

        case MEASURE:
            // (re-)initialize TWI Sensor interface
            sensor_interface_init();

            pal_led(LED_1, LED_ON);

            // wake up the sensors from sleep
            BMA150_WakeUp();

            // trigger measurement which will take some time
            TMP102_StartOneshotMeasurement();
            ISL29020_StartOneshotMeasurement();

            // get the battery voltage in the meantime
            Batmon_GetBatteryVoltage(&vbat);

            // get the remaining sensor parameters
            BMA150_GetAcceleration(&acceleration);
            ISL29020_GetLuminosity(&luminosity);
            TMP102_GetTemperature(&temperature, true);

            // put sensors to sleep again
            // (temperature and luminosity automatically power down)
            BMA150_PowerDown();

            pal_led(LED_1, LED_OFF);

            // format the frame to be transmitted
            update_frame();
            state = TRANSMIT;

        case TRANSMIT:
            // power-up and init transceiver
            tal_trx_wakeup();
            tal_init();
            tal_rx_enable(PHY_RX_ON);
            init_trx();

            // transmit the prepared frame
            pal_led(LED_2, LED_ON);
            tal_tx_frame(tx_buffer, TRX_MODE, true);
            pal_led(LED_2, LED_OFF);

            // poll for end of frame transmission, shutdown transceiver afterwards
            do {
                tal_task();
            } while (tal_state!= TAL_IDLE);
            tal_trx_sleep(SLEEP_MODE_1);

            // force energy saving
            low_power_mode();
            system_clock_down();

            // start a timer triggering subsequent transmissions
            timeout = (uint32_t) TRX_INTERVAL;
#if (PAL_GENERIC_TYPE == ARM7)
            // scale timeout according to current MCK
            // 48MHz / 32.768kHz ~ 1465
            timeout /= 1465;
#endif
            pal_timer_start(T_APP_TIMER,
                           timeout,
                           TIMEOUT_RELATIVE,
                           (FUNC_PTR) timer_cb,
                           NULL);

            system_suspend();

            // after wake-up, execution continues here
            system_wakeup();
            normal_power_mode();

            // handle PAL-internal timer queue
            timer_service();

            state = MEASURE;
            break;

        default:
            break;
    }
}


/**
 * @brief Initializes the external flash device and puts it into sleep mode
 */
static void flash_init(void)
{
    AT25_Init();
    AT25_DeepPowerDown();
}


/**
 * @brief Initializes the Two Wire Interface
 */
static void sensor_interface_init(void)
{
    if(TWI_SUCC != TWI_MasterInit())
        pal_alert();
}


/**
 * @brief Initializes the sensor devices
 */
static void sensors_init(void)
{
    uint8_t status;

    /* initialize the temperature sensor */
    status = TMP102_Init();
    if(status) pal_alert();

    /* initialize the light sensor */
    status = ISL29020_Init();
    if(status) pal_alert();

    /* initialize the acceleration sensor */
    status = BMA150_Init();
    if(status) pal_alert();
    status = BMA150_PowerDown();
    if(status) pal_alert();
}


/**
 * @brief Configures the TAL PIB
 */
static void init_trx(void)
{
    uint8_t temp_value_8;
    uint16_t temp_value_16;

    temp_value_16 = DEFAULT_PAN_ID;
    tal_pib_set(macPANId, (pib_value_t *)&temp_value_16);

    temp_value_16 = OWN_SHORT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value_16);

    temp_value_8 = DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value_8);

    // no frame retries
    temp_value_8 = 0;
    tal_pib_set(macMaxFrameRetries, (pib_value_t *)&temp_value_8);
    // since setting via the PIB does NOT work, write register directly
    pal_trx_bit_write(SR_MAX_FRAME_RETRIES, 0);

    // minimalize TX power
    // @see ieee_const.h:
    // 2 MBSs ... tx pwr tolerance (00 = 1 dB, 01 = 3 dB 10 = 6 dB)
    // 6 LSBs ... tx pwr in 2s-complement, "[...] The lowest value
    //      shall be interpreted as less than or equal to 32 dBm.
    temp_value_8 = 0x00;
    tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_value_8);
}


/**
 * @brief Prepare the frame to be sent later
 */
static void prepare_frame(void)
{
    uint16_t temp_value_16;
    uint16_t fcf = 0;

    /* Set static frame length to MHR */
    tx_buffer[0] = PAYLOAD_LENGTH + FRAME_OVERHEAD;

    /* Calculate and fill in FCF  */
    fcf = FCF_FRAMETYPE_DATA;
    fcf |= FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR);
    fcf |= FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);
#if (DST_PAN_ID == SRC_PAN_ID)
    fcf |= FCF_PAN_ID_COMPRESSION;
#endif
    //fcf |= FCF_ACK_REQUEST;
    convert_16_bit_to_byte_array(fcf, &tx_buffer[PL_POS_FCF_1]);

    /* Set initial sequence number. */
    tx_buffer[PL_POS_SEQ_NUM] = (uint8_t)rand();

    /* Set Destination PAN-Id in frame. */
    temp_value_16 = DEFAULT_PAN_ID;
    convert_16_bit_to_byte_array(temp_value_16, &tx_buffer[PL_POS_DST_PAN_ID_START]);

    /* Set Destination Address in frame. */
    temp_value_16 = DST_SHORT_ADDR;
    convert_16_bit_to_byte_array(temp_value_16, &tx_buffer[PL_POS_DST_ADDR_START]);

    /* Set Source PAN-Id in frame. */
#if (DST_PAN_ID != SRC_PAN_ID)
    temp_value_16 = DEFAULT_PAN_ID;
    convert_16_bit_to_byte_array(temp_value_16, &tx_buffer[PL_POS_DST_ADDR_START + 2]);
#endif

    /* Set Source Address in frame. */
    temp_value_16 = OWN_SHORT_ADDR;
#if (DST_PAN_ID != SRC_PAN_ID)
    convert_16_bit_to_byte_array(temp_value_16, &tx_buffer[PL_POS_DST_ADDR_START + 4]);
#else
    convert_16_bit_to_byte_array(temp_value_16, &tx_buffer[PL_POS_DST_ADDR_START + 2]);
#endif
}


/**
 * @brief Completes frame content prior to its transmission
 *
 * Fills in the payload and increments the sequence number.
 */
static void update_frame(void)
{
    uint8_t* pMSDU = &tx_buffer[LENGTH_FIELD_LEN + FRAME_OVERHEAD  - FCS_LEN];

    *pMSDU++ = (vbat>>8)&0xFF;
    *pMSDU++ = vbat&0xFF;

    *pMSDU++ = temperature.sign;
    *pMSDU++ = temperature.integralDigit;
    *pMSDU++ = temperature.fractionalDigit;

    *pMSDU++ = (luminosity>>8)&0xFF;
    *pMSDU++ = luminosity&0xFF;

    *pMSDU++ = acceleration.acc_x_sign;
    *pMSDU++ = acceleration.acc_x_integral;
    *pMSDU++ = acceleration.acc_x_fractional;
    *pMSDU++ = acceleration.acc_y_sign;
    *pMSDU++ = acceleration.acc_y_integral;
    *pMSDU++ = acceleration.acc_y_fractional;
    *pMSDU++ = acceleration.acc_z_sign;
    *pMSDU++ = acceleration.acc_z_integral;
    *pMSDU++ = acceleration.acc_z_fractional;

    // increment sequence number
    tx_buffer[PL_POS_SEQ_NUM]++;
}


/**
 * @brief Callback function for expired timers
 */
static void timer_cb(void* param)
{
    // No functionality required here, simply wakeup the uC
}


/** @}*/
/** @}*/
/** @}*/

/* EOF */
