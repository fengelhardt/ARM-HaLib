/**
 * @file pal_usbd_callback.c
 *
 * Implements custom callback behaviour since the AT91Lib USB implementation
 * toggles LEDs which we probably do _not_ want except if debugging.
 *
 * $Id: pal_usbd_callbacks.c,v 1.1.4.1 2011/07/08 16:45:51 dam Exp $
 *
 * @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2011, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* === Includes ============================================================= */

#include <board.h>
#include <cmsis/core_cm3.h>
//#include "USBD_LEDs.h"
//#include "USBDCallbacks.h"
//#include "USBDDriverCallbacks.h"
#include <utility/led.h>
#include <utility/trace.h>

/* === Types ============================================================== */

/* === Globals ============================================================== */

/* === Prototypes =========================================================== */

/* === Implementation ======================================================= */

//------------------------------------------------------------------------------
// Callbacks re-implementation (from drivers/usbd)
//------------------------------------------------------------------------------

/**
 * Invoked after the USB driver has been initialized. By default, configures
 * the UDP/UDPHS interrupt.
 */
void USBDCallbacks_Initialized(void)
{
    NVIC_EnableIRQ(UDP_IRQn);
}


/**
 * Invoked when the USB device leaves the Suspended state. By default,
 * configures the LEDs.
 */
void USBDCallbacks_Resumed(void)
{
#ifdef USB_DBG
    /* Initialize LEDs */
    LED_Configure(USBD_LEDPOWER);
    LED_Set(USBD_LEDPOWER);
    LED_Configure(USBD_LEDUSB);
    LED_Clear(USBD_LEDUSB);
#endif
}


/**
 * Invoked when the USB device gets suspended. By default, turns off all LEDs.
 */
void USBDCallbacks_Suspended(void)
{
#ifdef USB_DBG
    /* Turn off LEDs */
    LED_Clear(USBD_LEDPOWER);
    LED_Clear(USBD_LEDUSB);
#endif
}


//------------------------------------------------------------------------------
// Callbacks re-implementation (from usb/device/usbd)
//------------------------------------------------------------------------------


/**
 * Invoked when the USB driver is reset. Does nothing by default.
 */
void USBDCallbacks_Reset(void)
{
    /* Does nothing*/
}


/**
 * Indicates that the current configuration of the device has changed.
 * \param cfgnum  New device configuration index.
 */
void USBDDriverCallbacks_ConfigurationChanged(unsigned char cfgnum)
{
    TRACE_INFO_WP("ConfigurationChanged ");
}


/**
 * Notifies of a change in the currently active setting of an interface.
 * \param interface  Number of the interface whose setting has changed.
 * \param setting  New interface setting.
 */
void USBDDriverCallbacks_InterfaceSettingChanged(
    unsigned char interface,
    unsigned char setting)
{
    TRACE_INFO_WP("InterfaceSettingChanged ");
}

