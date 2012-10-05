/**
 * @file cmd_handler.c
 *
 * @brief handlers for the various commands
 *
 * $Id: cmd_handler.c 28613 2011-09-20 06:56:06Z viswanadham.kotla $
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

#include <stdint.h>
#include <stdio.h>
#include "cmd_parser.h"
#include "mac_api.h"
#include "mac_internal.h"

/* === MACROS ============================================================== */


/* === Globals ============================================================= */

/*
 * Enables/disables automatic sending of association response when a device
 * tries to associate
 */
uint8_t auto_assoc = true;

/*
 * Short address to allocate to the device that tries to associate, when
 * automatic sending of association response is enabled (will be incremented
 * by one after sending the response, in case the value is less that 0xfffe)
 */
uint16_t assoc_short_addr = 0xfffe;


/* Buffer for storing responses */
extern char rsp_buffer[];

/* === Prototypes ==========================================================*/


/* === Implementation ======================================================*/

/**
 * @brief Handles MAC reset command
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_reset_handler(command_table_t *table_entry,
                                   char **cmd_line,
                                   cmd_response_cb_t response_cb)
{
    int value = get_number(cmd_line);
    if (wpan_mlme_reset_req(value))
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;


}

/**
 * @brief Handles MAC PIB Set/Get Commands
 *
 * This function checks the PIB number of the PIB Set/Get command and invokes
 * the corresponding MAC primitive
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_pib_handler(command_table_t *table_entry,
                                 char **cmd_line,
                                 cmd_response_cb_t response_cb)
{
    int pib = get_hex_number(cmd_line), i;
    int size = mac_get_pib_attribute_size(pib);
    static uint8_t pib_value[aMaxBeaconPayloadLength];
    command_type_t type;

    type = get_cmd_type(cmd_line);

    /* If PIB is invalid, return error */
    if (size == 0)
    {
        return CMD_RESULT_ERROR;
    }

    switch (type)
    {
        case CMD_TYPE_SET:
            /* PIB Set Command */
            for (i = 0; i < size; i++)
            {
                pib_value[i] = get_hex_byte(cmd_line);
            }
            wpan_mlme_set_req(pib, pib_value);
            break;

        case CMD_TYPE_GET:
            /* PIB Get Command */
            wpan_mlme_get_req(pib);
            break;

        case CMD_TYPE_TEST:
            /* Command is valid, so return ok */
            return CMD_RESULT_OK;

        default:
            return CMD_RESULT_ERROR;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_PENDING;
}


/**
 * @brief Handles auto address allocation setting command
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t auto_addr_alloc_handler (command_table_t *table_entry,
                                          char **cmd_line,
                                          cmd_response_cb_t response_cb)
{
    int auto_addr;
    int sh_addr;
    char *cmd_ptr = *cmd_line;

    auto_addr = get_number(cmd_line);

    if (cmd_ptr == *cmd_line)
    {
        /* No argument so must be a query */
        sprintf(rsp_buffer, "%s: %d, %04X", table_entry->cmd,
                auto_assoc, assoc_short_addr);
        response_cb(CMD_RESULT_PENDING, rsp_buffer);
        return CMD_RESULT_OK;
    }

    if (!get_next_arg(cmd_line))
    {
        auto_assoc = auto_addr;
        return CMD_RESULT_OK;
    }

    /* Get the short address */
    sh_addr = get_hex_number(cmd_line);
    if ( (sh_addr < 1) || (sh_addr > 0xfffe) )
    {
        return CMD_RESULT_ERROR;
    }

    auto_assoc = auto_addr;
    assoc_short_addr = sh_addr;

    return CMD_RESULT_OK;
}

