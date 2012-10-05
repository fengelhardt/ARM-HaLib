/**
 * @file pal_config.h
 *
 * @brief PAL configuration for AT91SAM3S4B
 *
 * This header file contains configuration parameters for AT91SAM3S4B.
 *
 * $Id: pal_config.h,v 1.1.4.1 2011/07/08 16:45:51 dam Exp $
 *
 *  @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2011, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes ============================================================ */
#include "pal_boardtypes.h"

#if (BOARD_TYPE == deRFusb_13E06)
#include "AT91SAM3S4.h"
#include "armtypes.h"
#include "core_cm3.h"

#include "board.h"
#include "pal_internal.h"
#include "return_val.h"

/* === Types =============================================================== */

/* Enumerations used to identify LEDs */
typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

#define NO_OF_LEDS                      (3)

/* Enumerations used to identify buttons */
typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

#define NO_OF_BUTTONS                   (0)

/* === Externals =========================================================== */

/*
 * External GPIO Pin references.
 * Respective variables are declared within pal_board.c
 */
extern const pin_t pin_radio_irq;
extern const pin_t pin_radio_rx_ts;
extern const pin_t rst[];
extern const pin_t slp_tr[];
extern const pin_t sel[];
extern const pin_t led_pins[];

/* === Macros ============================================================== */

/* The CPU clock */
#define F_CPU                           (BOARD_MCK)

/**
 * PLLA and PLLB multiplier and divider,
 * clock derivation for Master clock and USB clock
 * generally: CLK = XTAL_F*((MUL+1)/DIV)/prescaler
 */
#if (F_CPU == 32000000UL)

/* PLLA multiplier and divider to generate a 96 MHz clock (used for USB) */
#define PLLA_MULTIPLIER                 (124)
#define PLLA_DIVIDER                    (24)

/* PLLB multiplier and divider to generate a 64 MHz clock (used for MCK) */
#define PLLB_MULTIPLIER                 (124)
#define PLLB_DIVIDER                    (36)

/* generate MCK from PLLB with prescaler 2 */
#define BOARD_MCKR               (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLB_CLK)

/* USB clock source selection : PLLA, Prescaler 2 */
#define BOARD_USB_CLK            (PMC_USB_USBDIV & (1<<8)) | (PMC_USB_USBS & 0)

/* SPI baud rate divider to generate 4MHz SPI clock. */
#define SPI_BR_DIVIDER                  (8)

/* Timer prescaler, Timer clock is generated from MCK */
#define TC_PRESCALER                    (32)              // has to be 2, 8, 32 or 128


#elif (F_CPU == 48000000UL)

/* PLLA multiplier and divider to generate a 96 MHz clock (used for USB) */
#define PLLA_MULTIPLIER                 (124)
#define PLLA_DIVIDER                    (24)

/* PLLB multiplier and divider to generate a 96 MHz clock (used for MCK) */
#define PLLB_MULTIPLIER                 (124)
#define PLLB_DIVIDER                    (24)

/* generate MCK from PLLB with prescaler 2 */
#define BOARD_MCKR               (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLB_CLK)

/* USB clock source selection : PLLA, Prescaler 2 */
#define BOARD_USB_CLK            (PMC_USB_USBDIV & (1<<8)) | (PMC_USB_USBS & 0)

/* SPI baud rate divider to generate 4MHz SPI clock. */
#define SPI_BR_DIVIDER                  (12)

/* Timer prescaler, Timer clock is generated from MCK */
#define TC_PRESCALER                    (32)              // has to be 2, 8, 32 or 128


#else
#error "unsupported CPU clock"
#endif


/*
 * SPI Base Register
 */
#define SPI_USED                        (BOARD_TRX_SPI_BASE)


#define PERIPHERAL_CLOCK_ENABLE(id)     (pal_peripheral_clock_enable(id))

/*
 * IRQ macros for AT91SAM3S4B
 */

