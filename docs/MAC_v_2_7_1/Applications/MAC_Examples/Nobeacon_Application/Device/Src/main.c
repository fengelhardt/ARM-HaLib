/**
 * @file main.c
 *
 * @brief MAC Example Nobeacon Application - Device
 *
 * This is the source code of a simple MAC example. It implements the
 * firmware for the Device of a network with star topology.
 *
 * The coordinator puts the data in the indirect queue periodically
 * and transmits data frames based on the periodic poll request from the device.
 *
 * The Device receives the data from the coordinator and redirects the data
 * back to the coordinator using direct transmission
 *
 * $Id: main.c 28120 2011-08-18 06:07:25Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "pal.h"
#include "tal.h"
#include "sio_handler.h"
#include "mac_api.h"
#include "app_config.h"
#include "ieee_const.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/** Defines the default channel. */
#if (TAL_TYPE == AT86RF212)
    #ifdef CHINESE_BAND
        #define DEFAULT_CHANNEL                 (0)
        #define DEFAULT_CHANNEL_PAGE            (5)
    #else
        #define DEFAULT_CHANNEL                 (1)
        #define DEFAULT_CHANNEL_PAGE            (0)
    #endif  /* #ifdef CHINESE_BAND */
#else
#define DEFAULT_CHANNEL                 (20)
#define DEFAULT_CHANNEL_PAGE            (0)
#endif  /* #if (TAL_TYPE == AT86RF212) */
/** Defines the PAN ID of the network. */
#define DEFAULT_PAN_ID                  CCPU_ENDIAN_TO_LE16(0xBABE)
/** This is a device which will communicate with a single coordinator.
 *  Therefore, the maximum number of devices this code needs to
 *  handle is one.
 */
#define MAX_NUMBER_OF_DEVICES           (1)

/** This is the time period in micro seconds for polling transmissions. */
#define APP_POLL_PERIOD_MS              (5000)

/** Define the LED on duration time. */
#define LED_ON_DURATION			(500000)

/** Defines the bit mask of channels that should be scanned. */
#if (TAL_TYPE == AT86RF212)
    #if (DEFAULT_CHANNEL == 0)
        #define SCAN_ALL_CHANNELS       (0x00000001)
    #else
        #define SCAN_ALL_CHANNELS       (0x000007FE)
    #endif
#else
#define SCAN_ALL_CHANNELS               (0x07FFF800)
#endif

/** Defines the short scan duration time. */
#define SCAN_DURATION_SHORT             (5)
/** Defines the long scan duration time. */
#define SCAN_DURATION_LONG              (6)

#if (NO_OF_LEDS >= 3)
#define LED_START                       (LED_0)
#define LED_NWK_SETUP                   (LED_1)
#define LED_DATA                        (LED_2)
#elif (NO_OF_LEDS == 2)
#define LED_START                       (LED_0)
#define LED_NWK_SETUP                   (LED_0)
#define LED_DATA                        (LED_1)
#else
#define LED_START                       (LED_0)
#define LED_NWK_SETUP                   (LED_0)
#define LED_DATA                        (LED_0)
#endif

/* === GLOBALS ============================================================= */

/** This structure stores the short address of the coordinator. */
static uint16_t coord_addr;

/*Stores total receive count*/
static uint32_t rx_cnt;

/* === PROTOTYPES ========================================================== */

static void app_initiate_polling(void *parameter);
static void network_search_indication_cb(void *parameter);
static void rx_data_led_off_cb(void *parameter);
static void send_data(uint8_t *data);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the device application
 */
int main(void)
{
    /* Initialize the MAC layer and its underlying layers, like PAL, TAL, BMM. */
    if (wpan_init() != MAC_SUCCESS)
    {
        /*
         * Stay here; we need a valid IEEE address.
         * Check kit documentation how to create an IEEE address
         * and to store it into the EEPROM.
         */
        pal_alert();
    }

    /* Initialize LEDs. */
    pal_led_init();
    pal_led(LED_START, LED_ON);         // indicating application is started
    pal_led(LED_NWK_SETUP, LED_OFF);    // indicating node is associated
    pal_led(LED_DATA, LED_OFF);         // indicating successfull data transmission

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();

#ifdef SIO_HUB
    /* Initialize the serial interface used for communication with terminal program. */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        /* Something went wrong during initialization. */
        pal_alert();
    }

