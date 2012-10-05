/**
 * @file app_op_mode_2.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_op_mode_2.h 28866 2011-10-11 12:00:29Z v_prasad.anjangi $
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
#ifndef APP_OP_MODE_2_H
#define APP_OP_MODE_2_H

/* === Includes ============================================================= */

/* === Macros =============================================================== */

/* === Types ================================================================ */
#define  CHANNEL      0
#define  CHANNEL_PAGE 1
#define  TX_POWER_DBM 2
#if (TAL_TYPE != AT86RF212)
#define  TX_POWER_REG 3
#endif

typedef struct
{
  uint8_t  value;
} parameters_t;

/* === Externals ============================================================ */

/* === Prototypes =========================================================== */

void app_task_op_mode_2_init(startup_mode_e mode,  bool pairing_status);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_OP_MODE_2_H */
/* EOF */
