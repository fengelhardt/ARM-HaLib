/**
 * @file sio_handler.h
 *
 * @brief This file contains macros and function prototypes for SIO handling.
 *
 * $Id: sio_handler.h 28613 2011-09-20 06:56:06Z viswanadham.kotla $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef SIO_HANDLER_H
#define SIO_HANDLER_H

/* === Includes ============================================================= */


/* === Macros =============================================================== */
#ifdef USB0
#define SIO_CHANNEL  SIO_2
#elif (defined(UART0))
#define SIO_CHANNEL  SIO_0
#elif (defined(UART1))
#define SIO_CHANNEL  SIO_1
#elif (defined(UART2))
#define SIO_CHANNEL  SIO_3
#elif (defined(UART3))
#define SIO_CHANNEL  SIO_4
#endif

/* === Types ================================================================ */

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    void init_sio(void);
    void sio_task();
    void sio_write(uint8_t *message);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SIO_HANDLER_H */

/* EOF */
