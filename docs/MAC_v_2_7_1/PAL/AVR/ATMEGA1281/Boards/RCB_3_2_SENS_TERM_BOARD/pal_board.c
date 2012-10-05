/**
 * @file PAL/AVR/ATMEGA1281/Boards/RCB_3_2_SENS_TERM_BOARD/pal_board.c
 *
 * @brief Board specific functions for RCB_3_2_SENS_TERM_BOARD with ATmega1281 and AT86RF230B
 *
 * $Id: pal_board.c 25827 2011-03-03 11:28:18Z felix.varghese $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"
/*
 * 'sys_time' is a entity of timer module which is given to other modules
 * through the interface file below. 'sys_time' is required to obtain the
 * frame timestamp
 */
#include "pal_timer.h"

#if (BOARD_TYPE == RCB_3_2_SENS_TERM_BOARD)

/**
 * \addtogroup grpPal_AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD
 * @{
 */

/* === Globals ============================================================== */

static uint8_t led_state = 0x00;

/* === Prototypes ========================================================== */


static void led_helper_func(void);

/* === Implementation ======================================================= */

/**
 * @brief Provides timestamp of the last received frame
 *
 * This function provides the timestamp (in microseconds)
 * of the last received frame.
 *
 * @param[out] timestamp Timestamp in microseconds
 */
void pal_trx_read_timestamp(uint32_t *timestamp)
{
    /*
     * Everytime a transceiver interrupt is triggred, input capture register of
     * the AVR is latched. The 'sys_time' is concatenated to the ICR to
     * generate the time stamp of the received frame.
     * 'sys_time'   'ICR'
     *  ---------|--------- => 32 bit timestamp
     *   16 bits   16 bits
     */
    *timestamp = (uint32_t)sys_time << (uint32_t)16;
    *timestamp |= (uint32_t)TIME_STAMP_REGISTER;
}



/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator, based
 * on the external 32.768 Hz crystal clock source.
 *
 * @return True if calibration is successful, false otherwise.
 */
bool pal_calibrate_rc_osc(void)
{
    /*
     * Use the 32.768 kHz external crystal oscillator as reference clock source.
     */
    /*
     * This is the actual value of the timer to be calibrated
     * after each calibration run.
     */
    uint16_t curr_value_cal_timer = 0;

    /* This is the best OSCCAL value. */
    uint8_t best_value_osccal = 0;
    /*
     * This is the smallest difference between the target value and the actual value
     * of the timer the timer to be calibrated.
     */

    uint16_t best_dif_timers = ~(uint16_t)0;
    /*
     * This is the difference between  between the target value and the actual value
     * of the timer the timer to be calibrated of the current calibration attempt.
     */
    uint16_t curr_dif_timers = 0;

    uint16_t counter;
    uint8_t tccr2b;
    uint8_t tccr1b;
    uint8_t tccr1a;
    bool cal_result = true;

    ENTER_CRITICAL_REGION();

    /*
     * Save current values of timer status.
     */
    tccr2b = TCCR2B;
    tccr1b = TCCR1B;
    tccr1a = TCCR1A;

    /*
     * Stop timers 1 and 2.
     * Set timer 1 to normal mode (no CTC, no PWM, just count).
     */
    TCCR2B = 0;
    TCCR1B = 0;
    TCCR1A = 0;

    for (counter = 0; counter < MAX_CAL_LOOP_CNT;  counter++)
    {
        /*
         * Delete pending timer 1 and 2 interrupts, and clear the
         * counters.
         */
        TIFR1 = 0xFF;
        TIFR2 = 0xFF;
        TCNT2 = 0;
        TCNT1 = 0;

        /* Kick timer1 with internal clock source and no prescaler */
        TCCR1B = (1 << CS10);

        /*
         * Kick timer2 with external 32.768 Hz asynchronous clock
         * and no prescaler
         */
        TCCR2B = (1 << CS20);
        ASSR = (1 << AS2);

        /*
         * Wait for timer 2 to overflow.
         */
        while (!(TIFR2 & (1 << TOV2)))
        {
            /* Wait */
        }


        /*
         * Stop timer 1.  Now, TCNT1 contains the number of CPU cycles
         * counted during F_CPU / (32 * 256) cycles.
         */
        TCCR1B = 0;
        TCCR2B = 0;

        curr_value_cal_timer = TCNT1;

        /*
         * Check if reference timer is running at all,
         * i.e. the reference clock is available.
         */
        if (0 == curr_value_cal_timer)
        {
            /* Reference timer is not running, return error. */
            cal_result = false;
            break;
        }

        if (curr_value_cal_timer <= (uint16_t)(TARGETVAL_CALIBRATION))
        {
            curr_dif_timers = (uint16_t)(TARGETVAL_CALIBRATION) - curr_value_cal_timer;
        }
        else
        {
            curr_dif_timers = curr_value_cal_timer - (uint16_t)(TARGETVAL_CALIBRATION);
        }

        if (curr_dif_timers < best_dif_timers)
        {
            best_dif_timers = curr_dif_timers;
            best_value_osccal = OSCCAL;
        }

        if (curr_value_cal_timer <= (uint16_t)(TARGETVAL_CALIBRATION))
        {
            /* Too slow, increase speed */
            OSCCAL++;
        }
        else
        {
            /* Too fast, lower speed */
            OSCCAL--;
        }
    }

    TCCR2B = tccr2b;
    TCCR1B = tccr1b;
    TCCR1A = tccr1a;
    LEAVE_CRITICAL_REGION();

    OSCCAL = best_value_osccal;

    return (cal_result);
}



