/* 
 * timer.h - A small timer library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef _TIMER_H
#define _TIMER_H

/*==================================
 * Includes
 *==================================*/

#include <hidef.h>
#include "modclock.h" // Sets bus clock to 8 MHZ.
#include "derivative.h"
#include "inttypes_MC9S12C128.h"

/*==================================
 * Macros
 *==================================*/

#define TIMER_INTERRUPT_VECTOR (16)

// TODO: timer prescale? timer increment? or derive these from Period?

/*==================================
 * Types
 *==================================*/

/*
 * Timer overflow frequencies which this library supports.
 */
typedef enum {
    PERIOD_1000_HZ,
    PERIOD_100_HZ,
    PERIOD_10_HZ,
    PERIOD_1_HZ
} period_t;

/*==================================
 * Local Globals
 *==================================*/

 static uint32_t currentTime;

/*==================================
 * Public Functions
 *==================================*/

/*
 * Configures the timer to overflow with the given period.
 */
void timerInit(period_t period);

/*
 * Gets the time since timerInit was called, in ms.
 *
 * NOTE: This function may yield invalid results if interrupted.
 * Users should ensure that interrupts are disabled when calling it.
 */
uint32_t timerGetCurrent(void);

/*
 * Call this function from within the timer overflow ISR in order to ensure
 * that the current time stays valid.
 */
void timerUpdateCurrent(void);

#endif // _TIMER_H