/**
 * @brief Handles sending association request
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_assoc_handler (command_table_t *table_entry,
                                    char **cmd_line,
                                    cmd_response_cb_t response_cb)
{
    wpan_addr_spec_t coord_addr;
    uint8_t channel;
    uint8_t channel_page;
    uint8_t capability_info;
    uint8_t coord_ext_addr[8];
    int i;

    channel = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    channel_page = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    coord_addr.AddrMode = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    coord_addr.PANId = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    for (i = 0; i < sizeof(uint64_t); i++)
    {
        coord_ext_addr[i] = get_hex_byte(cmd_line);
    }

    memcpy(&coord_addr.Addr.long_address, &coord_ext_addr, sizeof(uint64_t));

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    capability_info = get_hex_number(cmd_line);

    if (wpan_mlme_associate_req(channel,
                                channel_page,
                                &coord_addr,
                                capability_info))
    {
        return CMD_RESULT_PENDING;
    }
    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;
}

/**
 * @brief Handles sending of association response
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_assoc_rsp_handler (command_table_t *table_entry,
                                        char **cmd_line,
                                        cmd_response_cb_t response_cb)
{
    uint64_t device_addr;
    uint16_t sh_addr;
    uint8_t status;
    int i;

    for (i = 0; i < sizeof(device_addr); i++)
    {
        ((uint8_t *)&device_addr)[i] = get_hex_byte(cmd_line);
    }

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    sh_addr = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    status = get_hex_number(cmd_line);

    if ( wpan_mlme_associate_resp(device_addr, sh_addr, status) )
    {
        return CMD_RESULT_PENDING;
    }
    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;
}
/**
 * @brief Handles MLME Start
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_start_handler (command_table_t *table_entry,
                                    char **cmd_line,
                                    cmd_response_cb_t response_cb)
{
    uint16_t PANId;
    uint8_t Channel;
    uint8_t ChannelPage;
    uint8_t BeaconOrder;
    uint8_t SuperframeOrder;
    bool PAN_Coord;
    bool BatteryLifeExt;
    bool CoordRealign;

    PANId = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    Channel = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    ChannelPage = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    BeaconOrder = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    SuperframeOrder = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    PAN_Coord = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    BatteryLifeExt = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    CoordRealign = get_hex_number(cmd_line);

    if ( wpan_mlme_start_req(PANId, Channel, ChannelPage, BeaconOrder,
                             SuperframeOrder, PAN_Coord, BatteryLifeExt,
                             CoordRealign) )
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;
}

/**
 * @brief Handles MCPS Data req
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_data_req_handler(command_table_t *table_entry,
                                      char **cmd_line,
                                      cmd_response_cb_t response_cb)
{

    uint8_t src_addr_mode;
    wpan_addr_spec_t dst_addr_spec;
    uint8_t msdu_length;
    uint8_t msdu[128];
    uint8_t msdu_handle;
    uint8_t tx_options;
    int i;

    /* get teh src addr mode */
    src_addr_mode = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst address mode */
    dst_addr_spec.AddrMode = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst pan id */
    dst_addr_spec.PANId = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst address */
    if (dst_addr_spec.AddrMode == 0x02)
    {
        dst_addr_spec.Addr.short_address = get_hex_number(cmd_line);
    }
    else if (dst_addr_spec.AddrMode == 0x03)
    {
        for (i = 7; i >= 0 ; i--)
        {
            ((uint8_t *)&dst_addr_spec.Addr.long_address)[i] = get_hex_byte(cmd_line);
        }
    }
    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the msdu length */
    msdu_length = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the msdu */
    for (i = 0; i < msdu_length; i++ )
    {
        msdu[i] = get_hex_byte(cmd_line);
    }

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the msdu handle */
    msdu_handle = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the txoptions */
    tx_options = get_hex_number(cmd_line);

    /* send the mcps data req */
    if ( wpan_mcps_data_req(src_addr_mode,
                            &dst_addr_spec,
                            msdu_length,
                            msdu,
                            msdu_handle,
                            tx_options
                           ) )
    {
        uint8_t c = '\n';
        pal_sio_tx(0, &c, 1);
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;
}


/**
 * @brief Handles Purge req
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_purge_req_handler(command_table_t *table_entry,
                                       char **cmd_line,
                                       cmd_response_cb_t response_cb)
{
    uint8_t msduHandle;
    /* Get the msdu handle */
    msduHandle = get_hex_number(cmd_line);

    if ( wpan_mcps_purge_req(msduHandle))
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;

}

/**
 * @brief Handles poll req
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_poll_req_handler(command_table_t *table_entry,
                                      char **cmd_line,
                                      cmd_response_cb_t response_cb)
{
    wpan_addr_spec_t dst_addr_spec;
    int8_t i;

    /* get the dst address mode */
    dst_addr_spec.AddrMode = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst pan id */
    dst_addr_spec.PANId = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst address */
    if (dst_addr_spec.AddrMode == WPAN_ADDRMODE_SHORT)
    {
        dst_addr_spec.Addr.short_address = get_hex_number(cmd_line);
    }
    else if (dst_addr_spec.AddrMode == WPAN_ADDRMODE_LONG)
    {
        for (i = sizeof(dst_addr_spec.Addr.long_address); i >= 0 ; i--)
        {
            ((uint8_t *)&dst_addr_spec.Addr.long_address)[i] = get_hex_byte(cmd_line);
        }
    }

    if ( wpan_mlme_poll_req(&dst_addr_spec) )
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;

}

