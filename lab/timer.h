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
#include "inttypesMC9S12C128.h"

/*==================================
 * Macros
 *==================================*/

#define TIMER_INTERRUPT_VECTOR (16)
#define THREE_BIT_MASK (0x7)

/* time prescalers per freq */
#define FREQ_8_MHZ_PRESCALER 	(0)
#define FREQ_4_MHZ_PRESCALER 	(1)
#define FREQ_2_MHZ_PRESCALER 	(2)
#define FREQ_1_MHZ_PRESCALER 	(3)

/* time increments per freq - all are 32 bit
 * for 8.24 fixed point representation 
 */
#define FREQ_8_MHZ_INCR (0x083126E9)
#define FREQ_4_MHZ_INCR (0x10624DD2)
#define FREQ_2_MHZ_INCR (0x20C49BA5)
#define FREQ_1_MHZ_INCR (0x4189374B)

#define TIME_COUNT_N_FRAC_BITS 		(24)
#define TIME_COUNT_MSEC_MASK 		(0xFF000000)
#define TIME_COUNT_FRAC_SEC_MASK 	(0x00FFFFFF)

/*==================================
 * Types
 *==================================*/

/*
 * Timer overflow frequencies which this library supports.
 */
typedef enum {
    FREQ_8_MHZ, 	// .000125 ms
    FREQ_4_MHZ, 	// .00025 ms
    FREQ_2_MHZ, 	// .0005 ms
    FREQ_1_MHZ,		// .001 ms
} freq_t;

/*==================================
 * Exernal Globals
 *==================================*/

/*==================================
 * Public Functions
 *==================================*/

/*
 * Configures the timer to overflow with the given period.
 * Sets the bus clock to run at 8MHz
 */
void timerInit(freq_t freq);

/*
 * Enables the timer overflow interrupt.
 * This is atomic and does not disable interrupts.
 */
void timerEnableInterrupt(void);

/*
 * Disables the timer overflow interrupt.
 * This is atomic and does not disable interrupts.
 */
void timerDisableInterrupt(void);

/*
 * Gets the time since timerInit was called, in ms.
 *
 * NOTE: This function may yield invalid results if interrupted.
 * Users should ensure that interrupts are disabled when calling it.
 */
uint32_t timerGetCurrentMsec(void);

/*
 * Call this function from within the timer overflow ISR in order to ensure
 * that the current time stays valid.
 */
void timerUpdateCurrent(void);

/*
 * timerElapsedTime returns the elapsed time since last calling this function
 * This function relies on _lastTimestamp global. Behavior of this 
 * function is undefined if not sampled frequently enough.
 *
 * NOTE: This function may yield invalid results if interrupted.
 * Users should ensure that interrupts are disabled when calling it.
 */
 // TODO fix undefined behavior
int32_t timerElapsedTime(void);

/*==================================
 * Private Functions
 *==================================*/

/* 
 * _freq2Prescaler returns a 3 bit prescaler value based on the given
 * freq. Calculated values are assumed to be based off of an 8MHz bus clock.
 * Also updates the timeIncrement variable for the given freq.
 */
static uint8_t _freq2Prescaler(freq_t freq);

#endif // _TIMER_H
