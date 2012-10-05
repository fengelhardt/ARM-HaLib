/**
 * @file pal_boardtypes.h
 *
 * @brief PAL board types for ATxmega256A3
 *
 * This header file contains board types based on ATxmega256A3.
 *
 * $Id: pal_boardtypes.h 27443 2011-06-23 11:08:00Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_BOARDTYPES_H
#define PAL_BOARDTYPES_H

/* === Includes ============================================================= */

#if defined(VENDOR_BOARDTYPES) && (VENDOR_BOARDTYPES != 0)
#include "vendor_boardtypes.h"
#else   /* Use standard board types as defined below. */

/* === Macros =============================================================== */

/* Boards for AT86RF230B */

/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB230B V2.3
 */
#define REB_2_3_STK600              (0x01)

/* REB Controller base board with
 * - Radio Extender board REB230B V2.3
 */
#define REB_2_3_CBB                 (0x02)



/* Boards for AT86RF231 */
/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231 V4.0.1
 */
#define REB_4_0_STK600              (0x11)

/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231 V4.0.1
 * CLKM from transceiver is used as timer source
 */
#define REB_4_0_STK600_USING_CLKM   (0x12)

/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231ED V4.1.1
 */
#define REB_4_1_STK600              (0x13)

/* REB Controller base board with
 * - Radio Extender board REB231 V4.0.1
 */
#define REB_4_0_CBB                 (0x14)

/* REB Controller base board with
 * - Radio Extender board REB231ED V4.1.1
 */
#define REB_4_1_CBB                 (0x15)

/* REB Controller base board with
 * - Radio Extender board REB231FE2 V4.5
 * - http://sige.com/files/SiGe_SE2431L_Datasheet_Rev_1_7_SF_Apr-07-2010%281%29.pdf
*/
#define REB_4_5_CBB                 (0x16)

/* Boards for AT86RF212 */
/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB212 V5.0.2
 */
#define REB_5_0_STK600              (0x21)

/* REB Controller base board with
 * - Radio Extender board REB212 V5.0.2
 */
#define REB_5_0_CBB                 (0x22)

/* REB Controller base board with
 * - Radio Extender board REB212SMA v5.5 (not existent yet, aligned to REB231FE2 v4.5)
 * - RFFE type: n/a
*/
#define REB_5_5_CBB                 (0x23)

/* Boards for AT86RF232 */
/* REB Controller base board with
 * - Radio Extender board REB232 V7.0.0
 */
#define REB_7_0_CBB                 (0x31)

/* REB Controller base board with
 * - Radio Extender board REB232ED V7.1.0
 */
#define REB_7_1_CBB                 (0x32)


/* Boards for AT86RF233 */
/* REB Controller base board with
 * - Radio Extender board REB233SMAD V8.1.0
 */
#define REB_8_1_CBB                 (0x41)

#endif  /* #if defined(VENDOR_BOARDTYPES) && (VENDOR_BOARDTYPES != 0) */

#endif  /* PAL_BOARDTYPES_H */

/* EOF */
