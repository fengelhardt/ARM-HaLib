/**
 * @file board.h
 *
 * @brief AT91Lib board specific functionality
 *
 * This file provides a number of definitions of deRFusb_23E06 characteristics,
 * AT91SAM3S-dependent PIOs and external components interfacing, which are
 * needed by the AT91Lib and therefore cannot be placed into the pal_config.h.
 *
 * $Id: board.h,v 1.1.4.1 2011/07/08 16:45:51 dam Exp $
 *
 *  @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

#ifndef BOARD_H
#define BOARD_H

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "AT91SAM3S4.h"

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// Board
//------------------------------------------------------------------------------
/** Name of the board */
#define BOARD_NAME "deRFusb_13E06"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Clocks
//------------------------------------------------------------------------------
/** Frequency of the board main oscillator (in Hz) */
#define BOARD_MAINOSC           18432000

/* Master clock frequency (in Hz), switchable between 32000000 and 48000000 */
#define BOARD_MCK               32000000
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USB
//------------------------------------------------------------------------------
/** Indicates chip has an UDP Full Speed. */
#define CHIP_USB_UDP

/** Indicates chip has an internal pull-up. */
#define CHIP_USB_PULLUP_INTERNAL

/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 8

#define CHIP_USB_ENDPOINTS_MAXPACKETSIZE(i) \
   ((i == 0) ? 64 : \
   ((i == 1) ? 64 : \
   ((i == 2) ? 64 : \
   ((i == 3) ? 64 : \
   ((i == 4) ? 512 : \
   ((i == 5) ? 512 : \
   ((i == 6) ? 64 : \
   ((i == 7) ? 64 : 0 ))))))))

/** Endpoints Number of Bank */
#define CHIP_USB_ENDPOINTS_BANKS(i) \
   ((i == 0) ? 1 : \
   ((i == 1) ? 2 : \
   ((i == 2) ? 2 : \
   ((i == 3) ? 1 : \
   ((i == 4) ? 2 : \
   ((i == 5) ? 2 : \
   ((i == 6) ? 2 : \
   ((i == 7) ? 2 : 0 ))))))))

/** USB attributes configuration descriptor (bus or self powered, remote wakeup) */
#define BOARD_USB_BMATTRIBUTES                  USBConfigurationDescriptor_BUSPOWERED_NORWAKEUP
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Pins
//------------------------------------------------------------------------------
/** UART pins (UTXD0 and URXD0) definitions, PA9,10. */
#define PINS_UART      {0x00000600, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** LED #0 pin definition (RED). */
#define PIN_LED_0      {1 << 19, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** LED #1 pin definition (YELLOW). */
#define PIN_LED_1      {1 << 17, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** LED #2 pin definition (GREEN). */
#define PIN_LED_2      {1 << 18, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** List of all LEDs definitions. */
#define PINS_LEDS      PIN_LED_0, PIN_LED_1, PIN_LED_2

/** TRX and RXTS IRQ Pins */
#define TRX_INT_PIN    {1 << 1 , PIOA, ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_IT_RISE_EDGE}
#define TRX_RXTS_PIN   {1 << 15, PIOA, ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_IT_RISE_EDGE}

/* TRX-MCU connections provided for pal_config.h */
#define PIN_RST        {1 << 24, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_PULLUP}
#define PIN_SLP_TR     {1 << 21, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External components
//------------------------------------------------------------------------------
/** MCI pins definition. */
#define PINS_MCI       {0x3f << 26, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_PULLUP}
/** MCI pin Card Detect. */
#define PIN_MCI_CD     {1 << 6, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}

/** SPI MISO pin definition. */
#define PIN_SPI_MISO   {1 << 12, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI MOSI pin definition. */
#define PIN_SPI_MOSI   {1 << 13, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI SPCK pin definition. */
#define PIN_SPI_SPCK   {1 << 14, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin, used as GPIO. */
#define PIN_SEL        {1 << 11, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_PULLUP}
/** List of SPI pin definitions (MISO, MOSI & SPCK). */
#define PINS_SPI       PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK

/** Base address of SPI peripheral connected to the RF TRX. */
#define BOARD_TRX_SPI_BASE         SPI
/** Identifier of SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_ID           ID_SPI
/** Pins of the SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_PINS         PINS_SPI
/** Chip select connected to the transceiver. */
#define BOARD_TRX_NPCS             2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Memories
//------------------------------------------------------------------------------
/** Internal SRAM address */
#define AT91C_ISRAM                   AT91C_IRAM
/** Internal SRAM size */
#define AT91C_ISRAM_SIZE              0x00008000

/** Internal Flash size */
#define AT91C_IFLASH_SIZE               (0x40000)
/** Internal Flash page size */
#define AT91C_IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
/** Internal Flash number of pages */
#define AT91C_IFLASH_NB_OF_PAGES           (1024) /* Internal FLASH 0 Number of Pages: 512 */
/** Internal Flash lock region size */
#define AT91C_IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
/** Internal Flash number of lock bits */
#define AT91C_IFLASH_NB_OF_LOCK_BITS         (16) /* Internal FLASH 0 Number of Lock Bits: 16 */
//------------------------------------------------------------------------------


#endif /* #ifndef BOARD_H */
