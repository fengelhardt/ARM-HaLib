/**
 * @file main.c
 *
 * @brief  STB Example - Secure Star Network using MAC 2006 security - Device
 *
 * This is the source code of a Device within a simple STB example based on the MAC layer.
 * It implements the firmware for all nodes of a network with star topology.
 * The data communication between the nodes is secured using MAC 2006 security.
 *
 * $Id: main_using_pib_set_sequence.c 26661 2011-05-13 12:54:50Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pal.h"
#include "tal.h"
#include "mac_api.h"
#include "app_config.h"
#include "ieee_const.h"

/* === TYPES =============================================================== */

typedef struct node_addr_tag
{
    uint16_t short_addr;
    uint64_t ieee_addr;
}
/** This type definition of a structure can store the short address and the
 *  extended address of a device.
 */
node_addr_t;

/* === MACROS ============================================================== */

#ifdef CHANNEL
#define DEFAULT_CHANNEL                 (CHANNEL)
#define DEFAULT_CHANNEL_PAGE            (0)
#else
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
#endif  /* #ifdef CHANNEL */
/** Defines the PAN ID of the network. */
#ifdef PAN_ID
#define DEFAULT_PAN_ID                  (PAN_ID)
#else
#define DEFAULT_PAN_ID                  (0x1AAA)
#endif
/** This is the time period in micro seconds for data transmissions. */
#define DATA_TX_PERIOD                  (2000000)

/** Defines the scan duration time. */
#define SCAN_DURATION                   (4)

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

/* MAC security macros */
#define KEY_INDEX_1                     (1)
#define LOOKUP_DATA_SIZE_1              (1) // Size is 9 octets
#define FRAME_TYPE_DATA                 (1)
#define CMD_FRAME_ID_NA                 (0) // CommandFrameIdentifier is n/a
#define ZIP_SEC_MIN                     (5) // SecurityMinimum for ZIP is 5
#define DEV_OVERRIDE_SEC_MIN            (1) // DeviceOverrideSecurityMinimum: True
#define ZIP_KEY_ID_MODE                 (1) // ZIP uses KeyIdMode 1

/* === GLOBALS ============================================================= */

/** This structure stores the short and extended address of a node. */
static node_addr_t coord_addr;
/*
 * This is implemented as an array of bytes, but actually this is a
 * 128-bit variable. This is the reason why the array needs to be filled
 * in in reverse order than expected.
 */
static uint8_t default_key[16] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
                                  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};

static uint8_t default_key_source[8] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t payload[23] = {0x7f, 0x33, 0xf2, 0xb1, 0x00, 0x02, 0x74, 0x5a, 0x53, 0x65,
                           0x6e, 0x64, 0x65, 0x72, 0x20, 0x73, 0x61, 0x79, 0x73, 0x20,
                           0x48, 0x69, 0x21};

extern uint16_t mac_pib_macCoordShortAddress;
extern uint64_t mac_pib_macCoordExtendedAddress;

/* === PROTOTYPES ========================================================== */

static void app_timer_cb(void *parameter);
static void network_search_indication_cb(void *parameter);
static void data_exchange_led_off_cb(void *parameter);

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

