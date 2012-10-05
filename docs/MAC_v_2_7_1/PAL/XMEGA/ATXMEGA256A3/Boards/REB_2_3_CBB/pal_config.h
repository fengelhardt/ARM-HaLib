/**
 * @file pal_config.h
 *
 * @brief PAL configuration for ATxmega256A3
 *
 * This header file contains configuration parameters for ATxmega256A3.
 *
 * $Id: pal_config.h 27631 2011-07-07 16:56:47Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes =============================================================*/

#include "pal_boardtypes.h"

#if (BOARD_TYPE == REB_2_3_CBB)

/*
 * This header file is required since a function with
 * return type retval_t is declared
 */
#include "return_val.h"

/* === Types ================================================================*/

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

#define NO_OF_BUTTONS                   (1)

/* === Externals ============================================================*/


/* === Macros ===============================================================*/

/** The default CPU clock */
#ifndef F_CPU
#define F_CPU                   (16000000UL)
#endif

/* Currently only the following system clock frequencies are supported */
#if ((F_CPU != (32000000UL)) && (F_CPU != (16000000UL)) && (F_CPU != (8000000UL)) && (F_CPU != (4000000UL)))
#error "Unsupported F_CPU value"
#endif

/**
 * This board does not support antenna diversity.
 */
#define ANTENNA_DIVERSITY               (0)

/**
 * This board supports CW transmission.
 */
#define CW_SUPPORTED                    (1)

/*
 * IRQ macros for ATxmega256A3
 */

/* Mapping of TRX interrupts to ISR vectors */
#define TRX_MAIN_ISR_VECTOR             (PORTC_INT0_vect)


/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()                (PORTC.INTCTRL |= PORT_INT0LVL_gm)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()               (PORTC.INTCTRL &= ~PORT_INT0LVL_gm)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()                 (PORTC.INTFLAGS = PORT_INT0IF_bm)


/** Enables the global interrupts. */
#define ENABLE_GLOBAL_IRQ()             sei()

/** Disables the global interrupts. */
#define DISABLE_GLOBAL_IRQ()            cli()

/*
 * This macro saves the global interrupt status
 */
#define ENTER_CRITICAL_REGION()         {uint8_t sreg = SREG; cli()

/*
 *  This macro restores the global interrupt status
 */
#define LEAVE_CRITICAL_REGION()         SREG = sreg;}

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()      { uint8_t irq_mask = PORTC.INTCTRL; PORTC.INTCTRL &= ~PORT_INT0LVL_gm

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()      PORTC.INTCTRL = irq_mask; }


/*
 * GPIO macros for ATxmega256A3
 */

/*
 * This board uses an SPI-attached transceiver.
 */
#define PAL_USE_SPI_TRX                 (1)

/* Actual Ports */
/*
 * The data direction register for the transceiver
 */
#define TRX_PORT1_DDR                   (PORTC.DIR)

/*
 * The transceiver port
 */
#define TRX_PORT1                       (PORTC)

/*
 * RESET pin of transceiver
 */
#define TRX_RST                         (0)

/*
 * Sleep Transceiver pin
 */
#define SLP_TR                          (3)

/*
 * Slave select pin
 */
#define SEL                             (4)

/*
 * SPI Bus Master Output/Slave Input pin
 */
#define MOSI                            (5)

/*
 * SPI Bus Master Input/Slave Output pin
 */
#define MISO                            (6)

/*
 * SPI serial clock pin
 */
#define SCK                             (7)

/*
 * IRQ pin direct GPIO access
 *
 * use pin 2 directly to avoid additional macro "TRX_IRQ"
 *
 */
#define IRQ_PINGET()                    (TRX_PORT1.IN & _BV(2))

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      (TRX_PORT1.OUTSET = _BV(TRX_RST))
#define RST_LOW()                       (TRX_PORT1.OUTCLR = _BV(TRX_RST))
#define SLP_TR_HIGH()                   (TRX_PORT1.OUTSET = _BV(SLP_TR))
#define SLP_TR_LOW()                    (TRX_PORT1.OUTCLR = _BV(SLP_TR))

/*
 * PORT where the test mode pin TST is connected to.
 */
#define TST_PORT                        (PORTD)

/*
 * PIN number where the test mode pin TST is connected to.
 */
#define TST_PIN                         (1)

/*
 * Initialization of the TST pin: direction is set to output
 */
#define TST_INIT()                      TST_PORT.DIRSET = (1<< TST_PIN)

