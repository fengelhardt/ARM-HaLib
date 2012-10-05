/**
 * @file main.c
 *
 * @addtogroup macExamples
 * @{
 * @addtogroup macExamples_LowPower_Sensor LowPower_Sensor
 * @{
 * @addtogroup macExamples_LowPower_Sensor_Receiver Receiver
 * @{
 *
 * @brief  Main of TINY_TAL Example - LowPower_Sensor, Receiver part
 *
 * This application implements a receiver for the LowPower Sensor Application.
 * It configures the device for receiving messages sent by the transmitter and
 * prints out the enclosed current parameters of the sensor device. Static
 * addressing is used.
 *
 *
 * $Id: main.c,v 1.1.4.1 2011/07/14 18:37:09 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pal.h"
#include "tiny_tal.h"
#include "app_config.h"
#include "ieee_const.h"
#include "sio_handler.h"


/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

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
#define OWN_SHORT_ADDR                  (0x0001)

/**
 * Ensure that MAX_APP_DATA_LENGTH is defined, even if communication
 * goes not over the SIO_HUB (i.e. use DBGU on ARM modules)
 */
#ifndef MAX_APP_DATA_LENGTH
#define MAX_APP_DATA_LENGTH             (aMaxMACSafePayloadSize)
#endif

#if (NO_OF_LEDS >= 3)
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_1)
#define LED_DATA_TX                     (LED_2)
#elif (NO_OF_LEDS == 2)
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_0)
#define LED_DATA_TX                     (LED_1)
#else
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_0)
#define LED_DATA_TX                     (LED_0)
#endif


#define PAYLOAD_LENGTH                  (2+3+2+9)

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

static void configure_tal_pib(void);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Main function of the application
 */
int main(void)
{
    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();
    pal_led(LED_START, LED_ON);     // indicating application is started
    pal_led(LED_DATA_RX, LED_OFF);  // indicating data reception
    pal_led(LED_DATA_TX, LED_OFF);  // indicating successfull data transmission

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    /* Configure the Transceiver; e.g. set short address */
    configure_tal_pib();

    /* Switch receiver on */
    tal_rx_enable(PHY_RX_ON);

    /* Endless while loop */
    while (1)
    {
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
    }
}



/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param rx_frame_array Pointer to data array containing received frame
 */
void tal_rx_frame_cb(uint8_t *rx_frame_array)
{
    uint8_t rx_payload_len;
    uint8_t *rx_payload_ptr;
    uint16_t fcf, pan_id, transmitter_id;
    uint8_t frame_overhead;

    /* get the source PAN ID */
    pan_id = convert_byte_array_to_16_bit(&rx_frame_array[PL_POS_DST_PAN_ID_START]);

    /* dynamically check for PAN ID compression */
    fcf=convert_byte_array_to_16_bit(&rx_frame_array[PL_POS_FCF_1]);

    /* get transmitter address, dynamically determine frame overhead */
    if (fcf & FCF_PAN_ID_COMPRESSION) {
        transmitter_id = convert_byte_array_to_16_bit(&rx_frame_array[PL_POS_DST_ADDR_START + 2]);
        frame_overhead = 11;
    }
    else {
        transmitter_id = convert_byte_array_to_16_bit(&rx_frame_array[PL_POS_DST_ADDR_START + 4]);
        frame_overhead = 13;
    }

    /* determine MCPS length, get ptr to its start */
    rx_payload_len = rx_frame_array[0] - frame_overhead;
    rx_payload_ptr = rx_frame_array + frame_overhead + LENGTH_FIELD_LEN - FCS_LEN;

    /* only process packets having a correct payload (length as well as addressing) */
    if(rx_payload_len != PAYLOAD_LENGTH)
    {
        printf("\r\nwrong payload length, ignoring ...");

    } else {

        /*
         * format and print received data
         * payload format: 2 bytes temperature | 2 bytes luminosity | 9 bytes acceleration
         */
        if(pan_id == DEFAULT_PAN_ID)
        {
            printf("\r\nSensor %04X:%04X vbat: %04dmV, tmp: %c%02d.%02d°C, lum.: %04d lux, "
                    "acc.: (x,y,z) %c%01d.%02d,%c%01d.%02d,%c%01d.%02d g ",
                    pan_id, transmitter_id,
                    (rx_payload_ptr[0]<<8)+rx_payload_ptr[1],
                    (rx_payload_ptr[2] ? '-' : '+'),
                    rx_payload_ptr[3], rx_payload_ptr[4],
                    (rx_payload_ptr[5]<<8)+rx_payload_ptr[6],
                    (rx_payload_ptr[7] ? '-' : '+'),rx_payload_ptr[8],rx_payload_ptr[9],
                    (rx_payload_ptr[10] ? '-' : '+'),rx_payload_ptr[11],rx_payload_ptr[12],
                    (rx_payload_ptr[13] ? '-' : '+'),rx_payload_ptr[14],rx_payload_ptr[15]
                    );
        }
    }

    pal_led(LED_DATA_RX, LED_TOGGLE);    // indicating data reception
}


/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 */
void tal_tx_frame_done_cb(retval_t status)
{
    /* we dont like to TX anything */
}


/**
 * @brief Configure the transceiver for frame transmission
 */
static void configure_tal_pib(void)
{
    uint8_t temp_value_8;
    uint16_t temp_value_16;

    /* Set Source PAN-Id */
    temp_value_16 = DEFAULT_PAN_ID;
    tal_pib_set(macPANId, (pib_value_t *)&temp_value_16);

    /* Set Source Address */
    temp_value_16 = OWN_SHORT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value_16);

    /* Set proper channel. */
    temp_value_8 = DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value_8);
}


/** @}*/
/** @}*/
/** @}*/

/* EOF */
