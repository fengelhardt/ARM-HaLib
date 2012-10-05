/**
 * @file main.c
 *
 * @brief  Main of TAL Example - Performance_Test_EVK
 *
 * $Id: main.c 28809 2011-10-03 13:48:18Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright(c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include "pal.h"
#include "tal.h"
#include "tfa.h"
#include "app_config.h"
#include "ieee_const.h"
#include "bmm.h"
#include "sio_handler.h"
#include "app_frame_format.h"
#include "app_pairing.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

#define COUNT_FOR_VALID_KEY_PRESS       20
#define LED_ON_DURAION_MICRO_SEC        100000

/* === PROTOTYPES ========================================================== */

static void configure_pibs(void);
/* Functions for Startup */
static void app_task_startup(void);
static void app_rx_frame_cb_for_startup(frame_info_t *frame);
static void app_switch_off_tx_led_cb(void* parameter);
static void app_switch_off_rx_led_cb(void* parameter);

/* === GLOBALS ============================================================= */

frame_info_t *tx_frame_info;

void (*app_task)(void);
void (*app_tx_frame_done_cb)(retval_t status, frame_info_t *frame);
void (*app_rx_frame_cb)(frame_info_t *frame);

uint8_t storage_buffer[LARGE_BUFFER_SIZE];

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        board_t board;
    #endif
#endif

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the Performance_Test_EVK application
 */
int main(void)
{

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        board = board_identify();
    #endif
#endif

    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        /* something went wrong during initialization */
        pal_alert();
    }

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();

    /* Initialize buttons */
    pal_button_init();

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        /* something went wrong during initialization */
        pal_alert();
    }

#if ((!defined __ICCAVR__) && (!defined __ICCARM__) && (!(defined __GNUC__ && defined __AVR32__))&& (!defined __GNUARM__))
    fdevopen(_sio_putchar, _sio_getchar);
#endif

    /* Setting stdin & stdout as unbuffered for AVR32 & ARM GCC*/
#if ((defined __GNUAVR32__) || (defined __GNUARM__))
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
#endif

    /* Init tx frame info structure value that do not change during program execution */
    tx_frame_info = (frame_info_t *)storage_buffer;

    /* Initialize startup application */
    app_task_startup_init();

    /* Endless while loop */
    while (1)
    {
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
        app_task(); /* Application task */
    }
}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param frame Pointer to received frame
 */
void tal_rx_frame_cb(frame_info_t *frame)
{
    if(NULL != app_rx_frame_cb)
    {
        /* Call application specific callbacks */
        app_rx_frame_cb(frame);
    }

    /* free buffer that was used for frame reception */
    bmm_buffer_free((buffer_t *)(frame->buffer_header));
}

/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
    if(NULL != app_tx_frame_done_cb)
    {
        /* Call application specific callbacks */
        app_tx_frame_done_cb(status, frame);
    }
}

/**
 * @brief Application initialization
 */
void app_task_startup_init()
{
    /* Initialise the application with app_task_startup */
    app_task = &app_task_startup;
    app_rx_frame_cb = &app_rx_frame_cb_for_startup;

    /* Configure the TAL PIBs; e.g. set short address */
    configure_pibs();

    /* Leave the radio in RX mode in default channel */
    tal_rx_enable(PHY_RX_ON);

    /* reset the pairing status to UNPAIRED */
    app_pairing_reset();

    /* indicating application is started */
    app_led_event(LED_EVENT_POWER_ON);
}

/**
 * @brief Application task in during startup
 */
static void app_task_startup(void)
{
    uint16_t char_received;
    uint8_t key_press;

    /* See if any character received on the UART */
    char_received = sio_getchar_nowait();

    /* Check for any key press */
    key_press = app_debounce_button();

    if(key_press != 0)
    {
        app_task_pairing_init(OP_MODE_1_KEY_PRESS);
    }
    else if(char_received != 0xFFFF)
    {
        app_task_pairing_init(OP_MODE_2_CHAR_ON_UART);
    }
}

/**
 * @brief Callback that is called if data has been received by trx in startup mode.
 *
 * @param frame    Pointer to received data structure
 */