/*
 * Macro to set TST pin to high
 */
#define TST_PORT_HIGH()                 TST_PORT.OUTSET = (1 << TST_PIN)

/*
 * Macro to set TST pin to low
 */
#define TST_PORT_LOW()                  TST_PORT.OUTCLR = (1 << TST_PIN)


/*
 * PORT where LEDs are connected
 */
#define LED_PORT                        (PORTB)

/*
 * PINs where LEDs are connected
 */
#define LED_PIN_0                       (0)
#define LED_PIN_1                       (1)
#define LED_PIN_2                       (2)

/*
 * PORT where button is connected
 */
#define BUTTON_PORT                     (PORTB)

/*
 * PINs where buttons are connected
 */
#define BUTTON_PIN_0                    (3)

/*
 * PORT Control register for specific button
 */
#define BUTTON_PIN_0_CTRL               (BUTTON_PORT.PIN3CTRL)


/*
 * UART 0 port and pin defines
 * here: UART 0 is located at port D, pin 2 and 3
 */
#ifdef UART0
#define UART0_PORT                      PORTD
#define UART0_TX_PIN                    PIN3_bm
#define UART0_RX_PIN                    PIN2_bm
#define UART0_REG                       USARTD0
#define UART0_RX_ISR_VECT               USARTD0_RXC_vect
#define UART0_TX_ISR_VECT               USARTD0_TXC_vect
#endif

/*
 * UART 1 port and pin defines
 * here: UART 1 is located at port D, pin 6 and 7
 */
#ifdef UART1
#define UART1_PORT                      PORTD
#define UART1_TX_PIN                    PIN7_bm
#define UART1_RX_PIN                    PIN6_bm
#define UART1_REG                       USARTD1
#define UART0_RX_ISR_VECT               USARTD1_RXC_vect
#define UART0_TX_ISR_VECT               USARTD1_TXC_vect
#endif


/*
 * PORT where over-the-air update Serial Flash is connected
 */
#define OTAU_FLASH_PORT                 (PORTD)

/*
 * OTAU flash chip select signal
 * bit position
 */
#define OTAU_FLASH_CS_bp                (4)
#define OTAU_FLASH_CS_bm                (1<<OTAU_FLASH_CS_bp)

/*
 * OTAU flash serial data in
 * bit position
 */
#define OTAU_FLASH_SI_bp                (5)
#define OTAU_FLASH_SI_bm                (1<<OTAU_FLASH_SI_bp)

/*
 * OTAU flash serial data out
 * bit position
 */
#define OTAU_FLASH_SO_bp                (6)
#define OTAU_FLASH_SO_bm                (1<<OTAU_FLASH_SO_bp)

/*
 * OTAU flash serial clock signal
 * bit position
 */
#define OTAU_FLASH_SCK_bp               (7)
#define OTAU_FLASH_SCK_bm               (1<<OTAU_FLASH_SCK_bp)


/*
 * Value of an external PA gain.
 * If no external PA is available, the value is 0.
 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Timer macros for ATxmega256A3
 */
#define WAIT_2_NOPS()                   {nop(); nop();}
#define WAIT_4_NOPS()                   {WAIT_2_NOPS(); WAIT_2_NOPS();}
#define WAIT_8_NOPS()                   {WAIT_4_NOPS(); WAIT_4_NOPS();}
#define WAIT_16_NOPS()                  {WAIT_8_NOPS(); WAIT_8_NOPS();}
#define WAIT_32_NOPS()                  {WAIT_16_NOPS(); WAIT_16_NOPS();}


/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonbly implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 */
#if (F_CPU == (32000000UL))
/*
 * ATxmega256A3 @ 32MHz
 */
    /*
     * Wait for 65 ns.
     * time t7: SLP_TR time (see data sheet or SWPM).
     * In case the system clock is 32 MHz we need to have this value filled,
     * otherwise frame transmission may not be initiated properly.
     */
    #define PAL_WAIT_65_NS()                nop(); nop(); nop()
    /* Wait for 500 ns. */
    #define PAL_WAIT_500_NS()               WAIT_16_NOPS()
    /* Wait for 1 us. */
    #define PAL_WAIT_1_US()                 WAIT_32_NOPS()