#if ((!defined __ICCAVR__) && (!defined __ICCARM__) && (!defined __GNUARM__) && \
     (!defined __ICCAVR32__) && (!defined __GNUAVR32__))
    fdevopen(_sio_putchar, _sio_getchar);
#endif

    /* To make sure the hyper terminal gets connected to the system */
    sio_getchar();

    printf("\nNobeacon_Application\n\n");
    printf("\nDevice\n\n");
#endif /* SIO_HUB */

    /*
     * Reset the MAC layer to the default values.
     * This request will cause a mlme reset confirm message ->
     * usr_mlme_reset_conf
     */
    wpan_mlme_reset_req(true);

    /* Main loop */
    while (1)
    {
        wpan_task();
    }
}



/**
 * @brief Callback function usr_mlme_reset_conf
 *
 * @param status Result of the reset procedure
 */
void usr_mlme_reset_conf(uint8_t status)
{
    if (status == MAC_SUCCESS)
    {
#ifdef SIO_HUB
        printf("Searching network\n");
#endif

        /*
         * Initiate an active scan over all channels to determine
         * which channel is used by the coordinator.
         * Use: bool wpan_mlme_scan_req(uint8_t ScanType,
         *                              uint32_t ScanChannels,
         *                              uint8_t ScanDuration,
         *                              uint8_t ChannelPage);
         *
         * This request leads to a scan confirm message -> usr_mlme_scan_conf
         * Scan for about 50 ms on each channel -> ScanDuration = 1
         * Scan for about 1/2 second on each channel -> ScanDuration = 5
         * Scan for about 1 second on each channel -> ScanDuration = 6
         */
        wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
                           SCAN_ALL_CHANNELS,
                           SCAN_DURATION_SHORT,
                           DEFAULT_CHANNEL_PAGE);

        /* Indicate network scanning by a LED flashing. */
        pal_timer_start(APP_TIMER_LED_OFF,
                        LED_ON_DURATION,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)network_search_indication_cb,
                        NULL);
    }
    else
    {
        /* Something went wrong; restart. */
        wpan_mlme_reset_req(true);
    }
}



/**
 * @brief Callback function usr_mlme_scan_conf
 *
 * @param status            Result of requested scan operation
 * @param ScanType          Type of scan performed
 * @param ChannelPage       Channel page on which the scan was performed
 * @param UnscannedChannels Bitmap of unscanned channels
 * @param ResultListSize    Number of elements in ResultList
 * @param ResultList        Pointer to array of scan results
 */
