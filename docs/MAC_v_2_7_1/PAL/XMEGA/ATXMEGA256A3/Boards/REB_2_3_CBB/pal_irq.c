/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts.
 *
 * $Id: pal_irq.c 27841 2011-07-26 08:53:18Z sschneid $
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
#include "pal.h"
#include "pal_boardtypes.h"

#if (BOARD_TYPE == REB_2_3_CBB)

/* === Types ============================================================== */


/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
static irq_handler_t irq_hdl_trx;

/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Initializes the transceiver main interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver main interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver main
 * interrupt
 */
void pal_trx_irq_init(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx = (irq_handler_t)trx_irq_cb;

    /* Rising edge on IRQ pin used to trigger IRQ */
    PORTC.PIN1CTRL = PORT_ISC0_bm;

    /* Set pin 1 as source for port interrupt 0 */
    PORTC.INT0MASK = PIN1_bm;

    /* Clear pending interrupts */
    PORTC.INTFLAGS = PORT_INT0IF_bm;
}


/**
 * @brief ISR for transceiver's main interrupt
 */
ISR(TRX_MAIN_ISR_VECTOR)
{
    /* clear capture interrupt */
    TCC1_INTFLAGS |= TC1_CCAIF_bm;

    irq_hdl_trx();
}

#endif /* REB_2_3_CBB */

/* EOF */
