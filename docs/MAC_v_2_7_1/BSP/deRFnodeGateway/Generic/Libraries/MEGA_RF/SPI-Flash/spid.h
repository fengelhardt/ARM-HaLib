/**
 * @file spid.h
 *
 * @brief header file for spid.c.
 *
 * $Id: spid.h,v 1.1.4.1 2011/07/08 16:59:09 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2010, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef _SPID_H
#define _SPID_H

/* === INCLUDES ============================================================= */
#include <avr/io.h>                 // MCU IO definitions

/* === MACROS / DEFINES ==================================================== */

/** Wait for SPI interrupt flag */
#define SPI_WAIT()                  while (!(SPSR & (1 << SPIF)))

/** Dummy value written in SPDR to retrieve data from it. */
#define SPI_DUMMY_VALUE             (0x00)

/* === TYPES ================================================================ */

/* === PROTOTYPES =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the MCU as single SPI master device
 */
void SPI_MasterInit(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* === INLINE FUNCTIONS ==================================================== */


#endif /* _SPID_H */

/* EOF */