/* test define for ZIP security testing */
tal_pib_IeeeAddress = 0xACDE480000000002;

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
                           1UL << DEFAULT_CHANNEL,
                           SCAN_DURATION,
                           DEFAULT_CHANNEL_PAGE);

        /* Indicate network scanning by a LED flashing. */
        pal_timer_start(TIMER_LED_OFF,
                        500000,
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
                /* Store the coordinator's address. */
                if (coordinator->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_SHORT)
                {
                    ADDR_COPY_DST_SRC_16(coord_addr.short_addr, coordinator->CoordAddrSpec.Addr.short_address);
                }
                else if (coordinator->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_LONG)
                {
                    ADDR_COPY_DST_SRC_64(coord_addr.ieee_addr, coordinator->CoordAddrSpec.Addr.long_address);
                }
                else
                {
                    /* Something went wrong; restart. */
                    wpan_mlme_reset_req(true);
                    return;
                }

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
                           1UL << DEFAULT_CHANNEL,
                           SCAN_DURATION,
                           DEFAULT_CHANNEL_PAGE);
    }
    else if (status == MAC_NO_BEACON)
    {
        /*
         * No beacon is received; no coordiantor is located.
         * Scan again, but used longer scan duration.
         */
        wpan_mlme_scan_req(MLME_SCAN_TYPE_ACTIVE,
                           1UL << DEFAULT_CHANNEL,
                           SCAN_DURATION,
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
        /* Stop timer used for search indication (same as used for data transmission). */
        pal_timer_stop(TIMER_LED_OFF);
        pal_led(LED_NWK_SETUP, LED_ON);

        /* Start setting of PIBs. */
        bool rx_on_when_idle = true;
        mlme_set(macRxOnWhenIdle,
                 NO_PIB_INDEX,
                 (pib_value_t *)&rx_on_when_idle,
                 true);

        mlme_set(macDefaultKeySource,
                 NO_PIB_INDEX,
                 (pib_value_t *)&default_key_source,
                 true);

        uint8_t mac_sec_level_table_entries = 1;
        mlme_set(macSecurityLevelTableEntries,
                 NO_PIB_INDEX,
                 (pib_value_t *)&mac_sec_level_table_entries,
                 true);

        uint8_t mac_sec_level_table[4] = {FRAME_TYPE_DATA,      // FrameType: Data
                                          CMD_FRAME_ID_NA,      // CommandFrameIdentifier: N/A
                                          ZIP_SEC_MIN,          // SecurityMinimum: 5
                                          DEV_OVERRIDE_SEC_MIN  // DeviceOverrideSecurityMinimum: True
                                         };
        mlme_set(macSecurityLevelTable,
                 0,    // Index: 0
                 (pib_value_t *)&mac_sec_level_table,
                 true);

        uint8_t mac_key_table_entries = 1;
        mlme_set(macKeyTableEntries,
                 NO_PIB_INDEX,
                 (pib_value_t *)&mac_key_table_entries,
                 true);

        uint8_t mac_key_table[34] = {
                                      // KeyIdLookupList[1].LookupData : macDefaultKeySource || g_Sec_KeyIndex_1
                                     default_key_source[0], // LookupData[0]
                                     default_key_source[1], // LookupData[1]
                                     default_key_source[2], // LookupData[2]
                                     default_key_source[3], // LookupData[3]
                                     default_key_source[4], // LookupData[4]
                                     default_key_source[5], // LookupData[5]
                                     default_key_source[6], // LookupData[6]
                                     default_key_source[7], // LookupData[7]
                                     LOOKUP_DATA_SIZE_1, // LookupDataSize: 0x01 : Size 9 octets
                                     1,              // KeyIdLookupListEntries = 1
                                     // KeyDeviceList[1]
                                     0,              // DeviceDescriptorHandle
                                     0,              // UniqueDevice - Key is unique per node
                                     0,              // Blacklisted
                                     1,              // KeyDeviceListEntries
                                     //  KeyUsageList
                                     FRAME_TYPE_DATA,    // FrameType - Data frames
                                     CMD_FRAME_ID_NA,    // CommandFrameIdentifier not used in ZIP
                                     1,                  // KeyUsageListEntries
                                     // Key
                                     default_key[0],
                                     default_key[1],
                                     default_key[2],
                                     default_key[3],
                                     default_key[4],
                                     default_key[5],
                                     default_key[6],
                                     default_key[7],
                                     default_key[8],
                                     default_key[9],
                                     default_key[10],
                                     default_key[11],
                                     default_key[12],
                                     default_key[13],
                                     default_key[14],
                                     default_key[15],
                                    };
        mlme_set(macKeyTable,
                 0,    // Index: 0
                 (pib_value_t *)&mac_key_table,
                 true);

        uint8_t mac_dev_table_entries = 1;
        mlme_set(macDeviceTableEntries,
                 NO_PIB_INDEX,
                 (pib_value_t *)&mac_dev_table_entries,
                 true);

        uint8_t mac_dev_table[17] = {
                                    // DeviceDescriptor
                                    // PANId
                                    (uint8_t)tal_pib_PANId,
                                    (uint8_t)(tal_pib_PANId >> 8),
                                    // Device ShortAddress
                                    (uint8_t)mac_pib_macCoordShortAddress,
                                    (uint8_t)(mac_pib_macCoordShortAddress >> 8),
                                    // Device ExtAddress
                                    (uint8_t)mac_pib_macCoordExtendedAddress,
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 8),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 16),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 24),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 32),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 40),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 48),
                                    (uint8_t)(mac_pib_macCoordExtendedAddress >> 56),
                                    // FrameCounter
                                    0, 0, 0, 0,
                                    // Exempt
                                    0
                                   };
        mlme_set(macDeviceTable,
                 0,    // Index: 0
                 (pib_value_t *)&mac_dev_table,
                 true);

        /* Use DSN start value as in ZIP test spec. */
        uint8_t new_dsn = 0x0;
        mlme_set(macDSN,
                 NO_PIB_INDEX,
                 (pib_value_t *)&new_dsn,
                 true);

        /* Use Frame Counter start value as in ZIP test spec. */
        uint32_t frame_counter = 1;
        mlme_set(macFrameCounter,
                 NO_PIB_INDEX,
                 (pib_value_t *)&frame_counter,
                 true);

        // Start a timer that sends some data to the coordinator every 2 seconds.
        pal_timer_start(TIMER_TX_DATA,
                        DATA_TX_PERIOD,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)app_timer_cb,
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
 * @brief Callback function usr_mlme_set_conf
 *
 * @param status        Result of requested PIB attribute set operation
 * @param PIBAttribute  Updated PIB attribute
 */
