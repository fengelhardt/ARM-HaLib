/**
 * @file bsp.c
 *
 * @brief Implementation of the BSP related common API
 *
 * This file contains the implementation of the common BSP API.
 *
 * $Id: bsp.c,v 1.1.4.2 2011/07/14 18:21:56 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */


/* === INCLUDES ============================================================ */
#include "bsp.h"                    // prototypes
#include "bsp_config.h"             // board specific

/* === MACROS / DEFINES ==================================================== */

/* === TYPES =============================================================== */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initialization of BSP
 *
 * This function initializes the BSP layer
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
bsp_retval_t bsp_init()
{
    /**
     * initialize the different components depending on their
     * availability on each board. Assume that each initialization
     * function returns with 0 in case of success.
     */

#if (TWI_AVAILABE == 1)
    if(0 != BSP_TWI_INIT()) goto fail;

#if (TEMPERATURE_SENSOR_AVAILABLE == 1)
    if(0 != BSP_TEMPERATURE_SENSOR_INIT()) goto fail;
#endif

#if (ACCELERATION_SENSOR_AVAILABLE == 1)
    if(0 != BSP_ACC_SENSOR_INIT()) goto fail;
#endif

#if (LUMINOSITY_SENSOR_AVAILABLE == 1)
    if(0 != BSP_LUMINOSITY_SENSOR_INIT()) goto fail;
#endif

#endif /* (TWI_AVAILABE == 1) */

#if (FLASH_AVAILABLE == 1)
    if(0 != BSP_FLASH_INIT()) goto fail;
#endif /* (FLASH_AVAILABLE == 1) */

    return BSP_SUCCESS;

fail:
    return BSP_FAILURE;
}


/**
 * @brief Things to do periodically
 *
 * This function performs things which are to do periodically. Currently it is
 * only a spaceholder.
 */
void bsp_task(void)
{

}


/* EOF */
