/**
 * @file main.c
 *
 * This file holds the main function with the main loop for test harness.
 *
 * $Id: main.c 28613 2011-09-20 06:56:06Z viswanadham.kotla $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pal.h"
#include "return_val.h"
#include "bmm.h"
#include "qmm.h"
#include "tal.h"
#include "mac.h"
#include "app_config.h"
#include "sio_handler.h"
#include "mac_api.h"

/* === Macros ============================================================== */

/* === Globals ============================================================= */


/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

/**
 * Main function, initialization and main message loop
 *
 * @return error code
 */
int main (void)
{
    wpan_init();

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();

    /*
     * The global interrupt has to be enabled here as TAL uses the timer
     * delay which in turn requires interrupt to be enabled
     */
    pal_global_irq_enable();


    /* Initialize the serial interface used for communication with terminal program */
    pal_sio_init(SIO_CHANNEL);

    pal_sio_tx(SIO_CHANNEL, "\n\rMAC AT Serial Interface v1.0\n\r", 33);

    init_sio();

    /* Loop forever, the interrupts are doing the rest */
    while (1)
    {
        wpan_task();
        sio_task();
    }
    /* No return statement here, because this code is unreachable */
}
/* EOF */
