/**
 * @file pal_boardtypes.h
 *
 * @brief PAL board types for AT91SAM3S4B
 *
 * This header file contains board types based on AT91SAM3S4B
 *
 * $Id: pal_boardtypes.h,v 1.2.2.1 2011/07/08 16:45:51 dam Exp $
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
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

/*
 * Boards for AT86RF212
 */

//usb board w/ sub ghz trx rev 0
#define DERF_USB_13E00              (0x01)


/*
 * Boards for AT86RF231
 */

//usb board w/ 2.4Ghz trx rev 0
#define DERF_USB_23E00              (0x03)

#endif  /* #if defined(VENDOR_BOARDTYPES) && (VENDOR_BOARDTYPES != 0) */

#endif  /* PAL_BOARDTYPES_H */
/* EOF */
