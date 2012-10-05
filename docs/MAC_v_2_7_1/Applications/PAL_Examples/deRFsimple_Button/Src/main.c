/**
 * @file main.c
 *
 * @addtogroup macExamples
 * @{
 * @addtogroup macExamples_deRFsimple_Button deRFsimple_Button
 * @{
 *
 * @brief  Demo application showing GPIO access using the PAL layer
 *
 * The PAL Example "deRFsimple_Button" demonstrates access to IO resources
 * (here Button and LEDs) of the following platforms:
 *    - Radio Controller Board
 *    - Sensor Terminal Board
 *    - Breakout Board
 *    - deRFtoRCB Adapter
 *    - deRFnode
 *    - deRFgateway
 *
 * by the help of PAL layer.
 *
 * The example code toggles LED_0 and LED_1 every time a button is pressed.
 *  
 * $Id: main.c,v 1.1.8.1 2011/07/14 18:57:22 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 */
/*
 * Copyright (c) 2010, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pal.h"
#include "app_config.h"
#include "deRFsimple_Button.h"

/* === TYPES =============================================================== */

/* === MACROS / DEFINES ==================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */
static uint8_t app_debounce_button(void);
static void app_init(void);
static void app_task(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the application
 */
int main(void)
{
   /* initialize hardware */
   app_init();

   /* start application */
   while(1)
   {
      app_task();
   }
}

/**
 * @brief Button debounce routine.
 *
 * Helper function for debouncing the button.
 * @return 1 if a button event is detected, 0 otherwise.
 *
 */
static uint8_t app_debounce_button(void)
{
   static uint8_t keycnt;

   if(BUTTON_PRESSED == pal_button_read(BUTTON_0))
   {
      keycnt ++;
      if (keycnt > KEY_UP_COUNT)
      {
         keycnt = 0;
         return BUTTON_PRESSED;
      }
   }
   else
   {
      keycnt = 0;
   }
   return BUTTON_OFF;
}


/**
 * @brief Application initialization function.
 *
 * This function initializes
 *  - the PAL module including the IRQ handler registering,
 *  - the LED and Button interface.
 */
static void app_init(void)
{
   /* Step 0: initialize MCU peripherals */
   pal_init();
   pal_led_init();
   pal_button_init();

   /* Step 1: switch one LED on */
   pal_led(LED_0, LED_ON);
   pal_led(LED_1, LED_OFF);

   /* Step 2: ready to receive data. */
   pal_global_irq_enable();
}


/**
 * @brief Application task function.
 *
 * This function checks the state of button and starts 
 * to toggle LED's if a button press event is detected.
 */
static void app_task(void)
{
   if(BUTTON_PRESSED == app_debounce_button())
   {
      /* wait until button released */
      while(BUTTON_PRESSED == pal_button_read(BUTTON_0)){;}
      pal_led(LED_0, LED_TOGGLE);
      pal_led(LED_1, LED_TOGGLE);
   }
}


/** @}*/
/** @}*/

/* EOF */