#elif (F_CPU == (16000000UL))
/*
 * ATxmega256A3 @ 16MHz
 */
    /*
     * Wait for 65 ns.
     * time t7: SLP_TR time (see data sheet or SWPM).
     */
    #define PAL_WAIT_65_NS()                nop(); nop()
    /* Wait for 500 ns. */
    #define PAL_WAIT_500_NS()               WAIT_8_NOPS()
    /* Wait for 1 us. */
    #define PAL_WAIT_1_US()                 WAIT_16_NOPS()

#elif (F_CPU == (8000000UL))
/*
 * ATxmega256A3 @ 8MHz
 */
    /*
     * Wait for 65 ns.
     * time t7: SLP_TR time (see data sheet or SWPM).
     */
    #define PAL_WAIT_65_NS()                nop()
    /* Wait for 500 ns. */
    #define PAL_WAIT_500_NS()               WAIT_4_NOPS()
    /* Wait for 1 us. */
    #define PAL_WAIT_1_US()                 WAIT_8_NOPS()

#elif (F_CPU == (4000000UL))
/*
 * ATxmega256A3 @ 4MHz
 */
    /*
     * Wait for 65 ns.
     * time t7: SLP_TR time (see data sheet or SWPM).
     * In case the system clock is slower than 32 MHz we do not need
     * to have this value filled.
     */
    #define PAL_WAIT_65_NS()                // empty
    /* Wait for 500 ns. */
    #define PAL_WAIT_500_NS()               WAIT_2_NOPS()
    /* Wait for 1 us. */
    #define PAL_WAIT_1_US()                 WAIT_4_NOPS()

#else
    #error Unknown system clock
#endif

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/**
 * Minimum time in microseconds, accepted as a delay request
 */
#define MIN_DELAY_VAL                   (5)

/*
 * Timer clock source while radio is awake.
 *
 * T1 & T0:
 * clk source is event channel 0 triggered by system clock (16MHz) with prescaler 16
 * TRX's clock (CLKM) is never used with Xmega, keep macro empty.
 */
#define TIMER_SRC_DURING_TRX_AWAKE()

/*
 * Timer clock source while radio is sleeping.
 */
/*
 * T1 & T0:
 * clk source is event channel 0 triggered by system clock with corresponding
 * event system prescaler (see function event_system_init()).
 */
#define TIMER_SRC_DURING_TRX_SLEEP() {TCC0_CTRLA = TC0_CLKSEL3_bm; TCC1_CTRLA = TC1_CLKSEL3_bm;}

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
#define TIME_STAMP_REGISTER             (TCC1_CCA)


/*
 * TRX Access macros for ATxmega256A3
 */

/*
 * Bit mask for slave select
 */
#define SS_BIT_MASK                     (1 << SEL)

/*
 * Slave select made low
 */
#define SS_LOW()                        (TRX_PORT1.OUTCLR = SS_BIT_MASK)

/*
 * Slave select made high
 */
#define SS_HIGH()                       (TRX_PORT1.OUTSET = SS_BIT_MASK)

/*
 * Mask for SPIF bit in status register
 */
#define SPIF_MASK                       (SPI_IF_bm)

/*
 * SPI Data Register
 */
#define SPI_DATA_REG                    (SPIC.DATA)

/*
 * Wait for SPI interrupt flag
 */
#define SPI_WAIT()                      do {                        \
    while ((SPIC.STATUS & SPIF_MASK) == 0) { ; }                    \
} while (0)

/*
 * Dummy value written in SPDR to retrieve data form it
 */
#define SPI_DUMMY_VALUE                 (0x00)

/*
 * TRX Initialization
 */
#if (F_CPU == (32000000UL))
    #define TRX_INIT()                      do {                        \
        /* Enable the SPI and make the microcontroller as SPI master */ \
        /* Set the SPI speed to 4MHz. */                                \
        /* CLK2X = 1; PRESCALER = 1 (clkPER/8) */                       \
        /* Resulting SPI speed: 4MHz */                                 \
        SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | (0 << SPI_MODE0_bp);\
        SPIC.CTRL |= (1 << SPI_CLK2X_bp) | (1 << SPI_PRESCALER0_bp);    \
        /* Set SEL pin to high */                                       \
        TRX_PORT1.OUTSET = _BV(SEL);                                    \
    } while (0)

#elif (F_CPU == (16000000UL))
    #define TRX_INIT()                      do {                        \
        /* Enable the SPI and make the microcontroller as SPI master */ \
        /* Set the SPI speed to 4MHz. */                                \
        /* CLK2X = 0; PRESCALER = 0 (clkPER/4) */                       \
        SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | (0 << SPI_MODE0_bp);\
        /* Set SEL pin to high */                                       \
        TRX_PORT1.OUTSET = _BV(SEL);                                    \
    } while (0)

