/**
 * @file vendor_boardtypes.h
 *
 * @brief vendor-specific PAL board types for AT91SAM7X512
 *
 * This header file contains vendor-specific board types
 * based on AT91SAM7X512.
 *
 * $Id: vendor_boardtypes.h,v 1.1.6.3 2011/07/14 15:50:50 dam Exp $
 *
 * @author
 *      dresden elektronik: http://www.dresden-elektronik.de
 *      Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, dresden elektronik All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* Prevent double inclusion */
#ifndef VENDOR_BOARDTYPES_H
#define VENDOR_BOARDTYPES_H

/* === Macros =============================================================== */


/*
 * dresden elektronik deRFarm7-15X00 radio module series (Sub-GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * on dresden elektronik deRFnode_1XXX2 development board
 * (http://www.dresden-elektronik.de/shop/prod142.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * RADIO_MODULE_REV00, BOARD_REV00 and BOARD_REV01 may defined in Makefile
 * to fit these internal releases.
 */
#define deRFarm7_15X00_deRFnode_1XXX2               (0x80)

/*
 * dresden elektronik deRFarm7-15X00 radio module series (Sub-GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * on dresden elektronik deRFgateway_1XXX2 development board
 * (http://www.dresden-elektronik.de/shop/prod143.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * RADIO_MODULE_REV00, BOARD_REV0 and BOARD_REV1 may defined in Makefile to fit
 * these internal releases.
 */
#define deRFarm7_15X00_deRFgateway_1XXX2            (0x81)

/*
 * dresden elektronik deRFarm7-15X00 radio module series (Sub-GHz,
 * SMD/THT,internal/external antenna
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * onto dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * RADIO_MODULE_REV00 may defined in Makefile to fit these internal releases.
 */
#define deRFarm7_15X00_deRFtoRCB                    (0x82)

/*
 * dresden elektronik deRFarm7-15X00 radio module series (Sub-GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Breakout Board
 * (http://www.dresden-elektronik.de/shop/prod85.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * RADIO_MODULE_REV00 may defined in Makefile to fit these internal releases.
 */
#define deRFarm7_15X00_deRFtoRCB_BREAKOUT_BOARD     (0x83)

/*
 * dresden elektronik deRFarm7-15X00 radio module series (Sub-GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Sensor Terminal Board
 * (http://www.dresden-elektronik.de/shop/prod75.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * RADIO_MODULE_REV00 may defined in Makefile to fit these internal releases.
 */
#define deRFarm7_15X00_deRFtoRCB_SENS_TERM_BOARD    (0x84)

/*
 * dresden elektronik deRFarm7-25X00 radio module series (2.4 GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * on dresden elektronik deRFnode_1XXX2 development board
 * (http://www.dresden-elektronik.de/shop/prod142.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * BOARD_REV0 and BOARD_REV1 may defined in Makefile to fit these internal
 * releases.
 */
#define deRFarm7_25X00_deRFnode_1XXX2               (0x85)

/*
 * dresden elektronik deRFarm7-25X00 radio module series (2.4 GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * on dresden elektronik deRFgateway_1XXX2 development board
 * (http://www.dresden-elektronik.de/shop/prod143.html?language=en)
 *
 * By default it already uses the latest revisions, for development usage
 * BOARD_REV0 and BOARD_REV1 may defined in Makefile to fit these internal
 * releases.
 */
#define deRFarm7_25X00_deRFgateway_1XXX2            (0x86)

/*
 * dresden elektronik deRFarm7-25X00 radio module series (2.4 GHz,
 * SMD/THT,internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * onto dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 */
#define deRFarm7_25X00_deRFtoRCB                    (0x87)

/*
 * dresden elektronik deRFarm7-25X00 radio module series (2.4 GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Breakout Board
 * (http://www.dresden-elektronik.de/shop/prod85.html?language=en)
 */
#define deRFarm7_25X00_deRFtoRCB_BREAKOUT_BOARD     (0x88)

/*
 * dresden elektronik deRFarm7-25X00 radio module series (2.4 GHz,
 * SMD/THT, internal/external antenna)
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Sensor Terminal Board
 * (http://www.dresden-elektronik.de/shop/prod75.html?language=en)
 */
#define deRFarm7_25X00_deRFtoRCB_SENS_TERM_BOARD    (0x89)



#endif  /* VENDOR_BOARDTYPES_H */
/* EOF */
