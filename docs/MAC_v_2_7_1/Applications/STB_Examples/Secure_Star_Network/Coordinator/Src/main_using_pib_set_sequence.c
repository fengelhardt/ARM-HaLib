/**
 * @file main.c
 *
 * @brief  STB Example - Secure Star Network using MAC 2006 security - Coordinator
 *
 * This is the source code of a Coordinator within a simple STB example based on the MAC layer.
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

typedef struct associated_device_tag
{
    uint16_t short_addr;
    uint64_t ieee_addr;
}
/** This type definition of a structure can store the short address and the
 *  extended address of a device.
 */
associated_device_t;

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
/** Defines the short address of the coordinator. */
#define COORD_SHORT_ADDR                (0x1122)
/** Defines the maximum number of devices the coordinator will handle. */
#define MAX_NUMBER_OF_DEVICES           (2)
/** This is the time period in micro seconds for data transmissions. */
#define DATA_TX_PERIOD                  (2000000)

/** Defines the scan duration time. */
#define SCAN_DURATION                   (4)
/** Defines the maximum number of scans before starting own network. */
#define MAX_NUMBER_OF_SCANS             (3)

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

/* test define for ZIP security testing */
#define SHORT_ADDR_OFFSET               (0x3344)

/* === GLOBALS ============================================================= */

/** This array stores all device related information. */
static associated_device_t device_list[MAX_NUMBER_OF_DEVICES];
/*
 * This is implemented as an array of bytes, but actually this is a
 * 128-bit variable. This is the reason why the array needs to be filled
 * in in reverse order than expected.
 */
static uint8_t default_key[16] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
                                  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF};

static uint8_t default_key_source[8] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* === PROTOTYPES ========================================================== */

static void led_off_cb(void *parameter);
static bool assign_new_short_addr(uint64_t addr64, uint16_t *addr16);

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

/* test define for ZIP security testing */
tal_pib_IeeeAddress = 0xACDE480000000001;

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
        /* Start PIB set sequence. */
        uint8_t short_addr[2];
        short_addr[0] = (uint8_t)COORD_SHORT_ADDR;          // low byte
        short_addr[1] = (uint8_t)(COORD_SHORT_ADDR >> 8);   // high byte
        mlme_set(macShortAddress, NO_PIB_INDEX, (pib_value_t *)&short_addr, false);

        uint8_t association_permit = true;
        mlme_set(macAssociationPermit,
                 NO_PIB_INDEX,
                 (pib_value_t *)&association_permit,
                 true);

        bool rx_on_when_idle = true;
        mlme_set(macRxOnWhenIdle,
                 NO_PIB_INDEX,
                 (pib_value_t *)&rx_on_when_idle,
                 true);

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
                           1UL << DEFAULT_CHANNEL,
                           SCAN_DURATION,
                           DEFAULT_CHANNEL_PAGE);
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
void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute, uint8_t PIBAttributeIndex)
{
    if (status != MAC_SUCCESS)
    {
        // something went wrong; restart
        wpan_mlme_reset_req(true);
    }

    /* Keep compiler happy. */
    status = status;
    PIBAttribute = PIBAttribute;
    PIBAttributeIndex = PIBAttributeIndex;
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
                         15, 15,
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
        /*
         * Network is established.
         * Waiting for association indication from a device.
         * -> usr_mlme_associate_ind
         */
        pal_led(LED_NWK_SETUP, LED_ON);

        /* Set security PIB attributes now. */
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
                                     KEY_INDEX_1,           // LookupData[8]
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
        /* Add security PIBs for devices. */
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
                                    (uint8_t)device_list[0].short_addr,
                                    (uint8_t)(device_list[0].short_addr >> 8),
                                    // Device ExtAddress
                                    (uint8_t)device_list[0].ieee_addr,
                                    (uint8_t)(device_list[0].ieee_addr >> 8),
                                    (uint8_t)(device_list[0].ieee_addr >> 16),
                                    (uint8_t)(device_list[0].ieee_addr >> 24),
                                    (uint8_t)(device_list[0].ieee_addr >> 32),
                                    (uint8_t)(device_list[0].ieee_addr >> 40),
                                    (uint8_t)(device_list[0].ieee_addr >> 48),
                                    (uint8_t)(device_list[0].ieee_addr >> 56),
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

        /* Use DSN start value as in ZIP test spec. */
        uint32_t frame_counter = 1;
        mlme_set(macFrameCounter,
                 NO_PIB_INDEX,
                 (pib_value_t *)&frame_counter,
                 true);
    }

    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
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
                       uint8_t DSN,
#ifdef ENABLE_TSTAMP
                       uint32_t Timestamp,
#endif  /* ENABLE_TSTAMP */
                       uint8_t SecurityLevel,
                       uint8_t KeyIdMode,
                       uint8_t KeyIndex)
{
   /*
    * Dummy data has been received successfully.
    * Application code could be added here ...
    */
    pal_led(LED_DATA, LED_ON);
    // Start a timer switching off the LED
    pal_timer_start(TIMER_LED_OFF,
                    500000,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)led_off_cb,
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
    SecurityLevel = SecurityLevel;
    KeyIdMode = KeyIdMode;
    KeyIndex = KeyIndex;
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



/**
 * @brief Application specific function to assign a short address
 */
static bool assign_new_short_addr(uint64_t addr64, uint16_t *addr16)
{
    uint8_t i;

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
            *addr16 = i + SHORT_ADDR_OFFSET;
            device_list[i].short_addr = i + SHORT_ADDR_OFFSET; /* Get next short address. */
            device_list[i].ieee_addr = addr64;      /* Store extended address. */
            return true;
        }
    }

    /* If we are here, no short address could be assigned. */
    return false;
}

/* EOF */