void usr_mlme_scan_conf(uint8_t status,
                        uint8_t ScanType,
                        uint8_t ChannelPage,
                        uint32_t UnscannedChannels,
                        uint8_t ResultListSize,
                        void *ResultList)
{
    if (status == MAC_SUCCESS)
    {
        wpan_pandescriptor_t *coordinator;
        uint8_t i;

        /*
         * Analyze the ResultList.
         * Assume that the first entry of the result list is our coodinator.
         */
        coordinator = (wpan_pandescriptor_t *)ResultList;

        for (i = 0; i < ResultListSize; i++)
        {
            /*
             * Check if the PAN descriptor belongs to our coordinator.
             * Check if coordinator allows association.
             */
            if ((coordinator->LogicalChannel == DEFAULT_CHANNEL) &&
                (coordinator->ChannelPage == DEFAULT_CHANNEL_PAGE) &&
                (coordinator->CoordAddrSpec.PANId == DEFAULT_PAN_ID) &&
                ((coordinator->SuperframeSpec & ((uint16_t)1 << ASSOC_PERMIT_BIT_POS)) == ((uint16_t)1 << ASSOC_PERMIT_BIT_POS))
               )
            {
                /* Store the coordinator's address information. */
                if (coordinator->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_SHORT)
                {
                    coord_addr = coordinator->CoordAddrSpec.Addr.short_address;
                }
                else
                {
                    /* Something went wrong; restart. */
                    wpan_mlme_reset_req(true);
                    return;
                }
#ifdef SIO_HUB
                printf("Found network\n");
#endif

                /*
                 * Associate to our coordinator.
                 * Use: bool wpan_mlme_associate_req(uint8_t LogicalChannel,
                 *                                   uint8_t ChannelPage,
                 *                                   wpan_addr_spec_t *CoordAddrSpec,
                 *                                   uint8_t CapabilityInformation);
                 * This request will cause a mlme associate confirm message ->
                 * usr_mlme_associate_conf.
                 */
                wpan_mlme_associate_req(coordinator->LogicalChannel,
                                        coordinator->ChannelPage,
                                        &(coordinator->CoordAddrSpec),
                                        WPAN_CAP_ALLOCADDRESS);
                return;
            }

            /* Get the next PAN descriptor. */
            coordinator++;
        }

        /*
         * If here, the result list does not contain our expected coordinator.
         * Let's scan again.
         */
        wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
                           SCAN_ALL_CHANNELS,
                           SCAN_DURATION_SHORT,
                           DEFAULT_CHANNEL_PAGE);
    }
    else if (status == MAC_NO_BEACON)
    {
        /*
         * No beacon is received; no coordiantor is located.
         * Scan again, but used longer scan duration.
         */
        wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
                           SCAN_ALL_CHANNELS,
                           SCAN_DURATION_LONG,
                           DEFAULT_CHANNEL_PAGE);
    }
    else
    {
        /* Something went wrong; restart. */
        wpan_mlme_reset_req(true);
    }

    /* Keep compiler happy. */
    ScanType = ScanType;
    ChannelPage = ChannelPage;
    UnscannedChannels = UnscannedChannels;
}



/**
 * @brief Callback function usr_mlme_associate_conf
 *
 * @param AssocShortAddress    Short address allocated by the coordinator
 * @param status               Result of requested association operation
 */
void usr_mlme_associate_conf(uint16_t AssocShortAddress, uint8_t status)
{
    if (status == MAC_SUCCESS)
    {
#ifdef SIO_HUB
        printf("Conntected to nonbeacon-enabled network\n");
#endif

        /* Stop timer used for search indication (same as used for data transmission). */
        pal_timer_stop(APP_TIMER_LED_OFF);
        pal_led(LED_NWK_SETUP, LED_ON);

        /* Start a timer that polls for pending data at the coordinator. */
        pal_timer_start(APP_TIMER_POLL_DATA,
                        ((uint32_t)APP_POLL_PERIOD_MS * 1000),
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)app_initiate_polling,
                        NULL);
    }
    else
    {
        /* Something went wrong; restart. */
        wpan_mlme_reset_req(true);
    }

    /* Keep compiler happy. */
    AssocShortAddress = AssocShortAddress;
}



/**
 * @brief Callback function usr_mcps_data_ind
 *
 * @param SrcAddrSpec      Pointer to source address specification
 * @param DstAddrSpec      Pointer to destination address specification
 * @param msduLength       Number of octets contained in MSDU
 * @param msdu             Pointer to MSDU
 * @param mpduLinkQuality  LQI measured during reception of the MPDU
 * @param DSN              DSN of the received data frame.
 * @param Timestamp        The time, in symbols, at which the data were received
 *                         (only if timestamping is enabled).
 */
void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
                       wpan_addr_spec_t *DstAddrSpec,
                       uint8_t msduLength,
                       uint8_t *msdu,
                       uint8_t mpduLinkQuality,
#ifdef ENABLE_TSTAMP
                       uint8_t DSN,
                       uint32_t Timestamp)
#else
                       uint8_t DSN)
