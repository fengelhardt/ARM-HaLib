/**
 * @file cmd_handler.c
 *
 * @brief responses coming from the MAC
 *
 * $Id: rsp_handler.c 28944 2011-10-18 11:27:09Z viswanadham.kotla $
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

#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "pal.h"
#include "return_val.h"
#include "bmm.h"
#include "qmm.h"
#include "tal.h"
#include "ieee_const.h"
#include "private_const.h"
#include "stack_config.h"
#include "mac_msg_const.h"
#include "mac_api.h"
#include "mac_msg_types.h"
#include "mac_data_structures.h"
#include "mac_internal.h"
#include "mac.h"
#include "pal.h"
#include "sio_handler.h"
#include "cmd_parser.h"

/* === MACROS ============================================================== */

/* TODO: remove */
#define EOT 0
/*static inline uint8_t *get_next_tx_buffer(void)
{
    return 0;
}*/

/* Buffer for storing responses */
extern char rsp_buffer[];

/* === Globals ============================================================= */

extern uint8_t auto_assoc;
extern uint16_t assoc_short_addr;

/*=========Prototypes======================================================= */
uint8_t sprintf_ext_address(uint64_t* ext_address,uint8_t currentPos);

/* === Implementation ======================================================*/

/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-RESET.confirm.
 *
 * @param status           Result of requested reset operation.
 *
 * @return void
 */
void usr_mlme_reset_conf(uint8_t status)
{
    sprintf(rsp_buffer, "+RESET: %02X", status);
    send_response(CMD_RESULT_OK, rsp_buffer);
}

#if (MAC_GET_SUPPORT == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-GET.confirm.
 *
 * @param status            Result of requested PIB attribute get operation.
 * @param PIBAttribute      Retrieved PIB attribute.
 * @param PIBAttributeValue Pointer to data containing retrieved PIB attribute,
 *
 * @return void
 */
void usr_mlme_get_conf(uint8_t status,
                       uint8_t PIBAttribute,
                       void *PIBAttributeValue)
{
    uint8_t i, n;
    uint8_t pib_attribute_octet_no;

    /* Get size of PIB attribute to be set */
    pib_attribute_octet_no = mac_get_pib_attribute_size(PIBAttribute);

    n = sprintf(rsp_buffer, "+PIB%02X: %02X", PIBAttribute, status);

    if (status == MAC_SUCCESS)
    {
        n += sprintf(&rsp_buffer[n], ",");
        for (i = 0; i < pib_attribute_octet_no; i++)
        {
            n += sprintf(&rsp_buffer[n], "%02X", ((uint8_t *)PIBAttributeValue)[i]);
        }
    }
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_GET_SUPPORT == 1) || defined(DOXYGEN) */



/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-SET.confirm.
 *
 * @param status        Result of requested PIB attribute set operation.
 * @param PIBAttribute  Updated PIB attribute.
 *
 * @return void
 */
void usr_mlme_set_conf(uint8_t status,
                       uint8_t PIBAttribute)
{
    sprintf(rsp_buffer, "+PIB%02x: %02X", PIBAttribute, status);
    send_response(CMD_RESULT_OK, rsp_buffer);
}



#if (MAC_START_REQUEST_CONFIRM == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-START.confirm.
 *
 * @param status        Result of requested start operation.
 *
 * @return void
 */
void usr_mlme_start_conf(uint8_t status)
{
    sprintf(rsp_buffer, "+START: %02X", status);
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_START_REQUEST_CONFIRM == 1) || defined(DOXYGEN) */


#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-ASSOCIATE.indication.
 *
 * @param DeviceAddress         Extended address of device requesting association.
 * @param CapabilityInformation Capabilities of device requesting association.
 *                              (@ref WPAN_CAP_ALTPANCOORD |
 *                               @ref WPAN_CAP_FFD |
 *                               @ref WPAN_CAP_PWRSOURCE |
 *                               @ref WPAN_CAP_RXONWHENIDLE |
 *                               @ref WPAN_CAP_ALLOCADDRESS)
 *
 * @return void
 */
