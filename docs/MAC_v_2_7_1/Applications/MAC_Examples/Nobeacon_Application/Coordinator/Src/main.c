/**
 * @file main.c
 *
 * @brief MAC Example Nobeacon Application - Coordinator
 *
 * This is the source code of a simple MAC example. It implements the
 * firmware for the coordinator of a network with star topology.
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

/** This type definition of a structure stores the short address and the
 *  extended address of a device.
 */
typedef struct associated_device_tag
{
    uint16_t short_addr;
    uint64_t ieee_addr;
}associated_device_t;

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
/** Defines the short address of the coordinator. */
#define COORD_SHORT_ADDR                (0x0000)
/** Defines the maximum number of devices this coordinator will handle. */
#define MAX_NUMBER_OF_DEVICES           (2)
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

/** Defines the scan duration time. */
#define SCAN_DURATION_COORDINATOR       (1)
/** Defines Beacon Order for Nobeacon Network. */
#define NOBEACON_BO                     (15)
/** Defines Superframe Order for Nobeacon Network. */
#define NOBEACON_SO                     (15)
/** Defines the time to initiate a indirect data transmission to the device. */
#define APP_INDIRECT_DATA_DURATION_MS   (5000)
/** Define the LED on duration time. */
#define LED_ON_DURATION			(500000)

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

/** This array stores all device related information. */
static associated_device_t device_list[MAX_NUMBER_OF_DEVICES];

/** Stores the number of associated devices. */
static uint8_t no_of_assoc_devices;

/** Store the current MSDU handle to be used for a data frame. */
static uint8_t curr_msdu_handle;

/** Stores total receive count */
static uint32_t rx_cnt;

/* === PROTOTYPES ========================================================== */

static bool assign_new_short_addr(uint64_t addr64, uint16_t *addr16);
static void indirect_data_cb(void *parameter);
static void led_off_cb(void *parameter);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the coordinator application
 *
 * This function initializes the MAC, initiates a MLME reset request
 * (@ref wpan_mlme_reset_req()), and implements a the main loop.
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
    pal_led(LED_NWK_SETUP, LED_OFF);    // indicating network is started
    pal_led(LED_DATA, LED_OFF);         // indicating data reception

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

    /* To make sure the Hyper Terminal Connected to the system*/
    sio_getchar();

    printf("\nNobeacon_Application\n\n");
    printf("\nCoordinator\n\n");
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
        /*
         * Set the short address of this node.
         * Use: bool wpan_mlme_set_req(uint8_t PIBAttribute,
         *                             void *PIBAttributeValue);
         *
         * This request leads to a set confirm message -> usr_mlme_set_conf
         */
        uint8_t short_addr[2];

        short_addr[0] = (uint8_t)COORD_SHORT_ADDR;          // low byte
        short_addr[1] = (uint8_t)(COORD_SHORT_ADDR >> 8);   // high byte
        wpan_mlme_set_req(macShortAddress, short_addr);
    }
    else
    {
        /* Something went wrong; restart. */
        wpan_mlme_reset_req(true);
    }
}



/**
 * @brief Callback function usr_mlme_set_conf
 *
 * @param status        Result of requested PIB attribute set operation
 * @param PIBAttribute  Updated PIB attribute
 */
