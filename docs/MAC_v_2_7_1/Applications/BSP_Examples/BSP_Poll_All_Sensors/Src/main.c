/**
 * @file main.c
 *
 * @addtogroup macExamples
 * @{
 * @addtogroup macExamples_BSP BSP_Poll_All_Sensors
 * @{
 *
 * @brief  Demo application showing usage of the Board Support Package
 *
 * The purpose of the current application is to demonstrate how the
 * BSP may be used. It polls all available sensors for their current
 * parameters and prints them out onto the choosen SIO channel.
 *
 * $Id: main.c,v 1.1.4.1 2011/07/14 18:28:19 dam Exp $
 *
 * @author    de: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2011, dresden elektronik All rights reserved.
 *
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
#include "app_config.h"
#include "sio_handler.h"
#include "bsp.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

static void app_task(void);
static void wait_n_ms(unsigned int timeout);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Main function of the application
 */
int main(void)
{
    /* Initialize the PAL layer */
    if (pal_init() != MAC_SUCCESS)
    {
        /* something went wrong during initialization */
        pal_alert();
    }

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();
    pal_led(LED_0, LED_ON);
    pal_led(LED_1, LED_OFF);
    pal_led(LED_2, LED_OFF);

    /* Initialize the BSP */
    bsp_init();

    /* The stack is initialized above, hence globally allow interrupts */
    pal_global_irq_enable();

    pal_led(LED_1, LED_ON);

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    pal_led(LED_2, LED_ON);

#if ((!defined __ICCAVR__) && (!defined __ICCARM__))
    fdevopen(_sio_putchar, _sio_getchar);
#endif

    /* wait for the first key stroke before continuing */
    sio_getchar();

    /* infinite loop */
    while (1)
    {
        app_task();
        pal_task();
    }
}


/**
 * @brief Application task
 */
static void app_task()
{
    luminosity_t lumi;
    temperature_t temp;
    acceleration_t acc;

    bsp_get_temperature(&temp);
    printf(" current temperature:  %c%d.%02d [dC] \r\n", (temp.sign?'-':'+'), temp.integralDigit, temp.fractionalDigit);

    bsp_get_luminosity(&lumi);
    printf(" current luminosity:   %4d [lux] \r\n", lumi);

    bsp_get_acceleration(&acc);
    printf(" current acceleration: %c%d.%02d, %c%d.%02d, %c%d.%02d  (x,y,z) [g] \r\n",
                   (acc.acc_x_sign ? '-' : '+'), acc.acc_x_integral, acc.acc_x_fractional,
                   (acc.acc_y_sign ? '-' : '+'), acc.acc_y_integral, acc.acc_y_fractional,
                   (acc.acc_z_sign ? '-' : '+'), acc.acc_z_integral, acc.acc_z_fractional);

    printf("\r\n");

    wait_n_ms(1000);
}


/**
 * @brief Wrapper function for delaying some time
 *
 * @param timeout the time to delay [ms]
 */
static void wait_n_ms(unsigned int timeout)
{
    while(timeout>0)
    {
        timeout--;
        pal_timer_delay(1000);
    }
}


/** @}*/
/** @}*/

/* EOF */
