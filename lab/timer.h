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

/*
 * Timer overflow frequencies which this library supports.
 */
// TODO: section for enums?
typedef enum {
    PERIOD_1000_HZ,
    PERIOD_100_HZ,
    PERIOD_10_HZ,
    PERIOD_1_HZ
} period_t;

/*==================================
 * Public Functions
 *==================================*/

/*
 * Configures serial communications, using the given baud prescale.
 */
void timerInit(period_t period);

#endif // _SERIAL_H