void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute)
{
    if ((status == MAC_SUCCESS) && (PIBAttribute == macShortAddress))
    {
        /*
         * Allow other devices to associate to this coordinator.
         * Use: bool wpan_mlme_set_req(uint8_t PIBAttribute,
         *                             void *PIBAttributeValue);
         *
         * This request leads to a set confirm message -> usr_mlme_set_conf
         */
        uint8_t association_permit = true;

        wpan_mlme_set_req(macAssociationPermit, &association_permit);
    }
    else if ((status == MAC_SUCCESS) && (PIBAttribute == macAssociationPermit))
    {
        /*
         * Set RX on when idle to enable the receiver as default.
         * Use: bool wpan_mlme_set_req(uint8_t PIBAttribute,
         *                             void *PIBAttributeValue);
         *
         * This request leads to a set confirm message -> usr_mlme_set_conf
         */
        bool rx_on_when_idle = true;

        wpan_mlme_set_req(macRxOnWhenIdle, &rx_on_when_idle);
    }
    else if ((status == MAC_SUCCESS) && (PIBAttribute == macRxOnWhenIdle))
    {
        /*
         * Initiate an active scan over all channels to determine
         * which channel to use.
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
                           SCAN_DURATION_COORDINATOR,
                           DEFAULT_CHANNEL_PAGE);
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
    /*
     * We are not interested in the actual scan result,
     * because we start our network on the pre-defined channel anyway.
     * Start a nonbeacon-enabled network
     * Use: bool wpan_mlme_start_req(uint16_t PANId,
     *                               uint8_t LogicalChannel,
     *                               uint8_t ChannelPage,
     *                               uint8_t BeaconOrder,
     *                               uint8_t SuperframeOrder,
     *                               bool PANCoordinator,
     *                               bool BatteryLifeExtension,
     *                               bool CoordRealignment)
     *
     * This request leads to a start confirm message -> usr_mlme_start_conf
     */
    wpan_mlme_start_req(DEFAULT_PAN_ID,
                         DEFAULT_CHANNEL,
                         DEFAULT_CHANNEL_PAGE,
                         NOBEACON_BO,
			 NOBEACON_SO,
                         true, false, false);

    /* Keep compiler happy. */
    status = status;
    ScanType = ScanType;
    ChannelPage = ChannelPage;
    UnscannedChannels = UnscannedChannels;
    ResultListSize = ResultListSize;
    ResultList = ResultList;
}



/**
 * @brief Callback function usr_mlme_start_conf
 *
 * @param status        Result of requested start operation
 */
void usr_mlme_start_conf(uint8_t status)
{
    if (status == MAC_SUCCESS)
    {
#ifdef SIO_HUB
        printf("Started nonbeacon-enabled network\n");
#endif
        /*
         * Network is established.
         * Waiting for association indication from a device.
         * -> usr_mlme_associate_ind
         */
        pal_led(LED_NWK_SETUP, LED_ON);
    }
    else
    {
        /* Something went wrong; restart. */
        wpan_mlme_reset_req(true);
    }
}



/**
 * @brief Callback function usr_mlme_associate_ind
 *
 * @param DeviceAddress         Extended address of device requesting association
 * @param CapabilityInformation Capabilities of device requesting association
 */
void usr_mlme_associate_ind(uint64_t DeviceAddress,
                            uint8_t CapabilityInformation)
{
    /*
     * Any device is allowed to join the network.
     * Use: bool wpan_mlme_associate_resp(uint64_t DeviceAddress,
     *                                    uint16_t AssocShortAddress,
     *                                    uint8_t status);
     *
     * This response leads to comm status indication -> usr_mlme_comm_status_ind
     * Get the next available short address for this device.
     */
    uint16_t associate_short_addr = macShortAddress_def;

    if (assign_new_short_addr(DeviceAddress, &associate_short_addr) == true)
    {
        wpan_mlme_associate_resp(DeviceAddress,
                                 associate_short_addr,
                                 ASSOCIATION_SUCCESSFUL);
    }
    else
    {
        wpan_mlme_associate_resp(DeviceAddress, associate_short_addr,
                                         PAN_AT_CAPACITY);
    }

    /* Keep compiler happy. */
    CapabilityInformation = CapabilityInformation;
}



/**
 * @brief Callback function usr_mlme_comm_status_ind
 *
 * @param SrcAddrSpec      Pointer to source address specification
 * @param DstAddrSpec      Pointer to destination address specification
 * @param status           Result for related response operation
 */
void usr_mlme_comm_status_ind(wpan_addr_spec_t *SrcAddrSpec,
                              wpan_addr_spec_t *DstAddrSpec,
                              uint8_t status)
{
    if (status == MAC_SUCCESS)
    {
        /*
         * Now the association of the device has been successful and its
         * information, like address, could  be stored.
         * But for the sake of simple handling it has been done
         * during assignment of the short address within the function
         * assign_new_short_addr()
         */

        /* Start timer to initiate indirect data transmission. */
        pal_timer_start(APP_TIMER_INDIRECT_DATA,
                        ((uint32_t)APP_INDIRECT_DATA_DURATION_MS * 1000),
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)indirect_data_cb,
                        NULL);
    }

    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
}



/**
 * Callback function usr_mcps_data_conf
 *
 * @param msduHandle  Handle of MSDU handed over to MAC earlier
 * @param status      Result for requested data transmission request
 * @param Timestamp   The time, in symbols, at which the data were transmitted
 *                    (only if timestamping is enabled).
 *
 */