/* Enables the transceiver interrupts. */
#define ENABLE_TRX_IRQ()                (pal_pio_enable_interrupt(&(const pin_t)TRX_INT_PIN))
#define ENABLE_TRX_IRQ_TSTAMP()         (pal_pio_enable_interrupt(&(const pin_t)TRX_RXTS_PIN))

/* Disables the transceiver interrupts. */
#define DISABLE_TRX_IRQ()               (pal_pio_disable_interrupt(&(const pin_t)TRX_INT_PIN))
#define DISABLE_TRX_IRQ_TSTAMP()        (pal_pio_disable_interrupt(&(const pin_t)TRX_RXTS_PIN))

/* Clears the transceiver interrupts. */
#define CLEAR_TRX_IRQ()                 (NVIC_ClearPendingIRQ((IRQn_Type)pin_radio_irq.id))
#define CLEAR_TRX_IRQ_TSTAMP()          (NVIC_ClearPendingIRQ((IRQn_Type)pin_radio_rx_ts.id))


/*
 * Macros dealing with the global interrupt (IRQ bit) of AT91SAM3S4B.
 */

/* Enables the global interrupt. */
#define ENABLE_GLOBAL_IRQ()             sei()


/* Disables the global interrupt. */
#define DISABLE_GLOBAL_IRQ()            cli()


/*
 * This macro is used to mark the start of a critical region. It saves the
 * current status register and then disables the interrupt.
 */