#elif (F_CPU == (8000000UL))
    #define TRX_INIT()                      do {                        \
        /* Enable the SPI and make the microcontroller as SPI master */ \
        /* Set the SPI speed to 4MHz. */                                \
        /* CLK2X = 1; PRESCALER = 0 (clkPER/2) */                       \
        SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | (0 << SPI_MODE0_bp);\
        SPIC.CTRL |= (1 << SPI_CLK2X_bp);                               \
        /* Set SEL pin to high */                                       \
        TRX_PORT1.OUTSET = _BV(SEL);                                    \
    } while (0)

#elif (F_CPU == (4000000UL))
    #define TRX_INIT()                      do {                        \
        /* Enable the SPI and make the microcontroller as SPI master */ \
        /* Set the SPI speed to 2MHz. */                                \
        /* CLK2X = 1; PRESCALER = 0 (clkPER/2) */                       \
        SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | (0 << SPI_MODE0_bp);\
        SPIC.CTRL |= (1 << SPI_CLK2X_bp);                               \
        /* Set SEL pin to high */                                       \
        TRX_PORT1.OUTSET = _BV(SEL);                                    \
    } while (0)

#else
    #error Unknown system clock
#endif


/*
 * This board does NOT have an external eeprom available.
 */
#ifndef EXTERN_EEPROM_AVAILABLE
#define EXTERN_EEPROM_AVAILABLE            (0)
#endif

/*
 * Alert initialization
 */
#define ALERT_INIT()                    do {    \
        LED_PORT.OUT    = 0;                    \
        LED_PORT.DIRSET = 0xFF;                 \
} while (0)

/*
 * Alert indication
 */
#define ALERT_INDICATE()                (LED_PORT.OUTTGL = 0xFF)


/*
 * Macros for random seed generation by means of the ADC
 */
#define COMMON_MODE_CYCLES              (16)
#define SAMPLE_COUNT                    (16)
#define LSB_MASK_INT_16                 (0x0001)

/*
 * ADC used for random seed generation
 */
#define ADC_FOR_RANDOM_SEED             (ADCA)

/*
 * Channel used from the ADC for random seed generation
 * ! Do not put the define in "()"
 */
#define ADC_CHANNEL                     CH0

/*
 * Positive and negative input multiplexer selection.
 *
 * The pin were selected based on test with this board, but might need to be
 * retested and updated for a new hardware platform.
 */
#define ADC_POS_MUX_SEL                 (2)
#define ADC_NEG_MUX_SEL                 (2)

/*
 * This macro returns the channel conversion complete flag.
 */
#define ADC_CH_CONVERSION_COMPLETEs(_adc_ch)    (((_adc_ch)->INTFLAGS & ADC_CH_CHIF_bm) != 0x00)

/*
 * Sleep  configurations
 */

/*
 * Sleep mode control register
 */
#define SLEEP_CTRL_REG SLEEP.CTRL

/*
 * Bitmask for Sleep Enable bit
 */
#define SLEEP_ENABLE_MASK SLEEP_SEN_bm

/*
 * Bitmask for all sleep mode bits
 */
#define SLEEP_MODE_MASK_GRP SLEEP_SMODE_gm

/*
 * configure sleep ctrl reg - clear SMODE bits & then set,sleep enable bit
 */
#define CONFIGURE_SLEEP( sleep_mode ) \
{ \
    SLEEP_CTRL_REG = (SLEEP_CTRL_REG & ~SLEEP_MODE_MASK_GRP) | \
        (sleep_mode) | SLEEP_ENABLE_MASK; \
}

/*
 *configure sleep ctrl reg to disable sleep - safe
 */
#define DISABLE_SLEEP() SLEEP_CTRL_REG &= ~SLEEP_ENABLE_MASK

/*
* Pwr_mode fun handles the sleeps modes
*/
#define pal_pwr_mode(x)       pal_sleep_mode(x)

/*
* Sleep fuction for AVR and GCC
*/
#if defined(__ICCAVR__)

#define CPU_SLEEP()      __sleep()

#elif defined(__GNUC__)

#define CPU_SLEEP()      sleep_cpu()

#endif

/* === Prototypes ===========================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* REB_2_3_CBB */

#endif  /* PAL_CONFIG_H */
/* EOF */