/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the port pins used to connect
 * the microcontroller to transceiver.
 */
void gpio_init(void)
{
    /* The following pins are output pins.  */
    TRX_PORT1_DDR |= _BV(SEL);
    TRX_PORT1_DDR |= _BV(SCK);
    TRX_PORT1_DDR |= _BV(RST);
    TRX_PORT1_DDR |= _BV(MOSI);
    TRX_PORT1_DDR |= _BV(SLP_TR);

    /* The following pins are input pins.  */
    TRX_PORT1_DDR &= ~_BV(MISO);
}



/**
 * This function is called by timer_init() to perform the non-generic portion
 * of the initialization of the timer module.
 */
void timer_init_non_generic(void)
{
    /* Select proper clock as timer clock source when radio is sleeping */
    TIMER_SRC_DURING_TRX_SLEEP();

    /*
     * Clear pending output compare matches of all the OCR1A, OCR1B and
     * OCR1C.
     */
    TIFR1 = _BV(OCF1A);
    TIFR1 = _BV(OCF1B);
    TIFR1 = _BV(OCF1C);

    /* Enable the timer overflow interrupt */
    TIMSK1 |= _BV(TOIE1);
}


/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    led_state = 0x00;

    /* Switch LEDs off. Both LEDs are low active. */
    LED_PORT |= (1 << LED_BIT_0);
    LED_PORT |= (1 << LED_BIT_1);

    led_helper_func();
}



/**
 * @brief Control LED status
 *
 * @param[in]  led_no LED ID
 * @param[in]  led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    uint8_t pin;
    /* New values of LED pins based on new LED state. */
    uint8_t led_pin_value;
    /*
     * Original value of LED port before writing new value.
     * This value needs to be restored.
     */
    uint8_t orig_led_port = LED_PORT & ~LED_BIT_MASK;

    /* Both LEDs need to be updated, since several peripherals
     * are dealing with the same port for this board
     * (USB, EEPROM, LEDs, Button).
     */
    LED_PORT_DIR |= (1 << LED_BIT_0);
    LED_PORT_DIR |= (1 << LED_BIT_1);

    switch (led_no)
    {
        default:
        case LED_0:
            pin = LED_BIT_0;
            break;
        case LED_1:
            pin = LED_BIT_1;
            break;
    }

    switch (led_setting)
    {
        case LED_ON:
            led_state |= _BV(pin);
            break;

        case LED_OFF:
            led_state &= ~_BV(pin);
            break;

        case LED_TOGGLE:
        default:
            if (led_state & _BV(pin))
            {
                /*
                 * LED is currently on,
                 * Switch it off
                 */
                led_state &= ~_BV(pin);
            }
            else
            {
                /*
                 * LED is currently off,
                 * Switch it on
                 */
                led_state |= _BV(pin);
            }
            break;
    }

    led_pin_value = (uint8_t)(~(uint16_t)led_state);  // Implicit casting required to avoid IAR Pa091.
    led_pin_value &= LED_BIT_MASK;

    LED_PORT = orig_led_port | led_pin_value;

    led_helper_func();
}