#ifdef ENABLE_TSTAMP
void usr_mcps_data_conf(uint8_t msduHandle, uint8_t status, uint32_t Timestamp)
#else
void usr_mcps_data_conf(uint8_t msduHandle, uint8_t status)
#endif  /* ENABLE_TSTAMP */
{
#ifdef SIO_HUB
    char sio_array[255];

    sprintf(sio_array, "Result frame with handle %" PRIu8 ": ", msduHandle);
    printf(sio_array);
#endif

    if (status == MAC_SUCCESS)
    {
#ifdef SIO_HUB
        printf("Success\n");
#endif
    }
    else if (status == MAC_TRANSACTION_OVERFLOW)
    {
#ifdef SIO_HUB
        /* Frame could not be placed into the indirect queue. */
        printf("Transaction overflow\n");
#endif
    }
    else if (status == MAC_TRANSACTION_EXPIRED)
    {
#ifdef SIO_HUB
        /*
         * Frame could not be delivered to the target node within
         * the proper time.
         */
        printf("Transaction expired\n");
#endif
    }


    /* Keep compiler happy. */
    msduHandle = msduHandle;
#ifdef ENABLE_TSTAMP
    Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
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

    /* Increments the receive count*/
    rx_cnt++;

    /*
     * Dummy data has been received successfully.
     */
    pal_led(LED_DATA, LED_ON);

     /* Start a timer switching off the LED. */
    pal_timer_start(TIMER_LED_OFF,
                    LED_ON_DURATION,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)led_off_cb,
                    NULL);
#ifdef SIO_HUB
    sprintf(sio_array, "Frame received: %" PRIu32 ",data: %" PRIu8 "\n",
                                        rx_cnt, *msdu);
    printf(sio_array);
#endif
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
 * @brief Callback function for initiation of indirect data transmission
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be used
 *                  to indicated LED to be switched off)
 */
static void indirect_data_cb(void *parameter)
{
    uint8_t cur_device;
    uint8_t src_addr_mode;
    wpan_addr_spec_t dst_addr;
    uint8_t payload;
#ifdef SIO_HUB
    char sio_array[255];
#endif

    /* Loop over all associated devices. */
    for (cur_device = 0; cur_device < no_of_assoc_devices; cur_device++)
    {
#ifdef SIO_HUB
        sprintf(sio_array, "Queue data for device %" PRIu8 " ", cur_device + 1);
        printf(sio_array);
#endif

        /*
         * Request transmission of indirect data to device.
         * This will just queue this frame into the indirect data queue.
         * Once this particular device polls for pending data,
         * the frame will be delivered to the device.
         */
        src_addr_mode = WPAN_ADDRMODE_SHORT;
        dst_addr.AddrMode = WPAN_ADDRMODE_SHORT;
        dst_addr.PANId = DEFAULT_PAN_ID;
        dst_addr.Addr.short_address = device_list[cur_device].short_addr;

        payload = (uint8_t)rand();  // Any dummy data
        curr_msdu_handle++;         // Increment handle
#ifdef SIO_HUB
        sprintf(sio_array, "(MSDU handle: %" PRIu8 ") data: %" PRIu8 "\n",
                                                curr_msdu_handle,payload);
        printf(sio_array);
#endif
        if (!wpan_mcps_data_req(src_addr_mode,
                                &dst_addr,
                                1,  // One octet
                                &payload,
                                curr_msdu_handle,
                                WPAN_TXOPT_INDIRECT_ACK)
           )
        {
            /*
             * Data could not be queued into the indirect queue.
             * Add error handling if required.
             */
        }
    }

    /* Start timer to initiate indirect data transmission. */
    pal_timer_start(APP_TIMER_INDIRECT_DATA,
                    ((uint32_t)APP_INDIRECT_DATA_DURATION_MS * 1000),
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)indirect_data_cb,
                    NULL);

    parameter = parameter;  /* Keep compiler happy. */
}



/**
 * @brief Application specific function to assign a short address
 */
static bool assign_new_short_addr(uint64_t addr64, uint16_t *addr16)
{
    uint8_t i;
#ifdef SIO_HUB
    char sio_array[255];
#endif
    /* Check if device has been associated before. */
    for (i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        if (device_list[i].short_addr == 0x0000)
        {
            /* If the short address is 0x0000, it has not been used before. */
            continue;
        }
        if (device_list[i].ieee_addr == addr64)
        {
            /* Assign the previously assigned short address again. */
            *addr16 = device_list[i].short_addr;
            return true;
        }
    }

    for (i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        if (device_list[i].short_addr == 0x0000)
        {
            *addr16 = CPU_ENDIAN_TO_LE16(i + 0x0001);
            device_list[i].short_addr = CPU_ENDIAN_TO_LE16(i + 0x0001); /* Get next short address. */
            device_list[i].ieee_addr = addr64;      /* Store extended address. */
            no_of_assoc_devices++;
#ifdef SIO_HUB
            sprintf(sio_array, "Device %" PRIu8 " associated\n", i + 1);
            printf(sio_array);
#endif
            return true;
        }
    }

    /* If we are here, no short address could be assigned. */
    return false;
}

/**
 * @brief Callback function switching off the LED
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be used
 *                  to indicated LED to be switched off)
 */
static void led_off_cb(void *parameter)
{
    pal_led(LED_DATA, LED_OFF);

    parameter = parameter;  /* Keep compiler happy. */
}
/* EOF */

