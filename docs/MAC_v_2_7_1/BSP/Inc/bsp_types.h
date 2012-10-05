/**
 * @file bsp_types.h
 *
 * @brief Return values of common BSP API
 *
 * This header file defines BSP return codes.
 *
 * $Id: bsp_types.h,v 1.1.4.1 2011/07/08 16:59:08 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef BSP_TYPES_H
#define BSP_TYPES_H

/* === INCLUDES ============================================================ */
#include "pal_types.h"      // platform-dependent SHORTENUM definition

/* === MACROS / DEFINES ==================================================== */

/* === TYPES =============================================================== */

typedef enum bsp_retval_tag
{
    BSP_SUCCESS = 0x00,
    BSP_FAILURE

} SHORTENUM bsp_retval_t;

/* === PROTOTYPES ========================================================== */

/* === INLINE FUNCTIONS ==================================================== */


#endif /* BSP_TYPES_H */
/* EOF */
