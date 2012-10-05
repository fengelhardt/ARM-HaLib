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

#if (BOARD_TYPE==deRFmega128_22X00_deRFnode_2XXX2)

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

// necessary for deRFmega128-based boards - change pullups
#define TWI_PULLUP_ENABLE()   do {  \
    DDRD |= (1<<PD6);               \
    PORTD &= ~(1<<PD6);             \
} while(0);

#define TWI_PULLUP_DISABLE()  do {  \
    DDRD |= (1<<PD6);               \
    PORTD |= (1<<PD6);              \
} while(0);

/*
 * Interconnection between At25DF041@deRFnode/gateway and deRFmega128-xxx
 * SCK  = Pin13 = X1-11 = PB1
 * MOSI = Pin15 = X1-9  = PB2
 * MISO = Pin17 = X1-7  = PB3
 * #CS  = Pin35 = X2-12 = PE5
 */
#define DDR_SPI                         (DDRB)
#define SCK                             (PB1)
#define MOSI                            (PB2)
#define MISO                            (PB3)
#define DDR_CS                          (DDRE)
#define PORT_CS                         (PORTE)
#define CS                              (PE5)

/** Chip (slave) select */
#define CS_LO()                         (PORT_CS &= ~(1<<CS))
#define CS_HI()                         (PORT_CS |= (1<<CS))

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
    TWI_PULLUP_ENABLE();
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
    TWI_PULLUP_ENABLE();
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
    TWI_PULLUP_ENABLE();
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

#endif /* (BOARD_TYPE==deRFmega128_22X00_deRFnode_2XXX2) */

#endif /* BSP_CONFIG_H */
/* EOF */
