/**
 * @file pwr_mgmt.h
 *
 * header file for pwr_mgmt.c
 *
 * @author    de: http://www.dresden-elektronik.de
 * @author    Support email: wireless@dresden-elektronik.de
 *
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 */


/* Prevent double inclusion */
#ifndef PWR_MGMT_H
#define PWR_MGMT_H

/* === INCLUDES ============================================================ */

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/**
 * @brief disable unnecessary peripherals
 */
void configure_power_saving(void);

/**
 * @brief configures the system for an energy-friendly state
 * Some peripherals might become inaccessible if invoked
 */
void low_power_mode(void);

/**
 * @brief configures the system for an normal mode where all peripherals
 * are accessible. Inverse function for 'low_power_mode()'.
 */
void normal_power_mode(void);

/**
 * @brief prescales the system clock if necessary.
 * Used before invoking 'system_suspend()'.
 */
void system_clock_down(void);

/**
 * @brief suspend the system by disabling the MCU clock
 */
void system_suspend(void);

/**
 * @brief things to do after the system woke up, i.e. revert clock prescaler
 */
void system_wakeup(void);



/* === IMPLEMENTATION ====================================================== */

#endif

/* EOF */
