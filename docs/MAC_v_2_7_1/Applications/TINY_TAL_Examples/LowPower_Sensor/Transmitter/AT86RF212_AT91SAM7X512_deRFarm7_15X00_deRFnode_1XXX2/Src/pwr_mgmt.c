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
#include "aic/aic.h"
#include "pwr_mgmt.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */
uint32_t periph_status;

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief disables unnecessary peripherals
 */
void configure_power_saving(void)
{
    // originally the MCK runs with PLL/2,
    // apply new prescaler such that MCK = PLL/8 (12MHz)
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_8;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
}


/**
 * @brief configures the system for an energy-friendly state
 * Some peripherals might become inaccessible if invoked
 */
void low_power_mode(void)
{
    // TODO: save PIO status (directions, levels, pullups)

    // === ADC ================================================================
    // disable the ADC at all
    AT91C_BASE_ADC->ADC_CHDR = 0xFF;
    AT91C_BASE_ADC->ADC_IDR = 0x000FFFFF;
    AT91C_BASE_PMC->PMC_PCDR = 1<<AT91C_ID_ADC;

    // configure ADC inputs PB28-PB30 to be PIO controlled
    static const Pin PIN_ADC_PB27_NEUTRAL = { 1<<27, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_PULLUP};
    static const Pin PIN_ADC_PB28_NEUTRAL = { 1<<28, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT};
    static const Pin PIN_ADC_PB29_NEUTRAL = { 1<<29, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT};
    static const Pin PIN_ADC_PB30_NEUTRAL = { 1<<30, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT};
    PIO_Configure(&PIN_ADC_PB27_NEUTRAL, 1);
    PIO_Configure(&PIN_ADC_PB28_NEUTRAL, 1);
    PIO_Configure(&PIN_ADC_PB29_NEUTRAL, 1);
    PIO_Configure(&PIN_ADC_PB30_NEUTRAL, 1);

    // === TWI ================================================================
    // disable TWI interrupts
    AIC_DisableIT(AT91C_ID_TWI);

    // === TRX interface ======================================================
    // configure transceiver clock _out_ pin
    static const Pin PIN_TRX_CLK_NEUTRAL  = { 1<<15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT};
    PIO_Configure(&PIN_TRX_CLK_NEUTRAL, 1);

    // === SPI ================================================================
    AT91C_BASE_SPI0->SPI_CR = AT91C_SPI_SPIDIS;
    AT91C_BASE_SPI1->SPI_CR = AT91C_SPI_SPIDIS; // transceiver interconnection

    // === GPIO ===============================================================

    // switch LEDs off
    pal_led(LED_0, LED_OFF);
    pal_led(LED_1, LED_OFF);
    pal_led(LED_2, LED_OFF);

    // explicitly switch monitoring pins - Pin36 (VBUS, PB27) L and Pin34 (VBAT,PB28)
    static const Pin PIN_VBAT_NEUTRAL = { 1<<27, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT };
    static const Pin PIN_VBUS_NEUTRAL = { 1<<28, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT };
    PIO_Configure(&PIN_VBAT_NEUTRAL,1);
    PIO_Configure(&PIN_VBUS_NEUTRAL,1);

    // deactivate Pullup for SW2/BMA IRQ line (may be connected to acceleration sensor)
    static const Pin PIN_ACC_INT_NEUTRAL = { 1<<25, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_INPUT, PIO_DEFAULT };
    PIO_Configure(&PIN_ACC_INT_NEUTRAL,1);

    // switch HW revision detection pins to neutral levels
    static const Pin PINS_HWREV_DETECTION_NEUTRAL[] = {
                { 1<<7,  AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT },
                { 1<<25, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT },
                { 1<<26, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT } };
    PIO_Configure(PINS_HWREV_DETECTION_NEUTRAL,3);

    // === DBGU ===============================================================
    // deactivating the DBGU is somehow critical since many functions like to
    // trace to it which would then block forever since the transceiver is disabled ...
    AT91C_BASE_SYS->DBGU_CR = AT91C_US_RXDIS | AT91C_US_TXDIS;
    AT91C_BASE_US1->US_CR   = AT91C_US_RXDIS | AT91C_US_TXDIS;

    // Change DBGU RxD from Peripheral to PIO Control, unnecessary for TxD
    static const Pin PIN_DBGU_RxD_NEUTRAL = { 1<<27, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT };
    PIO_Configure(&PIN_DBGU_RxD_NEUTRAL, 1);
    // Change DBGU RxD from Peripheral to PIO Control, unnecessary for TxD

    // == USB =================================================================
    // disable USB not necessary, since not initialized

    // == peripherals =========================================================
    // save currently enabled periperals
    periph_status = AT91C_BASE_PMC->PMC_PCSR;

    // disable unnecessary peripherals
    uint32_t mask = (1<<AT91C_ID_PIOA) | (1<<AT91C_ID_PIOB) | (1<<AT91C_ID_ADC)
                 | (1<<AT91C_ID_SPI0) | (1<<AT91C_ID_SPI1) | (1<<AT91C_ID_TWI);

    AT91C_BASE_PMC->PMC_PCDR = mask;
}


/**
 * @brief configures the system for an normal mode where all peripherals
 * are accessible. Inverse function for 'low_power_mode()'.
 */
void normal_power_mode(void)
{
    // re-enable peripherals enabled before power down
    AT91C_BASE_PMC->PMC_PCER = periph_status;

    // re-enable SPI transceiver interconnection
    AT91C_BASE_SPI1->SPI_CR = AT91C_SPI_SPIEN;
}


/**
 * @brief prescales the system clock if necessary.
 * Used before invoking 'system_suspend()'.
 */
__attribute__ ((section(".ramfunc")))
void system_clock_down(void)
{
    // switch MCK to SLCK (48MHz -> 32kHz)
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );

    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = 0;

    // Disable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = 0;

    // Voltage regulator in standby mode : Enable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR |= AT91C_VREG_PSTDBY;
}


/**
 * @brief suspend the system by disabling the MCU clock
 */
__attribute__ ((section(".ramfunc")))
void system_suspend(void)
{
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_PCK;
    while ((AT91C_BASE_PMC->PMC_SCSR & AT91C_PMC_PCK) != AT91C_PMC_PCK);
}


/**
 * @brief things to do after the system woke up, i.e. revert clock prescaler
 */
void system_wakeup(void)
{
    // Disable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR &= ~AT91C_VREG_PSTDBY;

    // enable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = (( (AT91C_CKGR_OSCOUNT & (0x06 <<8)) | AT91C_CKGR_MOSCEN ));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) );

    // enable PLL@96MHz, switch back to MCK derived from PLL,
    // all settings conforming to board_lowlevel.c
    AT91C_BASE_PMC->PMC_PLLR = AT91C_CKGR_USBDIV_1 | AT91C_CKGR_OUT_0 | (16 << 8)
                        | (AT91C_CKGR_MUL & (124 << 16)) | (AT91C_CKGR_DIV & 24);

    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK ) );
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );

    // MCK=SLCK/2 : change prescaler first
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=PLLCK/2 : then change source
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_8;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
}


/* EOF */