/**
 * @brief Handles synq req
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_synq_req_handler(command_table_t *table_entry,
                                      char **cmd_line,
                                      cmd_response_cb_t response_cb)
{

    uint8_t LogicalChannel;
    uint8_t ChannelPage;
    bool TrackBeacon;

    /* get the logical channel */
    LogicalChannel = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the channel page */
    ChannelPage = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* track beacon */
    TrackBeacon = get_hex_number(cmd_line);

    if ( wpan_mlme_sync_req(LogicalChannel,
                            ChannelPage,
                            TrackBeacon) )

    {
        sprintf(rsp_buffer, "+SYNC: ");
        send_response(CMD_RESULT_OK, rsp_buffer);
        return CMD_RESULT_OK;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;

}

/**
 * @brief Handles Rx ebnable req
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_rx_enable_req_handler(command_table_t *table_entry,
                                           char **cmd_line,
                                           cmd_response_cb_t response_cb)
{

    bool DeferPermit;
    uint32_t RxOnTime = 0;
    uint32_t RxOnDuration = 0;
    uint8_t i;

    /* get the DeferPermit parmeter */
    DeferPermit = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get RxOntime parameter */
    for ( i = 0; i < sizeof(RxOnTime); i++ )
    {
        RxOnTime <<= 8;
        RxOnTime |= get_hex_byte(cmd_line);

    }
    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get RxOnDuration parameter  */
    for ( i = 0; i < sizeof(RxOnDuration); i++ )
    {
        RxOnDuration <<= 8;
        RxOnDuration |= get_hex_byte(cmd_line);

    }
    if (wpan_mlme_rx_enable_req( DeferPermit,
                                 RxOnTime,
                                 RxOnDuration) )
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;

}


/**
 * @brief Handles sending Scan request
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_scan_req_handler (command_table_t *table_entry,
                                       char **cmd_line,
                                       cmd_response_cb_t response_cb)
{

    uint8_t ScanType;
    uint32_t ScanChannels;
    uint8_t ScanDuration;
    uint8_t ChannelPage;
    uint8_t i;

    /* get the scan type */
    ScanType = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get scan channel parameter */
    for ( i = 0; i < sizeof(ScanChannels); i++ )
    {
        ScanChannels <<= 8;
        ScanChannels |= get_hex_byte(cmd_line);

    }
    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }

    /* get the scan duration */
    ScanDuration = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the channel page */
    ChannelPage =  get_hex_number(cmd_line);

    if ( wpan_mlme_scan_req( ScanType,
                             ScanChannels,
                             ScanDuration,
                             ChannelPage) )
    {
        uint8_t c = '\n';
        pal_sio_tx(0, &c, 1);
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;

}

/**
 * @brief Handles sending Disassociation request
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t mac_disassoc_handler (command_table_t *table_entry,
                                       char **cmd_line,
                                       cmd_response_cb_t response_cb)
{

    wpan_addr_spec_t DeviceAddrSpec;
    uint8_t DisassociateReason;
    bool TxIndirect;
    int8_t i;

    /* get the address mode */
    DeviceAddrSpec.AddrMode = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the pan id */
    DeviceAddrSpec.PANId = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the dst address */
    if (DeviceAddrSpec.AddrMode == WPAN_ADDRMODE_SHORT)
    {
        DeviceAddrSpec.Addr.short_address = get_hex_number(cmd_line);
    }

    else if (DeviceAddrSpec.AddrMode == WPAN_ADDRMODE_LONG)
    {
        for (i =  sizeof(DeviceAddrSpec.Addr.long_address) - 1; i >= 0 ; i--)
        {
            ((uint8_t *)&DeviceAddrSpec.Addr.long_address)[i] = get_hex_byte(cmd_line);
        }
    }

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the disassoc reason  */
    DisassociateReason = get_hex_number(cmd_line);

    if (!get_next_arg(cmd_line))
    {
        return CMD_RESULT_ERROR;
    }
    /* get the tx method */
    TxIndirect = get_hex_number(cmd_line);

    if ( wpan_mlme_disassociate_req(&DeviceAddrSpec,
                                    DisassociateReason,
                                    TxIndirect
                                   ) )
    {
        return CMD_RESULT_PENDING;
    }

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;

    return CMD_RESULT_ERROR;
}

/* EOF */