void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
    if (status != MAC_SUCCESS)
    {
        // something went wrong; restart
        wpan_mlme_reset_req(true);
    }

    /* Keep compiler happy. */
    PIBAttributeIndex = PIBAttributeIndex;
}



/**
 * @brief Callback function for the application timer
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void app_timer_cb(void *parameter)
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
    static uint8_t msduHandle = 0x0C;

    src_addr_mode = WPAN_ADDRMODE_SHORT;

    dst_addr.AddrMode = WPAN_ADDRMODE_SHORT;
    dst_addr.PANId = DEFAULT_PAN_ID;
    ADDR_COPY_DST_SRC_16(dst_addr.Addr.short_address, coord_addr.short_addr);

    wpan_mcps_data_req(src_addr_mode,
                       &dst_addr,
                       23,  // msduLength
                       payload,
                       msduHandle,
                       WPAN_TXOPT_OFF,
                       ZIP_SEC_MIN,     // SecurityLevel
                       ZIP_KEY_ID_MODE, // KeyIdMode
                       1);  // KeyIndex

    msduHandle++;               // increment handle

    pal_timer_start(TIMER_TX_DATA,
                    DATA_TX_PERIOD,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)app_timer_cb,
                    NULL);

    parameter = parameter;  /* Keep compiler happy. */
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
    if (status == MAC_SUCCESS)
    {
        /*
         * Dummy data has been transmitted successfully.
         * Application code could be added here ...
         */
        pal_led(LED_DATA, LED_ON);
        // Start a timer switching off the LED
        pal_timer_start(TIMER_LED_OFF,
                        500000,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)data_exchange_led_off_cb,
                        NULL);
    }

    /* Keep compiler happy. */
    msduHandle = msduHandle;
#ifdef ENABLE_TSTAMP
    Timestamp = Timestamp;
#endif  /* ENABLE_TSTAMP */
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

    // Re-start led timer again
    pal_timer_start(TIMER_LED_OFF,
                    500000,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)network_search_indication_cb,
                    NULL);

    parameter = parameter;    /* Keep compiler happy. */
}



/**
 * @brief Callback function switching off the LED
 *
 * @param parameter Pointer to callback parameter
 *                  (not used in this application, but could be if desired).
 */
static void data_exchange_led_off_cb(void *parameter)
{
    pal_led(LED_DATA, LED_OFF);

    parameter = parameter;    /* Keep compiler happy. */
}

/* EOF */
