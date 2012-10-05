/**
 * @file app_config.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_config.h 28633 2011-09-21 07:56:44Z v_prasad.anjangi $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* === Includes ============================================================= */

#include "stack_config.h"
#include "tal.h"

/* === Macros =============================================================== */

/* Version of the software */
#define MAJOR_NUMBER        2
#define MINOR_NUMBER        1

#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

#if (TAL_TYPE == AT86RF212)
#define DEFAULT_CHANNEL         (1)
#else
#define DEFAULT_CHANNEL         (20)
#endif
#define DEFAULT_PAN_ID          (0xCAFE)
#define DST_PAN_ID              (DEFAULT_PAN_ID)
#define SRC_PAN_ID              (DEFAULT_PAN_ID)
#define DEFAULT_ADDR            (0x0001)
#define DST_SHORT_ADDR          (0xFFFF)

/* Frame overhead due to selected address scheme incl. FCS */
#if (DST_PAN_ID == SRC_PAN_ID)
#define FRAME_OVERHEAD          (11)
#else
#define FRAME_OVERHEAD          (13)
#endif

#define DEFAULT_SCAN_DURATION   8

#define OPERATION_MODE_1        0x01
#define OPERATION_MODE_2        0x02

#if (NO_OF_LEDS >= 3)
#define STATUS_LED              LED_0
#define TX_LED                  LED_1
#define RX_LED                  LED_2
#else
#define STATUS_LED              LED_0
#define TX_LED                  LED_0
#define RX_LED                  LED_1
#endif

/* === Types ================================================================ */

/* Various LED events */
typedef enum
{
    LED_EVENT_TX_FRAME,
    LED_EVENT_RX_FRAME,
    LED_EVENT_POWER_ON,
    LED_EVENT_START_PAIRING,
    LED_EVENT_PAIRING_DONE,
    LED_EVENT_ALL_ON,
    LED_EVENT_ALL_OFF
}led_event_t;

typedef enum
{
  OP_MODE_UNKNOWN,
  OP_MODE_1_KEY_PRESS,
  OP_MODE_2_CHAR_ON_UART,
  OP_MODE_1_RX_FRAME,
  OP_MODE_2_RX_FRAME
}startup_mode_e;

/* Timer ID's used by the Application */
typedef enum
{
    /* App Timers start from APP_FIRST_TIMER_ID */

    /* App timer 1 */
    T_APP_TIMER = (APP_FIRST_TIMER_ID),
    APP_TIMER_TO_TX,
    APP_TIMER_TO_TX_LED_OFF,
    APP_TIMER_TO_RX_LED_OFF
} app_timer_t;

#define NUMBER_OF_APP_TIMERS        (4)

#define TOTAL_NUMBER_OF_TIMERS      (NUMBER_OF_APP_TIMERS + NUMBER_OF_TOTAL_STACK_TIMERS)

/* Additional buffers used by the application */
#define NUMBER_OF_LARGE_APP_BUFS    (10)
#define NUMBER_OF_SMALL_APP_BUFS    (0)

#define TOTAL_NUMBER_OF_LARGE_BUFS  (NUMBER_OF_LARGE_APP_BUFS + NUMBER_OF_LARGE_STACK_BUFS)
#define TOTAL_NUMBER_OF_SMALL_BUFS  (NUMBER_OF_SMALL_APP_BUFS + NUMBER_OF_SMALL_STACK_BUFS)

#define TOTAL_NUMBER_OF_BUFS        (TOTAL_NUMBER_OF_LARGE_BUFS + TOTAL_NUMBER_OF_SMALL_BUFS)

/*
 * UART transmit buffer size
 */
#define UART_MAX_TX_BUF_LENGTH      (10)

/*
 * UART receive buffer size
 */
#define UART_MAX_RX_BUF_LENGTH      (10)

/*
 * USB transmit buffer size
 */
#define USB_TX_BUF_SIZE             (10)

/*
 * USB receive buffer size
 */
#define USB_RX_BUF_SIZE             (10)

/*
 * USB-specific definitions
 */

/*
 * USB Vendor ID (16-bit number)
 */
#define USB_VID                 0x03EB /* Atmel's USB vendor ID */

/*
 * USB Product ID (16-bit number)
 */
#define USB_PID                 0x2018 /* RZ USB stick product ID */

/*
 * USB Release number (BCD format, two bytes)
 */
#define USB_RELEASE             { 0x00, 0x01 } /* 01.00 */

/*
 * Maximal number of UTF-16 characters used in any of the strings
 * below.  This is only used for compilers that cannot handle the
 * initialization of flexible array members within structs.
 */
#define USB_STRING_SIZE         10

/*
 * String representation for the USB vendor name.
 */
#define USB_VENDOR_NAME L"ATMEL"

/*
 * String representation for the USB product name.
 */
#define USB_PRODUCT_NAME L"RZUSBSTICK"

/*
 * USB Flushed data size
 */
#define NB_MS_BEFORE_FLUSH           (50)

/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

/* Macro to enable the feature of counting wrong CRC packets */
#if ( (TAL_TYPE == ATMEGARF_TAL_1) || (TAL_TYPE == AT86RF231) ||\
      (TAL_TYPE == AT86RF212) )
#define CRC_SETTING_ON_REMOTE_NODE
#endif

/* === Externals ============================================================ */
extern void (*app_task)(void);
extern void (*app_tx_frame_done_cb)(retval_t status, frame_info_t *frame);
extern void (*app_rx_frame_cb)(frame_info_t *frame);

extern frame_info_t *tx_frame_info;

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    extern board_t board ;
    #endif
#endif

/* === Prototypes =========================================================== */
extern uint8_t app_debounce_button(void);
extern void app_led_event(led_event_t ev);
extern void app_task_startup_init();

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_CONFIG_H */
/* EOF */