/**
 * @brief Helper function for LEDs
 */
static void led_helper_func(void)
{
    /*
     * Enable LED address decoding.
     * This is similar to USB, but with other settings.
     */
    LED_ADDR_DEC_PORT |= _BV(6);    // Different to USB
    LED_ADDR_DEC_DDR |= _BV(6);
    LED_ADDR_DEC_PORT &= ~_BV(7);
    LED_ADDR_DEC_DDR |= _BV(7);

    /* Enable desired LED state. */
    DDRG |= _BV(0);
    /* Set PE4 to 0 and back to 1. */
    PORTG &= ~_BV(0);
    PORTG |= _BV(0);

    /*
     * Disable LED address decoding.
     * This enables USB again.
     */
    LED_ADDR_DEC_PORT &= ~_BV(6);
    LED_ADDR_DEC_DDR |= _BV(6);
    LED_ADDR_DEC_PORT &= ~_BV(7);
    LED_ADDR_DEC_DDR |= _BV(7);

    /* Set the pins back to input */
    LED_PORT_DIR &= ~(LED_BIT_MASK);
}


/**
 * @brief Initialize the button
 */
void pal_button_init(void)
{
    /* Nothing to do here. */
}



/**
 * @brief Read button
 *
 * @param button_no Button ID
 */
button_state_t pal_button_read(button_id_t button_no)
{
    uint8_t cur_button_state;

    /* Keep compiler happy. */
    button_no = button_no;

    /*
     * Enable button address decoding.
     * This is similar to USB, but with other settings.
     */
    BUTTON_ADDR_DEC_PORT |= _BV(6);    // Different to USB
    BUTTON_ADDR_DEC_DDR |= _BV(6);
    BUTTON_ADDR_DEC_PORT &= ~_BV(7);
    BUTTON_ADDR_DEC_DDR |= _BV(7);


    PORTG &= ~_BV(1);
    DDRG |= _BV(1);

    /* Switch port to input. */
    BUTTON_PORT |= (1 << BUTTON_PIN_0);
    BUTTON_PORT_DIR &= ~(1 << BUTTON_PIN_0);

    cur_button_state = BUTTON_INPUT_PINS;

    PORTG |= _BV(1);

    /* Switch port back to output. */
    BUTTON_PORT_DIR |= (1 << BUTTON_PIN_0);

    /*
     * Disable button address decoding.
     * This enables USB again.
     */
    BUTTON_ADDR_DEC_PORT &= ~_BV(6);
    BUTTON_ADDR_DEC_DDR |= _BV(6);
    BUTTON_ADDR_DEC_PORT &= ~_BV(7);
    BUTTON_ADDR_DEC_DDR |= _BV(7);

    if (cur_button_state & 0x01)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}



/**
 * @brief Generation of random seed for function srand() in case this
 *        is not supported by the transceiver (e.g. AT86RF230)
 *
 * @return uint16_t Random number to be used as seed for function srand()
 * @ingroup apiPalApi
 */
uint16_t pal_generate_rand_seed(void)
{
    /* Todo */
    return (0);
}

/** @} */

#endif /* RCB_3_2_SENS_TERM_BOARD */

/* EOF */
