/**
 * @file app_frame_format.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_frame_format.h 28841 2011-10-11 06:16:02Z v_prasad.anjangi $
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
#ifndef APP_FRAME_FORMAT_H
#define APP_FRAME_FORMAT_H

/* === Includes ============================================================= */
#include "app_op_mode_2.h"

/* === Macros =============================================================== */

/* === Types ================================================================ */

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t u8OpMode;
    uint16_t u16nwk_addr;
}pairing_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t u8OpMode;
    uint16_t u16nwk_addr;
}pairing_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint16_t u16nwk_addr;
}pairing_conf_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint32_t pkt_count;
}data_pkt_op_mode_1_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    parameters_t parmameter;
    uint8_t value;
}set_parm_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t Cmd;
}result_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint32_t num_of_frames_rx;
    uint32_t lqi_avrg_rx;
    uint32_t rssi_avrg_rx;
    uint32_t frames_with_wrong_crc;
}result_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}div_stat_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status    :1;
    uint8_t ant_sel   :2;
}div_stat_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status    :1;
    uint8_t ant_sel   :2;
}div_set_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}div_set_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}crc_stat_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}crc_stat_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}crc_set_req_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t status;
}crc_set_rsp_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef union
{
    pairing_req_t           pairing_req_data;
    pairing_rsp_t           pairing_rsp_data;
    pairing_conf_t          pairing_conf_data;
    data_pkt_op_mode_1_t    data_payload;
    set_parm_req_t          set_parm_req_data;
    result_rsp_t            test_result_rsp_data;
    div_stat_rsp_t          div_stat_rsp_data;
    div_set_req_t           div_set_req_data;
    crc_stat_rsp_t          crc_stat_rsp_data;
    crc_set_req_t           crc_set_req_data;
}general_pkt_u;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack(1)
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/
typedef struct
{
    uint8_t u8CmdID;
    uint8_t u8SeqNum;
    general_pkt_u payload[1];
}app_payload_t;
#if ((defined __ICCARM__) || (defined __ICCAVR32__) ||(defined __GNUARM__)|| (defined __GNUAVR32__))
#pragma pack()
#endif /* __ICCARM__, __ICCAVR32__, __GNUARM__ , __GNUAVR32__*/

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_FRAME_FORMAT_H */
/* EOF */