#endif  /* ENABLE_TSTAMP */
{
#ifdef SIO_HUB
    char sio_array[255];
#endif

    /*increments the receive count*/
    rx_cnt++;

#ifdef SIO_HUB
    sprintf(sio_array, "Frame received: %" PRIu32 ",data: %" PRIu8 "\n",rx_cnt, *msdu);
    printf(sio_array);
#endif

    /*
     * Dummy data has been received successfully.
     */
    pal_led(LED_DATA, LED_ON);

    /*
     * Device re-sending the data received from the coordinator
     */
    send_data(msdu);

    printf("Frame Transmitted\n");
     /* Start a timer switching off the LED. */
    pal_timer_start(APP_TIMER_LED_OFF,
                    LED_ON_DURATION,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)rx_data_led_off_cb,
                    NULL);

    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
    msduLength = msduLength;
    msdu = msdu;
    mpduLinkQuality = mpduLinkQuality;
    DSN = DSN;
#ifdef ENABLE_TSTAMP
    Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
}



/**
 * @brief Callback function for the application timer
 *        initiating polling of coordinator
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void app_initiate_polling(void *parameter)
{
    /*
     * Initiate MLME-POLL.request service and have it placed in the MLME-SAP queue.
     *
     * @param CoordAddrSpec   Pointer to wpan_addr_spec_t structure for the coordinator.
     *
     * @return true - success; false - buffer not availability or queue full.
     */
    wpan_addr_spec_t coord_addr_spec;

    coord_addr_spec.AddrMode = WPAN_ADDRMODE_SHORT;
    coord_addr_spec.PANId = DEFAULT_PAN_ID;
    coord_addr_spec.Addr.short_address = coord_addr;
#ifdef SIO_HUB
    printf("Poll coordinator\n");
#endif
    wpan_mlme_poll_req(&coord_addr_spec);
    /*Keep compiler happy*/
    parameter = parameter;
}



/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-POLL.confirm.
 *
 * @param status           Result of requested poll operation.
 *
 * @return void
 */
void usr_mlme_poll_conf(uint8_t status)
{
    /* Start a timer that polls for pending data at the coordinator. */
    pal_timer_start(APP_TIMER_POLL_DATA,
                    ((uint32_t)APP_POLL_PERIOD_MS * 1000),
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)app_initiate_polling,
                    NULL);

    status = status;    /* Keep compiler happy. */
}



/**
 * @brief Callback function indicating network search
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void network_search_indication_cb(void *parameter)
{
    pal_led(LED_NWK_SETUP, LED_TOGGLE);

    /* Re-start led timer again. */
    pal_timer_start(APP_TIMER_LED_OFF,
                    LED_ON_DURATION,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)network_search_indication_cb,
                    NULL);

    parameter = parameter;    /* Keep compiler happy. */
}



/**
 * @brief Callback function for switching off the LED after data reception
 *
 * @param parameter Pointer to callback parameter
 *
 */
static void rx_data_led_off_cb(void *parameter)
{
    pal_led(LED_DATA, LED_OFF);

    parameter = parameter;    /* Keep compiler happy. */
}
/**
 * @brief function for the sending received data back to coordinator
 *
 * @param data Pointer to received data
 *
 */
static void send_data(uint8_t *data)
{
    /*
     * Send some data and restart timer.
     * Use: bool wpan_mcps_data_req(uint8_t SrcAddrMode,
     *                              wpan_addr_spec_t *DstAddrSpec,
     *                              uint8_t msduLength,
     *                              uint8_t *msdu,
     *                              uint8_t msduHandle,
     *                              uint8_t TxOptions);
     *
     * This request will cause a mcps data confirm message ->
     * usr_mcps_data_conf
     */

    uint8_t src_addr_mode;
    wpan_addr_spec_t dst_addr;
    static uint8_t msduHandle = 0;

    src_addr_mode = WPAN_ADDRMODE_SHORT;

    dst_addr.AddrMode = WPAN_ADDRMODE_SHORT;
    dst_addr.PANId = DEFAULT_PAN_ID;
    ADDR_COPY_DST_SRC_16(dst_addr.Addr.short_address, coord_addr);

    msduHandle++;
    wpan_mcps_data_req(src_addr_mode,
                       &dst_addr,
                       1,
                       data,
                       msduHandle,
                       WPAN_TXOPT_ACK);

}
/* EOF */
