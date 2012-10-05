/**
 * @file pal_sio_hub.c
 *
 * @brief Stream I/O API functions
 *
 * This file implements the Stream I/O API functions.
 *
 * $Id: pal_sio_hub.c,v 1.2.2.1 2011/07/08 16:46:07 dam Exp $
 *
 */
/*  @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */


//#ifdef SIO_HUB // we need pal_sio_init() even if SIO_HUB was not defined

/* === Includes ============================================================ */
#include <stdint.h>
#include "pal.h"
#include "return_val.h"
#include "pal_usb.h"

#if ((PAL_GENERIC_TYPE == SAM3) && !(defined SIO_HUB))
#include "usart/uart_console.h"
#endif


/* === Globals =============================================================*/

/* === Prototypes ==========================================================*/

/* === Implementation ======================================================*/

/**
 * @brief Initializes the requested SIO unit
 *
 * This function initializes the requested SIO unit.
 *
 * @param sio_unit Specifies the SIO uint to be initialized
 *
 * @return MAC_SUCCESS  if SIO unit is initialized successfully, FAILURE
 *         otherwise
 */
retval_t pal_sio_init(uint8_t sio_unit)
{
    retval_t status = MAC_SUCCESS;

#ifdef SIO_HUB
    /* allow applications stay untouched while working with DBGU */
    switch (sio_unit)
    {
#if defined USB0
        case SIO_2: sio_usb_0_init();
                    break;
#else
#error "SIO HUB currently does not support UARTs"
#endif
        default:    status = FAILURE;
                    break;
    }
#endif /* SIO_HUB */

    return status;
}



/**
 * @brief Transmits data through selected SIO unit
 *
 * This function transmits data through the selected SIO unit.
 *
 * @param sio_unit Specifies the SIO unit
 * @param data Pointer to the data to be transmitted is present
 * @param length Number of bytes to be transmitted
 *
 * @return Actual number of transmitted bytes
 */
uint8_t pal_sio_tx(uint8_t sio_unit, uint8_t *data, uint8_t length)
{
    uint8_t number_of_bytes_transmitted=0;

#ifdef SIO_HUB

    switch (sio_unit)
    {
#if defined USB0
        case SIO_2:
                    /*
                     * Perform retransmissions if the SIO buffer has less space
                     * available than the current transmission requires.
                     */
                    do {
                        number_of_bytes_transmitted += sio_usb_0_tx(data,
                                            length-number_of_bytes_transmitted);
                        data += number_of_bytes_transmitted;
                    } while (number_of_bytes_transmitted < length);

                    break;

#else
#error "SIO HUB currently does not support UARTs"
#endif

        default:    number_of_bytes_transmitted = 0;
                    break;
    }
#else
    /* SIO_HUB not defined -> redirection to DBGU */
    ENTER_TRX_REGION();
    number_of_bytes_transmitted = length;
    do
    {
        UART_PutChar(*data);
        data++;
        length--;

    } while(length>0);

    LEAVE_TRX_REGION();
#endif

    return (number_of_bytes_transmitted);
}



/**
 * @brief Receives data from selected SIO unit
 *
 * This function receives data from the selected SIO unit.
 *
 * @param[in] sio_unit Specifies SIO unit
 * @param[out] data Pointer to the buffer to store received data
 * @param[in] max_length Maximum number of bytes to be received
 *
 * @return Actual number of received bytes
 */
uint8_t pal_sio_rx(uint8_t sio_unit, uint8_t *data, uint8_t max_length)
{
    uint8_t number_of_bytes_received;

#ifdef SIO_HUB
    switch (sio_unit)
    {

#if defined USB0
        case SIO_2: number_of_bytes_received = sio_usb_0_rx(data, max_length);
                    break;
#else
#error "SIO HUB currently does not support UARTs"
#endif

        default:    number_of_bytes_received = 0;
                    break;
    }
#else
    /* SIO_HUB not defined --> redirect from DBGU */
    number_of_bytes_received=0;
    do {
        if(!UART_IsRxReady())
            break;
        *data = UART_GetChar();
        data++;
        max_length--;
        number_of_bytes_received++;
    } while(max_length>0);
#endif

    return (number_of_bytes_received);
}


//#endif /* SIO_HUB */

/* EOF */
