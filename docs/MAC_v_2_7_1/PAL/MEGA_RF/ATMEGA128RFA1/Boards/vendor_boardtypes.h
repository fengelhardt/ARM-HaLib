/**
 * @file vendor_boardtypes.h
 *
 * @brief vendor-specific PAL board types for ATmega128RFA1
 *
 * This header file contains vendor-specific board types
 * based on ATmega128RFA1.
 *
 * $Id: vendor_boardtypes.h,v 1.1.6.2 2011/07/14 15:29:33 dam Exp $
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

/*
 * any radio module of deRFmega128-series on dresden elektronik deRFnode
 * with FTDI-based USB (REV01, for internal usage only)
 */
#define  deRFmega128_22X00_deRFnode_2XXX1               (0x80)

/*
 * any radio module of deRFmega128-series
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * on dresden elektronik deRFnode with FTDI-based USB (to be coming soon,
 * see: http://www.dresden-elektronik.de/shop/cat4_22.html?language=en)
 */
#define  deRFmega128_22X00_deRFnode_2XXX2               (0x81)

/*
 * any radio module of deRFmega128-series
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * onto deRFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 */
#define deRFmega128_22X00_deRFtoRCB                     (0x82)

/*
 * any radio module of deRFmega128-series
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Breakout Board
 * (http://www.dresden-elektronik.de/shop/prod85.html?language=en)
 */
#define  deRFmega128_22X00_deRFtoRCB_BREAKOUT_BOARD     (0x83)

/*
 * any radio module of deRFmega128-series
 * (http://www.dresden-elektronik.de/shop/cat4_32.html?language=en)
 * via dresden elektronik RFtoRCB-Adapter
 * (http://www.dresden-elektronik.de/shop/prod126.html?language=en)
 * onto dresden elektronik Sensor Terminal Board
 * (http://www.dresden-elektronik.de/shop/prod75.html?language=en)
 */
#define  deRFmega128_22X00_deRFtoRCB_SENS_TERM_BOARD    (0x84)


#endif  /* VENDOR_BOARDTYPES_H */

/* EOF */