void usr_mlme_associate_ind(uint64_t DeviceAddress,
                            uint8_t CapabilityInformation)
{
    int8_t n;

    /* Prepare the unsolicited response to be sent up */
    n = sprintf(rsp_buffer, "+INASSOC: ");


    n = sprintf_ext_address(&DeviceAddress, n);

    n += sprintf(&rsp_buffer[n], ",%02X", CapabilityInformation);

    send_response(CMD_RESULT_PENDING, rsp_buffer);

    /* Send association response if auto assoc is set */
    if (auto_assoc)
    {
        int status = wpan_mlme_associate_resp(DeviceAddress, assoc_short_addr, 0);
        sprintf(rsp_buffer, "+AUTORSP: %02X,%04X", status, assoc_short_addr);

        send_response(CMD_RESULT_PENDING, rsp_buffer);

        if (assoc_short_addr < 0xfffe)
        {
            assoc_short_addr++;
        }
    }
}
#endif /* MAC_ASSOCIATION_INDICATION_RESPONSE */


#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-ASSOCIATE.confirm.
 *
 * @param AssocShortAddress    Short address allocated by the coordinator.
 * @param status               Result of requested association operation.
 *
 * @return void
 *
 * @ingroup apiMacCb
 */
void usr_mlme_associate_conf(uint16_t AssocShortAddress,
                             uint8_t status)
{
    sprintf(rsp_buffer, "+ASSOC: %02X %04X", status, AssocShortAddress);
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */


#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-COMM-STATUS.indication.
 *
 * @param SrcAddrSpec      Pointer to source address specification.
 * @param DstAddrSpec      Pointer to destination address specification.
 * @param status           Result for related response operation.
 *
 * @return void
 */
void usr_mlme_comm_status_ind(wpan_addr_spec_t *SrcAddrSpec,
                              wpan_addr_spec_t *DstAddrSpec,
                              uint8_t status)
{
     int n = sprintf(rsp_buffer, "+INCOMM: ");

    /*Src address specifiction*/
    n += sprintf(&rsp_buffer[n], "%02X", SrcAddrSpec->AddrMode);
    n += sprintf(&rsp_buffer[n], ",%04X", SrcAddrSpec->PANId);

    switch (SrcAddrSpec->AddrMode)
    {
        case WPAN_ADDRMODE_SHORT:
            n += sprintf(&rsp_buffer[n], ",%04X", SrcAddrSpec->Addr.short_address);
            break;

        case WPAN_ADDRMODE_LONG:
          {
              n += sprintf(&rsp_buffer[n], ",");
              n = sprintf_ext_address(&SrcAddrSpec->Addr.long_address, n);
          }
            break;
    }
    /*Destination Address specification*/
    n += sprintf(&rsp_buffer[n], ",%02X", DstAddrSpec->AddrMode);
    n += sprintf(&rsp_buffer[n], ",%04X", DstAddrSpec->PANId);


    switch (DstAddrSpec->AddrMode)
    {
        case WPAN_ADDRMODE_SHORT:
            n += sprintf(&rsp_buffer[n], ",%04X", DstAddrSpec->Addr.short_address);
            break;

        case WPAN_ADDRMODE_LONG:
          {
              n += sprintf(&rsp_buffer[n], ",");
              n = sprintf_ext_address(&DstAddrSpec->Addr.long_address, n);
          }
          break;
    }
    /* mlme_comm_status_inication status */
    n += sprintf(&rsp_buffer[n], ",%02X", status);
    /* Send  mlme_comm_status_inication to serial interface*/
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) */


#if (MAC_PURGE_REQUEST_CONFIRM == 1)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MCPS-PURGE.confirm.
 *
 * @param msduHandle           Handle (id) of MSDU to be purged.
 * @param status               Result of requested purge operation.
 *
 * @return void
 */
void usr_mcps_purge_conf(uint8_t msduHandle,
                         uint8_t status)
{
    /* Send the Purge confirm to serial interface */
    sprintf(rsp_buffer, "+PURGE: %02X %02X", msduHandle, status);
    send_response(CMD_RESULT_OK, rsp_buffer);

}

#endif /* MAC_PURGE_REQUEST_CONFIRM */


#if (MAC_BEACON_NOTIFY_INDICATION == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-BEACON-NOTIFY.indication.
 *
 * @param BSN            Beacon sequence number.
 * @param PANDescriptor  Pointer to PAN descriptor for received beacon.
 * @param PendAddrSpec   Pending address specification in received beacon.
 * @param AddrList       List of addresses of devices the coordinator has pending data.
 * @param sduLength      Length of beacon payload.
 * @param sdu            Pointer to beacon payload.
 *
 * @return void
 */
void usr_mlme_beacon_notify_ind(uint8_t BSN,
                                wpan_pandescriptor_t *PANDescriptor,
                                uint8_t PendAddrSpec,
                                uint8_t *AddrList,
                                uint8_t sduLength,
                                uint8_t *sdu)
{
    uint8_t n = 0;
	int8_t i = 0;
    uint8_t no_of_short_addr = WPAN_NUM_SHORT_ADDR_PENDING(PendAddrSpec);
    uint8_t no_of_ext_addr =  WPAN_NUM_EXTENDED_ADDR_PENDING(PendAddrSpec);

    /* send sequence number */
    n = sprintf(rsp_buffer, "+INBCN: %02X", BSN);

    /* Pan descriptor - Address mode */
    n += sprintf(&rsp_buffer[n], ",%02X", PANDescriptor->CoordAddrSpec.AddrMode);

    /* Pan descriptor - Pan Id */
    n += sprintf(&rsp_buffer[n], ",%04X", PANDescriptor->CoordAddrSpec.PANId);

    /* Pan descriptor - Addres based on mode */
    if (PANDescriptor->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_SHORT)
    {
        n += sprintf(&rsp_buffer[n], ",%04X", PANDescriptor->CoordAddrSpec.Addr.short_address);
    }

    else if (PANDescriptor->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_LONG)
    {
        n += sprintf(&rsp_buffer[n], ",");
        n = sprintf_ext_address(&PANDescriptor->CoordAddrSpec.Addr.long_address, n);

    }

    /* Pan descriptor - Logical Channel */
    n += sprintf(&rsp_buffer[n], ",%02X", PANDescriptor->LogicalChannel);

    /* Pan descriptor - Logical Page */
    n += sprintf(&rsp_buffer[n], ",%02X", PANDescriptor->ChannelPage);

    /* Pan descriptor - Superframe order */
    n += sprintf(&rsp_buffer[n], ",%04X", PANDescriptor->SuperframeSpec);

    /* Pan descriptor - GTS Permit */
    n += sprintf(&rsp_buffer[n], ",%02X", PANDescriptor->GTSPermit);

    /* Pan descriptor - Link Quality */
    n += sprintf(&rsp_buffer[n], ",%02X", PANDescriptor->LinkQuality);

    /* Pan descriptor - Time Stamp */
#ifdef ENABLE_TSTAMP
    n += sprintf(&rsp_buffer[n], ",%08lX", PANDescriptor->TimeStamp);
#endif
    /* Pending Address specification */
    n += sprintf(&rsp_buffer[n], ",%02X", PendAddrSpec);

    /* Pending address list - short addresses*/
    for (i = 0; i < (no_of_short_addr * sizeof(uint16_t)); i += 2 )
    {
        n += sprintf(&rsp_buffer[n], ",%04X", (uint16_t)AddrList[i]);
    }
    AddrList = AddrList + (no_of_short_addr * sizeof(uint16_t));

    /* Pending address list - long addresses*/
    while (no_of_ext_addr != 0)
    {
        uint64_t ext_addr = (uint64_t)AddrList;

        n += sprintf(&rsp_buffer[n], ",");
        n = sprintf_ext_address(&ext_addr, n);

        AddrList += 8;
        no_of_ext_addr--;
    }

    /* Payload length */
    n += sprintf(&rsp_buffer[n], ",%02X", sduLength);

    /*  Payload */
    if (sduLength)
    {
        n += sprintf(&rsp_buffer[n], ",");

        for (i = 0; i < sduLength; i++ )
        {
            n += sprintf(&rsp_buffer[n], "%02X", ((uint8_t *)&sdu)[i]);
        }
    }
    /* Send Beacon Notify indication to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);

}
#endif


#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-DISASSOCIATE.confirm.
 *
 * @param status             Result of requested disassociate operation.
 * @param DeviceAddrSpec     Pointer to wpan_addr_spec_t structure for device
 *                           that has either requested disassociation or been
 *                           instructed to disassociate by its coordinator.
 *
 * @return void
 */
void usr_mlme_disassociate_conf(uint8_t status,
                                wpan_addr_spec_t *DeviceAddrSpec)
{
    uint8_t n = 0;

	/* Disassociation Confirm */
    n += sprintf(rsp_buffer, "+DASSOC: %02X", status);
    n += sprintf(&rsp_buffer[n], ",");

    /* Device Address spec - Address mode  */
    n += sprintf(&rsp_buffer[n], "%02X", DeviceAddrSpec->AddrMode);
    n += sprintf(&rsp_buffer[n], ",");

    /* Device Address spec - Pan Id  */
    n += sprintf(&rsp_buffer[n], "%02X", DeviceAddrSpec->PANId);
    n += sprintf(&rsp_buffer[n], ",");

    /* Device Address spec - Address based on mode  */
    if (DeviceAddrSpec->AddrMode == WPAN_ADDRMODE_SHORT)
    {
        n += sprintf(&rsp_buffer[n], "%04X", DeviceAddrSpec->Addr.short_address);
    }
    else if (DeviceAddrSpec->AddrMode == WPAN_ADDRMODE_LONG)
    {
        n = sprintf_ext_address(&DeviceAddrSpec->Addr.long_address, n);
    }
    /* Send Disassocition Confirm to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);

}
#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(DOXYGEN) */


#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-DISASSOCIATE.indication.
 *
 * @param DeviceAddress        Extended address of device which initiated the
 *                             disassociation request.
 * @param DisassociateReason   Reason for the disassociation. Valid values:
 *                           - @ref WPAN_DISASSOC_BYPARENT,
 *                           - @ref WPAN_DISASSOC_BYCHILD.
 *
 * @return void
 */
void usr_mlme_disassociate_ind(uint64_t DeviceAddress,
                               uint8_t DisassociateReason)
{
    uint8_t n;

    /* Disassociation Indication */
    n =  sprintf(rsp_buffer, "+INDASSOC: ");

    /* Device long Address */
    n = sprintf_ext_address(&DeviceAddress, n);

    n += sprintf(&rsp_buffer[n], ",");

    /* Disassociation reason */
    sprintf(&rsp_buffer[n], "%02X", DisassociateReason);

    /* Send Disassociation indiactaion to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) || defined(DOXYGEN) */

#if (MAC_ORPHAN_INDICATION_RESPONSE == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-ORPHAN.indication.
 *
 * @param OrphanAddress     Address of orphaned device.
 *
 * @return void
 */
void usr_mlme_orphan_ind(uint64_t OrphanAddress)
{

    uint8_t n;

    /* Orphan Indication */
    n =  sprintf(rsp_buffer, "+INORPHAN: ");

    /* Orphan Address */
    n = sprintf_ext_address(&OrphanAddress, n);

    /* Send Prphan indication to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);

}
#endif  /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) || defined(DOXYGEN) */


#if (MAC_INDIRECT_DATA_BASIC == 1) || defined(DOXYGEN)
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
    /* Send Poll confirm to serial interface */
    sprintf(rsp_buffer, "+POLL: %02X", status);
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) || defined(DOXYGEN) */



#if (MAC_RX_ENABLE_SUPPORT == 1) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-RX-ENABLE.confirm.
 *
 * @param status           Result of requested receiver enable operation.
 *
 * @return void
 */
void usr_mlme_rx_enable_conf(uint8_t status)
{
    /* Send Rx Enable confirm to serial interface */
    sprintf(rsp_buffer, "+RXEN: %02X", status);
    send_response(CMD_RESULT_OK, rsp_buffer);
}
#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) || defined(DOXYGEN) */


#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      || \
     (MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
     (MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
     (MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1)) || defined(DOXYGEN)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-SCAN.confirm.
 *
 * @param status            Result of requested scan operation.
 * @param ScanType          Type of scan performed.
 * @param ChannelPage       The channel page on which the scan was performed.
 * @param UnscannedChannels Bitmap of unscanned channels
 * @param ResultListSize    Number of elements in ResultList.
 * @param ResultList        Pointer to array of scan results .
 *
 * @return void
 */
void usr_mlme_scan_conf(uint8_t status,
                        uint8_t ScanType,
                        uint8_t ChannelPage,
                        uint32_t UnscannedChannels,
                        uint8_t ResultListSize,
                        void *ResultList)
{
    int8_t i = 0;

    /* Scan Confirm*/
    uint8_t n = sprintf(rsp_buffer, "+SCAN: ");

    /*Status */
    n += sprintf(&rsp_buffer[n], "%02X", status);
    n += sprintf(&rsp_buffer[n], ",");

    /* Scan Type */
    n += sprintf(&rsp_buffer[n], "%02X", ScanType);
    n += sprintf(&rsp_buffer[n], ",");

    /* Channel page */
    n += sprintf(&rsp_buffer[n], "%02X", ChannelPage);
    n += sprintf(&rsp_buffer[n], ",");

    /* Unscanne dchannels */
    n += sprintf(&rsp_buffer[n], "%08lX", UnscannedChannels);
    n += sprintf(&rsp_buffer[n], ",");

    /* Size of result list  */
    n += sprintf(&rsp_buffer[n], "%02X", ResultListSize);

    /* List of Pan descriptors */
    wpan_pandescriptor_t *wpan_desc_ptr = (wpan_pandescriptor_t *)ResultList;

    while (i < ResultListSize )
    {
        /* Pan descriptor - Addres mode */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->CoordAddrSpec.AddrMode);
        /* Pan descriptor - Pan Id */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->CoordAddrSpec.PANId);
        /* Pan descriptor - address based on mode*/
        if (wpan_desc_ptr->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_SHORT)
        {
            n += sprintf(&rsp_buffer[n], ",%04X", wpan_desc_ptr->CoordAddrSpec.Addr.short_address);
        }
        else if (wpan_desc_ptr->CoordAddrSpec.AddrMode == WPAN_ADDRMODE_LONG)
        {
            n += sprintf(&rsp_buffer[n], ",");
            n = sprintf_ext_address(&wpan_desc_ptr->CoordAddrSpec.Addr.long_address, n);

        }
        /* Pan descriptor - Logical channel */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->LogicalChannel);
        /* Pan descriptor - Channel page */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->ChannelPage);
        /* Pan descriptor - Superframe Specification */
        n += sprintf(&rsp_buffer[n], ",%04X", wpan_desc_ptr->SuperframeSpec);
        /* Pan descriptor - GTS permit */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->GTSPermit);
        /* Pan descriptor - link Qulaity  */
        n += sprintf(&rsp_buffer[n], ",%02X", wpan_desc_ptr->LinkQuality);

        wpan_desc_ptr++;
        i++;
    }
    /* Send Scan confirm to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);

}
#endif/* end */


#if (MAC_SYNC_LOSS_INDICATION == 1)
/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MLME-SYNC-LOSS.indication.
 *
 * @param LossReason     Reason for synchronization loss.
 * @param PANId          The PAN identifier with which the device lost
 *                       synchronization or to which it was realigned.
 * @param LogicalChannel The logical channel on which the device lost
 *                       synchronization or to which it was realigned.
 * @param ChannelPage    The channel page on which the device lost
 *                       synchronization or to which it was realigned.
 *
 * @return void
 */
void usr_mlme_sync_loss_ind(uint8_t LossReason,
                            uint16_t PANId,
                            uint8_t LogicalChannel,
                            uint8_t ChannelPage)
{
    /* Sync loss indication */
    uint8_t n = sprintf(rsp_buffer, "+INSYNL: ");
    /* Reason for the sync loass */
    n += sprintf(&rsp_buffer[n], "%02X", LossReason);
    n += sprintf(&rsp_buffer[n], ",");

    /* Pan Id */
    n += sprintf(&rsp_buffer[n], "%04X", PANId);
    n += sprintf(&rsp_buffer[n], ",");

    /* Logical channel*/
    n += sprintf(&rsp_buffer[n], "%02X", LogicalChannel);
    n += sprintf(&rsp_buffer[n], ",");

    /* Channel page  */
    n += sprintf(&rsp_buffer[n], "%02X", ChannelPage);

    /* Send sync loss indication to serial interface*/
    send_response(CMD_RESULT_OK, rsp_buffer);

}
#endif /* MAC_SYNC_LOSS_INDICATION */


/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MCPS-DATA.confirm.
 *
 * @param msduHandle  Handle of MSDU handed over to MAC earlier.
 * @param status      Result for requested data transmission request.
 * @param Timestamp   The time, in symbols, at which the data were transmitted.
 *
 * @return void
 */
void usr_mcps_data_conf(uint8_t msduHandle,
                        uint8_t status,
                        uint32_t Timestamp)
{
    uint8_t n;
    /* Data confirm Notification */
    n = sprintf(rsp_buffer, "+DATA: ");

    /* Msdu Handle*/
    n += sprintf(&rsp_buffer[n], "%02X", msduHandle);

    /* Confirm Status */
    n += sprintf(&rsp_buffer[n], ",%02X", status);

    /* Time Stamp parameter*/
    n += sprintf(&rsp_buffer[n], ",%08lX", Timestamp);

    /* Send data confirm to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);
}


/**
 * Callback function that must be implemented by application (NHLE) for MAC service
 * MCPS-DATA.indication.
 *
 * @param SrcAddrSpec      Pointer to source address specification.
 * @param DstAddrSpec      Pointer to destination address specification.
 * @param msduLength       Number of octets contained in MSDU.
 * @param msdu             Pointer to MSDU.
 * @param mpduLinkQuality  LQI measured during reception of the MPDU.
 * @param DSN              The DSN of the received data frame.
 * @param Timestamp        The time, in symbols, at which the data were received.
 *
 * @return void
 */
void usr_mcps_data_ind(wpan_addr_spec_t *SrcAddrSpec,
                       wpan_addr_spec_t *DstAddrSpec,
                       uint8_t msduLength,
                       uint8_t *msdu,
                       uint8_t mpduLinkQuality,
                       uint8_t DSN,
                       uint32_t Timestamp)
{
    int8_t i = 0;
    /* MCPS data Indication Notification*/
    uint8_t n = sprintf(rsp_buffer, "+INDATA: ");

    /* Src address specification  - Address Mode*/
    n += sprintf(&rsp_buffer[n], "%02X", SrcAddrSpec->AddrMode);
    /* Src address specification - Pan Id */
    n += sprintf(&rsp_buffer[n], ",%04X", SrcAddrSpec->PANId);
    /* Src address specification - Addr based on mode */
    if (SrcAddrSpec->AddrMode == WPAN_ADDRMODE_SHORT)
    {
        n += sprintf(&rsp_buffer[n], ",%04X", SrcAddrSpec->Addr.short_address);
    }
    else if (SrcAddrSpec->AddrMode == WPAN_ADDRMODE_LONG)
    {
        n += sprintf(&rsp_buffer[n], ",");
        n = sprintf_ext_address(&SrcAddrSpec->Addr.long_address, n);

    }
    /* Dest address specification */

    /* Dest address specification - Address Mode */
    n += sprintf(&rsp_buffer[n], ",%02X", DstAddrSpec->AddrMode);
    /* Dest address specification - Pan Id */
    n += sprintf(&rsp_buffer[n], ",%04X", DstAddrSpec->PANId);
    /* Dest address specification - Addr based on mode */
    if (DstAddrSpec->AddrMode == WPAN_ADDRMODE_SHORT)
    {
        n += sprintf(&rsp_buffer[n], ",%04X", DstAddrSpec->Addr.short_address);
    }
    else if (DstAddrSpec->AddrMode == WPAN_ADDRMODE_LONG)
    {
        n += sprintf(&rsp_buffer[n], ",");
        n = sprintf_ext_address(&DstAddrSpec->Addr.long_address, n);

    }

    /* msdu length */
    n += sprintf(&rsp_buffer[n], ",%02X", msduLength);

    /* msdu */
    for (i = 0; i < msduLength; i++)
    {
        n += sprintf(&rsp_buffer[n], ",%02X", msdu[i]);
    }

    /* Link Quality */
    n += sprintf(&rsp_buffer[n], ",%02X", mpduLinkQuality);

    /* data sequence number */
    n += sprintf(&rsp_buffer[n], ",%02X", DSN);

    /* Time stamp */
    n += sprintf(&rsp_buffer[n], ",%08lX", Timestamp);

    /* Send mcps data indication to serial interface */
    send_response(CMD_RESULT_OK, rsp_buffer);
}

/**
 * Briefing Sprintf the ieee address
 */

uint8_t sprintf_ext_address(uint64_t* ext_address,uint8_t currentPos)
{
    int8_t i;
    for (i = sizeof(uint64_t) - 1; i >= 0 ; i--)
    {
        currentPos += sprintf(&rsp_buffer[currentPos], "%02X", ((uint8_t *)ext_address)[i]);
    }
    return currentPos;

}

/*****************************************************************************/

/* EOF */