#define ENTER_CRITICAL_REGION()         \
        {uint32_t sreg; GET_CPSR(sreg); DISABLE_GLOBAL_IRQ()

/*
 * This macro is used to mark the end of a critical region. It restores the
 * current status register.
 */
#define LEAVE_CRITICAL_REGION()         SET_CPSR(sreg);}

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()              \
        { uint32_t irq_mask = __get_PRIMASK(); DISABLE_TRX_IRQ()

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()              __set_PRIMASK(irq_mask); }


/*
 * GPIO macros for AT91SAM3S4B
 */

/*
 * This board uses an SPI-attached transceiver.
 */
#define PAL_USE_SPI_TRX                 (1)

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      (rst->pio->PIO_SODR = rst->mask)
#define RST_LOW()                       (rst->pio->PIO_CODR = rst->mask)
#define SLP_TR_HIGH()                   (slp_tr->pio->PIO_SODR = slp_tr->mask)
#define SLP_TR_LOW()                    (slp_tr->pio->PIO_CODR = slp_tr->mask)


/*
 * Timer macros for AT91SAM3S4B
 */

/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonably implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 *
 * AT91SAM3S4B @ FPU
 */
#if (F_CPU == 32000000UL)
/* 32 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 32 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
#elif (F_CPU == 48000000UL)
/* 48 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 48 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop(); nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
#endif

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/*
 * Settings to give clocking to timer when radio is awake
 *
 */
#define TIMER_SRC_DURING_TRX_AWAKE()

/*
 * Settings to give clocking to timer when radio is sleeping
 *
 */
#define TIMER_SRC_DURING_TRX_SLEEP()

/*
 * Maximum numbers of software timers running at a time
 */
#define MAX_NO_OF_TIMERS                (25)
#if (MAX_NO_OF_TIMERS > 255)
#error "MAX_NO_OF_TIMERS must not be larger than 255"
#endif

/*
 * Hardware register that holds Rx timestamp
 */

/* If this macro is defined then the upper 16-bit of the timestamp value
is taken from sys_time which is a SW count incremented whenever lower 16bit
overflow happens */
#define TIMESTAMP_UPPER_16BIT_SW        (1)

/*
 * Hardware register that holds Rx timestamp
 */
#ifdef TIMESTAMP_UPPER_16BIT_SW
#define TIME_STAMP_LOW_REGISTER         (PAL_TIMER_CH.TC_CV)
#define TIMER_LOW_REGISTER              (PAL_TIMER_CH.TC_CV)
#else
#define TIME_STAMP_LOW_REGISTER         (TC0->TC_CHANNEL[0].TC_CV)
#define TIMER_LOW_REGISTER              (TC0->TC_CHANNEL[0].TC_CV)
#define TIME_STAMP_HIGH_REGISTER        (TC0->TC_CHANNEL[1].TC_CV)
#endif


/* Macros for PAL Normal Timer Register */
#define PAL_TIMER_CH                    (TC0->TC_CHANNEL[2])
#define PAL_TIMER_IRQ_ID                (TC2_IRQn)
#define PAL_TIMER_PERIPH_ID             (ID_TC2)
#define PAL_TIMER_CCR_EN                (TC_CCR0_CLKEN)
#define PAL_TIMER_IER_FLAG              (TC_IER0_CPCS)
#define PAL_TIMER_IDR_FLAG              (TC_IDR0_CPCS)
#define PAL_TIMER_SR_FLAG               (TC_SR0_CPCS)
#define PAL_TIMER_SR_OVERFLOW_FLAG      (TC_SR0_COVFS)
#define PAL_TIMER_STATUS_REG            (PAL_TIMER_CH.TC_SR)
#define PAL_TIMER_IER                   (PAL_TIMER_CH.TC_IER)
#define PAL_TIMER_IDR                   (PAL_TIMER_CH.TC_IDR)
#define PAL_TIMER_COMP_REG              (PAL_TIMER_CH.TC_RC)
#define PAL_TIMER_CCR                   (PAL_TIMER_CH.TC_CCR)
#define PAL_TIMER_IRQ                    void TC2_IrqHandler(void)


/* Macros for PAL High Priority Timer Register */
#define PAL_HIGHPRI_TMR_CH              (TC1->TC_CHANNEL[1])
#define PAL_HIGHPRIO_TIMER_IRQ_ID       (TC4_IRQn)
#define PAL_HIGHPRIO_TIMER_PERIPH_ID    (ID_TC4)
#define PAL_HIGHPRIO_TIMER_CCR_EN       (TC_CCR1_CLKEN)
#define PAL_HIGHPRIO_TIMER_IER_FLAG     (TC_IER1_CPCS)
#define PAL_HIGHPRIO_TIMER_IDR_FLAG     (TC_IDR1_CPCS)
#define PAL_HIGHPRIO_TIMER_STATUS_REG   (PAL_HIGHPRI_TMR_CH.TC_SR)
#define PAL_HIGHPRIO_TIMER_IER          (PAL_HIGHPRI_TMR_CH.TC_IER)
#define PAL_HIGHPRIO_TIMER_IDR          (PAL_HIGHPRI_TMR_CH.TC_IDR)
#define PAL_HIGHPRIO_TIMER_COMP_REG     (PAL_HIGHPRI_TMR_CH.TC_RC)
#define PAL_HIGHPRIO_TIMER_CCR          (PAL_HIGHPRI_TMR_CH.TC_CCR)
#define PAL_HIGHPRIO_TIMER_IRQ           void TC4_IrqHandler(void)


/*
 * TRX Access macros for AT91SAM3S4B
 */

/*
 * Position of the PCS (peripheral chip select) field in the SPI_MR register.
 */
#define PCS_FIELD_IN_MR                 (16)

/*
 * Value that needs to be written to in the PCS field of the SPI_MR to
 * activate the line to which the trx select line is connected.
 */
#define PCS_FIELD_VALUE                 (3)

/*
 * Value of PCS field in SPI_MR (mode register) which indicates the contoller
 * about the line to which the slave is connected.
 */
#define SS_ENABLE                       (PCS_FIELD_VALUE << PCS_FIELD_IN_MR)

/*
 * Slave select made low
 */
#define SS_LOW()                        (sel->pio->PIO_CODR = sel->mask)

/*
 * Slave select made high
 */
#define SS_HIGH()                       (sel->pio->PIO_SODR = sel->mask)

/*
 * Dummy value to be written in SPI transmit register to retrieve data form it
 */
#define SPI_DUMMY_VALUE                 (0x00)

/* Reads the data from the SPI receive register. */
#define SPI_READ(register_value)    do {         \
    while ((SPI->SPI_SR & SPI_SR_RDRF) == 0);    \
    register_value = (SPI->SPI_RDR & 0xFFFF);    \
} while (0);

