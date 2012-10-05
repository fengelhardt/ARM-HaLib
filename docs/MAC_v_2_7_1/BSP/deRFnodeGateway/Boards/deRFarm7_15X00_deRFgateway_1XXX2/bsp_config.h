/**
 * @file bsp_config.h
 *
 * @brief Board specific BSP implementation
 *
 * This header file defines functions to be called during the BSP Initialization.
 * It also defines board-specific extensions to the common BSP interface.
 *
 * $Id: bsp_config.h,v 1.1.4.1 2011/07/08 16:59:09 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#include "pal_boardtypes.h"

#if (BOARD_TYPE==deRFarm7_15X00_deRFgateway_1XXX2)

/* === INCLUDES ============================================================ */
#include "bsp_types.h"
#include "flash_interface.h"
#include "sensors_interface.h"
#include "vbatmon_interface.h"

/* === MACROS / DEFINES ==================================================== */
// available onboard-components
#define TWI_AVAILABE                   (1)
#define ACCELERATION_SENSOR_AVAILABLE  (1)
#define LUMINOSITY_SENSOR_AVAILABLE    (1)
#define TEMPERATURE_SENSOR_AVAILABLE   (1)
#define FLASH_AVAILABLE                (1)

// function binding for bsp_init()
#define BSP_TWI_INIT                   TWI_MasterInit
#define BSP_TEMPERATURE_SENSOR_INIT    TMP102_Init
#define BSP_ACC_SENSOR_INIT            BMA150_Init
#define BSP_LUMINOSITY_SENSOR_INIT     ISL29020_Init
#define BSP_FLASH_INIT                 AT25_Init

/// TWI clock frequency in Hz.
#define TWI_CLK                        100000

/// TWI pins definition.
#define PINS_TWI        {0x00000C00, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// BMA150 IRQ line
#if (! (defined REV00 | defined REV01) )
// REV02
#define PIN_BMA150IRQ   {1 << 25, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT}
#else
#define PIN_BMA150IRQ   {1 <<  4, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
#endif

/// VBAT monitoring Pin
#define PIN_VBATMON     {1 << 27, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO   {1 << 16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI   {1 << 17, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK   {1 << 18, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI0       PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0  {1 << 14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT25_SPI_BASE            AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the serialflash.
#define BOARD_AT25_SPI_ID              AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the serialflash.
#define BOARD_AT25_SPI_PINS            PINS_SPI0, PIN_SPI0_NPCS0
/// Chip select connected to the serialflash.
#define BOARD_AT25_NPCS                2

/* === TYPES =============================================================== */

/* === PROTOTYPES ========================================================== */

/* === INLINE FUNCTIONS ==================================================== */

/**
 * The following functions are just wrappers to the underlying BSP. They provide
 * basic access to the flash / sensors devices which allows for simple application
 * development. For full functionality, the xxx_interface.h provide a comprehensive
 * API which may be used alternately.
 */


/**
 * @brief Get the current temperature from the temperature sensor.
 * @param data where to put the read measurement
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_get_temperature(temperature_t* data)
{
    if(TWI_SUCC!=TMP102_StartOneshotMeasurement()) goto fail;
    if(TWI_SUCC!=TMP102_GetTemperature(data, true)) goto fail;

    return BSP_SUCCESS;

fail:
    data = NULL;
    return BSP_FAILURE;
}


/**
 * @brief Get the current luminosity from the luminosity sensor.
 * @param data where to put the read measurement
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_get_luminosity(luminosity_t* data)
{
    if(TWI_SUCC!=ISL29020_StartOneshotMeasurement()) goto fail;
    if(TWI_SUCC!=ISL29020_GetLuminosity(data)) goto fail;

    return BSP_SUCCESS;

fail:
    data = NULL;
    return BSP_FAILURE;
}


/**
 * @brief Get the current acceleration from the acceleration sensor.
 * @param data where to put the read measurement
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_get_acceleration(acceleration_t* data)
{
    if(TWI_SUCC!=BMA150_GetAcceleration(data)) goto fail;

    return BSP_SUCCESS;

fail:
    data = NULL;
    return BSP_FAILURE;
}


/**
 * @brief Enable or disable motion detection feature.
 * @param enabled whether feature should be switched on or off
 *        pCallback callback function to be invoked if motion was detected
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_switch_motion_detection(bool enabled, bma150_callback_t pCallback)
{
    if(enabled && (NULL == pCallback))
        return BSP_FAILURE;

    if(enabled) {
        BMA150_SetAnymotionIrqCallback(pCallback);
        BMA150_ConfigureAnymotionIrq(1);
    } else {
        BMA150_SetAnymotionIrqCallback(NULL);
        BMA150_ConfigureAnymotionIrq(0);
    }
    return BSP_SUCCESS;
}


/**
 * @brief Get the current battery voltage.
 * @param data where to put the read measurement [mV]
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_get_battery_voltage(uint16_t* data)
{
    Batmon_GetBatteryVoltage(data);
    return BSP_SUCCESS;
}


/**
 * @brief Erases the onboard flash device completely.
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_flash_erase(void)
{
    AT25_GlobalUnprotect();
    if(FL_SUCCESS == AT25_ChipErase())
        return BSP_SUCCESS;

    return BSP_FAILURE;
}


/**
 * @brief Writes up to 255 bytes to the onboard-flash device.
 * Requires the that the device has been erased previously.
 * Destination address is forced to be aligned to a page border.
 * @param   addr the page address
 *          pData points to the source data
 *          len length of source data
 * @return BSP_SUCCESS if successful, BSP_FAILURE otherwise
 */
static inline bsp_retval_t bsp_flash_write_page(uint32_t addr, uint8_t* pData, uint8_t len)
{
    if(FL_SUCCESS!=AT25_ProgramPage(1+(addr|0xFF), pData, len)) goto fail;
    return BSP_SUCCESS;

fail:
    return BSP_FAILURE;
}


/**
 * @brief Reads up to 255 bytes from the onboard-flash device.
 * Source address is forced to be aligned to a page border.
 * @param   addr the page address
 *          pData points to the destination data
 *          len length of source data
 */
static inline void bsp_flash_read_page(uint32_t addr, uint8_t* pData, uint8_t len)
{
    AT25_ReadArray(1+(addr|0xFF), pData, len);
}

#endif /* (BOARD_TYPE==deRFarm7_15X00_deRFgateway_1XXX2) */

#endif /* BSP_CONFIG_H */
/* EOF */
