/**
 * @file pwr_mgmt.c
 *
 * @brief provides functions for power management
 *
 * @author    de: http://www.dresden-elektronik.de
 * @author    Support email: wireless@dresden-elektronik.de
 *
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 */


/* === INCLUDES ============================================================ */
#include "pal.h"

#include "pwr_mgmt.h"
#include "usb.h"            // portpin definitions

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */
uint8_t ddrb, ddrd, ddre, ddrf, ddrg;
uint8_t portb, portd, porte, portf, portg;

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief disable unnecessary peripherals
 */
void configure_power_saving(void)
{
    // power reduction register manually

    // PRR0 controls enabled timers and peripherals
    PRR0 = (1<<PRTIM0) | (1<<PRTIM1) | (1<<PRUSART0);

    // PRR1 controls the power chain and which timers will be enabled
    PRR1 = (1<<PRTIM4) | (1<<PRTIM3) | (1<<PRUSART1);

    // PRR2 controls (de-)activation of SRAM blocks -> skip this
}


/**
 * @brief configures the system for an energy-friendly state
 * Some peripherals might become inaccessible if invoked
 */
void low_power_mode(void)
{
    // switch LEDs off
    pal_led(LED_0, LED_OFF);
    pal_led(LED_1, LED_OFF);
    pal_led(LED_2, LED_OFF);

    // remember previous state on GPIO lines
    ddrb = DDRB; portb = PORTB;
    ddrd = DDRD; portd = PORTD;
    ddre = DDRE; porte = PORTE;
    ddrf = DDRF; portf = PORTF;
    ddrg = DDRG; portg = PORTG;

    // switch GPIOs to neutral level
    // switch to tristate (Hi-Z), Pullups _globally_ disabled
    DDRB = 0x80; PORTB = 0x7F;
    DDRD = 0x00; PORTD = 0xFF;
    DDRE = 0x00; PORTE = 0xFF;
    DDRF = 0x00; PORTF = 0xFF;
    DDRG = 0x00; PORTG = 0xFF;
    MCUCR |= _BV(PUD);
    _delay_us(500); // necessary until tristate condition really set

    // disable deRFmega128 on-module flash, put PD6 to VCC (works since REV01)
    DDRD |= _BV(PD6);
    PORTD |= _BV(PD6);

    // disable octal driver energizing FTDI
    // put one of RD/WR H -> prevent octal driver /CE
    // better to take RD since WR also toggles driver direction
    USB_RD_DDR  |= _BV(USB_RD_PIN);
    USB_RD_PORT |= _BV(USB_RD_PIN);

    // disable /CS for on-board flash, put Pin35 (PE5) H
    DDRE |= _BV(PE5); PORTE |= _BV(PE5);
}


/**
 * @brief configures the system for an normal mode where all peripherals
 * are accessible. Inverse function for 'low_power_mode()'.
 */
void normal_power_mode(void)
{
    // disable PUD
    MCUCR &= ~_BV(PUD);

    // restore the previous state on all GPIO lines
    DDRB = ddrb; PORTB = portb;
    DDRD = ddrd; PORTD = portd;
    DDRE = ddre; PORTE = porte;
    DDRF = ddrf; PORTF = portf;
    DDRG = ddrg; PORTG = portg;
}


/**
 * @brief prescales the system clock
 * Used before invoking 'system_suspend()'.
 */
void system_clock_down(void)
{
    // set clock prescaler, not mandatory
    clock_prescale_set(0x08);
}


/**
 * @brief suspend the system by disabling the MCU clock
 */
void system_suspend(void)
{
    // go into power save mode
    SMCR = (1<<SE) | (1<<SM1) | (1<<SM0);

    // execute sleep instruction
    sleep_cpu();

    // woke up - disable sleep
    SMCR = 0;
}


/**
 * @brief things to do after the system woke up, i.e. revert clock prescaler
 */
void system_wakeup(void)
{
    // nothing to do

    // revert prescaler
    clock_prescale_set(0x01);
}


/* EOF */