/* Writes the data into the SPI transmit register. */
#define SPI_WRITE(data)     do {                 \
    while ((SPI->SPI_SR & SPI_SR_TDRE) == 0);    \
    SPI->SPI_TDR = data & 0xFFFF;                \
    while ((SPI->SPI_SR & SPI_SR_TXEMPTY) == 0); \
} while (0);


/* this board has no external EEPROM */
#define EXTERN_EEPROM_AVAILABLE         (0)

/*
 * Value of an external PA gain
 * If no external PA is available, value is 0.
 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Alert initialization
 */
#define ALERT_INIT()        do                                   \
{                                                                \
    pal_pio_configure(led_pins, 3);                              \
    led_pins[0].pio->PIO_SODR = led_pins[0].mask;                \
    led_pins[1].pio->PIO_SODR = led_pins[1].mask;                \
    led_pins[2].pio->PIO_SODR = led_pins[2].mask;                \
                                                                 \
} while (0);


/*
 * Alert indication
 */
#define ALERT_INDICATE()    do                                   \
{                                                                \
    if ((led_pins[0].pio->PIO_ODSR & led_pins[0].mask) != 0)     \
       led_pins[0].pio->PIO_CODR = led_pins[0].mask;             \
    else                                                         \
       led_pins[0].pio->PIO_SODR = led_pins[0].mask;             \
                                                                 \
    if ((led_pins[1].pio->PIO_ODSR & led_pins[1].mask) != 0)     \
       led_pins[1].pio->PIO_CODR = led_pins[1].mask;             \
    else                                                         \
       led_pins[1].pio->PIO_SODR = led_pins[1].mask;             \
                                                                 \
    if ((led_pins[2].pio->PIO_ODSR & led_pins[2].mask) != 0)     \
       led_pins[2].pio->PIO_CODR = led_pins[2].mask;             \
    else                                                         \
       led_pins[2].pio->PIO_SODR = led_pins[2].mask;             \
} while (0);

/*
 * ADC Initialization values
 */
/*
 * Number of ADC conversions to be done during generation of random number.
 * Since a 16-bit value is needed and 4 ADC channels are used
 * to get 1 single bit (always the LSB), 4 subsequent conversions are required.
 */
#define BOARD_ADC_FREQ                  (6000000)

#define NO_OF_CONVERSIONS               (16)

/* Value of the ADC clock in Hz */
#define USED_ADC_CLOCK_FREQ             (6000000)
/* Value of MCK in Hz */
#define USED_MCK_CLOCK_FREQ             (F_CPU)

#define ADC_STARTUP_TIME_NS             (10)        /* In us */
#define ADC_SAMPLE_AND_HOLD_TIME_US     (1200)      /* In ns */

/* Defines for the ADC Mode register */
#define ADC_PRESCAL     ((USED_MCK_CLOCK_FREQ / (2 * USED_ADC_CLOCK_FREQ)) - 1)
#define ADC_STARTUP     (((USED_ADC_CLOCK_FREQ / 1000000) * ADC_STARTUP_TIME_NS / 8) - 1)
#define ADC_SHTIM       ((((USED_ADC_CLOCK_FREQ / 1000000) * ADC_SAMPLE_AND_HOLD_TIME_US) / 1000) - 1)

#define ADC_NUM_1                       (4)


/* === Prototypes ========================================================== */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if (BOARD_TYPE == deRFusb_13E06) */

#endif /* PAL_CONFIG_H */

/* EOF */
