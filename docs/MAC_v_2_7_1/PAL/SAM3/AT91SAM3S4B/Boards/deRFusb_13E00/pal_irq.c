/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts.
 *
 * $Id: pal_irq.c,v 1.1.8.2 2011/07/12 16:44:07 dam Exp $
 *
 *  @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* === Includes ============================================================= */

#include <stdint.h>
#include "pal.h"
#include "pal_config.h"
#include "pal_boardtypes.h"


#if (BOARD_TYPE == deRFusb_13E00)

/* === Types ============================================================== */

/**
 * This is a typedef of the function which is called from the transceiver ISR
 */
typedef void (*irq_handler_t)(void);

/* === Globals ============================================================== */

/* Pins for TRX and RX timestamp interrupt */
const pin_t pin_radio_irq   = TRX_INT_PIN;
#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)) && (!defined(ANTENNA_DIVERSITY))
const pin_t pin_radio_rx_ts = TRX_RXTS_PIN;
#endif

/* Function pointers to store the callback functions of the transceiver interrupts */
static irq_handler_t irq_hdl_trx;
#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)) && (!defined(ANTENNA_DIVERSITY))
static irq_handler_t irq_hdl_trx_tstamp;
#endif


/* === Prototypes =========================================================== */
static void trx_isr(void);
#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)) && (!defined(ANTENNA_DIVERSITY))
static void trx_tstamp_isr(void);
#endif

/* === Implementation ======================================================= */

/**
 * @brief Initializes the transceiver interrupts
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver interrupts
 *
 * @param trx_irq_cb Callback function for the given transceiver interrupt
 */
void pal_trx_irq_init(FUNC_PTR trx_irq_cb)
{
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for
     * spurious interrupts, that can pop up the moment they are enabled.
     */
    irq_hdl_trx = (irq_handler_t)trx_irq_cb;

    /* Configure the pin and interrupt */
    pal_pio_configure(&pin_radio_irq, PIO_LISTSIZE(pin_radio_irq));
    pal_pio_configure_interrupt(&pin_radio_irq, (void (*)(const pin_t *))trx_isr);

    /* Configure the NVIC interrupt */
    NVIC_EnableIRQ(PIOA_IRQn);

    /* Clear the interrupt bit corresponding to the regular TRX interrupt. */
    CLEAR_TRX_IRQ();
}

#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)) && (!defined(ANTENNA_DIVERSITY))
void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb)
{
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for spurious
     * interrupts, that can pop up the moment they are enabled
     */
    irq_hdl_trx_tstamp = (irq_handler_t)trx_irq_cb;

    /* Configure the pin and interrupt */
    pal_pio_configure(&pin_radio_rx_ts, PIO_LISTSIZE(pin_radio_rx_ts));
    pal_pio_configure_interrupt(&pin_radio_rx_ts, (void (*)(const pin_t *))trx_tstamp_isr);

    /* Configure the NVIC interrupt */
    NVIC_EnableIRQ(PIOA_IRQn);
    
    /* Clear the interrupt bit corresponding to the RXTS interrupt. */
    CLEAR_TRX_IRQ_TSTAMP();
}
#endif


/**
 * @brief Handler for TRX interrupt
 *
 * This function is an handler for interrupt (transceiver interrupt) of the
 * microcontroller. It calls the user installed handler for transceiver
 * interrupt.
 */
void trx_isr(void)
{
    irq_hdl_trx();
}


#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)) && (!defined(ANTENNA_DIVERSITY))
/**
 * @brief Handler for TRX Tstamp interrupt
 *
 * This function is an handler for interrupt (trx time stamp interrupt) of the
 * microcontroller. It calls the user installed handler for tstamp
 * interrupt.
 */
void trx_tstamp_isr(void)
{
    irq_hdl_trx_tstamp();
}
#endif


#endif  /* (BOARD_TYPE == deRFusb_13E00) */

/* EOF */
