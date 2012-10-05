/**
 * @file bsp.h
 *
 * @brief Defines the BSP related common API
 *
 * This header file declares prototypes of the BSP API.
 *
 * $Id: bsp.h,v 1.1.4.1 2011/07/08 16:59:08 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef BSP_H
#define BSP_H

/* === INCLUDES ============================================================ */
#include "pal_types.h"      // BOARD_TYPE definition
#include "bsp_types.h"      // return codes
#include "bsp_config.h"     // auto-include board-specific inline functions

/* === MACROS / DEFINES ==================================================== */

/* === TYPES =============================================================== */

/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialization of BSP
 *
 * This function initializes the BSP layer.
 *
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
bsp_retval_t bsp_init(void);


/**
 * @brief Things to do periodically
 *
 * This function performs things which are to do periodically. Currently it is
 * only a spaceholder.
 */
void bsp_task(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* === INLINE FUNCTIONS ==================================================== */


#endif /* BSP_H */

/* EOF */