static void app_rx_frame_cb_for_startup(frame_info_t *frame)
{
    app_task_pairing_init(OP_MODE_UNKNOWN);
    if(app_rx_frame_cb != NULL)
    {
        app_rx_frame_cb(frame);
    }
}

/**
 * @brief Configure the TAL PIbs
 */
static void configure_pibs(void)
{
    uint16_t temp_value_16;
    uint8_t temp_value_8;

    /* Set Default address. */
    temp_value_16 = CCPU_ENDIAN_TO_LE16(DEFAULT_ADDR);
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value_16);

    /* Set PAN ID. */
    temp_value_16 = CCPU_ENDIAN_TO_LE16(SRC_PAN_ID);
    tal_pib_set(macPANId, (pib_value_t *)&temp_value_16);

    /* Set channel. */
    temp_value_8 = (uint8_t)DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value_8);
}

/**
 * @brief Turns LEDs on / off to indicate specific events.
 *
 * @param ev the event to be indicated.
 *
 * @ingroup apiPalAppDemo
 */
void app_led_event(led_event_t ev)
{
    switch(ev)
    {
        case LED_EVENT_TX_FRAME:
        {
            pal_led(TX_LED,LED_ON);
            pal_timer_start(APP_TIMER_TO_TX_LED_OFF,
                                LED_ON_DURAION_MICRO_SEC,
                                TIMEOUT_RELATIVE,
                                (FUNC_PTR)app_switch_off_tx_led_cb,
                                NULL);
        }
        break;

        case LED_EVENT_RX_FRAME:
        {
            pal_led(RX_LED,LED_ON);
            pal_timer_start(APP_TIMER_TO_RX_LED_OFF,
                                LED_ON_DURAION_MICRO_SEC,
                                TIMEOUT_RELATIVE,
                                (FUNC_PTR)app_switch_off_rx_led_cb,
                                NULL);
        }
        break;

        case LED_EVENT_POWER_ON:
            pal_led(STATUS_LED, LED_OFF);
            pal_led(TX_LED, LED_ON);
            pal_led(RX_LED, LED_OFF);
        break;

        case LED_EVENT_START_PAIRING:
            pal_led(STATUS_LED, LED_OFF);
            pal_led(TX_LED, LED_OFF);
            pal_led(RX_LED, LED_OFF);
        break;

        case LED_EVENT_PAIRING_DONE:
            pal_led(STATUS_LED, LED_ON);
            pal_led(TX_LED, LED_OFF);
            pal_led(RX_LED, LED_OFF);
        break;

        case LED_EVENT_ALL_ON:
            pal_led(STATUS_LED, LED_ON);
            pal_led(TX_LED, LED_ON);
            pal_led(RX_LED, LED_ON);
        break;

        case LED_EVENT_ALL_OFF:
            pal_led(STATUS_LED, LED_OFF);
            pal_led(TX_LED, LED_OFF);
            pal_led(RX_LED, LED_OFF);
        break;
    }
}

/**
 * @brief Button debounce routine.
 *
 * Helper function for debouncing the transmit button.
 * @return ret 1 if a button event is detected, 0 otherwise.
 *
 * @ingroup apiPalAppDemo
 */
uint8_t app_debounce_button(void)
{
    button_id_t app_start_button = BUTTON_0;
    uint8_t ret = 0;
    static uint8_t keycnt;
    button_state_t temp = pal_button_read(app_start_button);

    if(BUTTON_PRESSED == temp)
    {
        if (keycnt != COUNT_FOR_VALID_KEY_PRESS)
        {
            keycnt++;
        }
    }
    else if(BUTTON_OFF == temp && (keycnt == COUNT_FOR_VALID_KEY_PRESS))
    {
        ret = 1;
        keycnt = 0;
    }
    else
    {
        keycnt = 0;
    }
    return ret;
}

/**
 * @brief Callback function switching off the TX_LED
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
void app_switch_off_tx_led_cb(void* parameter)
{

    /* switch off the LED */
    pal_led(TX_LED, LED_OFF);

    /* keep the compiler happy */
    parameter = parameter;

}

/**
 * @brief Callback function switching off the RX_LED
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
void app_switch_off_rx_led_cb(void* parameter)
{

    /* switch off the LED */
    pal_led(RX_LED, LED_OFF);

    /* keep the compiler happy */
    parameter = parameter;

}
/* EOF */
