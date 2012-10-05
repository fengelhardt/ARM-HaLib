/**
 * @file vbatmon.h
 *
 * @brief header file for vbatmon.c.
 *
 * $Id: vbatmon_interface.h,v 1.1.4.1 2011/07/08 16:59:08 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* prevent double inclusion */
#ifndef _VBATMON_H
#define _VBATMON_H

/* === INCLUDES ============================================================ */
#include <inttypes.h>

/* === MACROS / DEFINES ==================================================== */

/* === TYPES =============================================================== */

/* === PROTOTYPES ========================================================== */
void Batmon_GetBatteryVoltage(uint16_t* voltage);

/* === INLINE FUNCTIONS ==================================================== */


#endif /* _VBATMON_H */

/* EOF */
