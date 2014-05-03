/* Demo of kronOS
 *  This file demonstrates the functionality of kronOS. kronOS is a lightweight 
 *  premtive multitasking RTOS with priority ceiling. 
 * 
 *  In the demo a series of tasks run and share a resource. The scheduler prints
 *  out the task state over serial whenever it runs. A watchdog task is 
 *  implemented as well as a poll button task.
 * 
 *  See documentation for further details
 * 
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef _DEMO_H
#define _DEMO_H

/*==================================
 * Includes
 *==================================*/

#include <hidef.h>  
#include "derivative.h"
// TODO include?
// #pragma LINK_INFO DERIVATIVE "mc9s12c128"

#include "inttypes_MC9S12C128.h"
#include "boolean.h"
#include "kronOS.h"

/*==================================
 * MACROS
 *==================================*/

/* Assorted */
#define N_TASKS (4)
/* some tasks are enabled by hardware switches */
#define N_ENABLEABLE_TASKS (4)
#define CYCLES_PER_MS (200)

/* Watchdog */
#define WATCHDOG_PERIOD         (0x7) /* 2^24 cycles ~ 1 sec for 8MHz Clk */
#define SET_WATCHDOG_FLGS(id)   (watchdogFlags |= (1 << id))
#define CLEAR_WATCHDOG_FLGS()   (watchdogFlags = 0)
#define WATCHDOG_FLG_ALL_SET    (0x7) /* 3 tasks kick the watchdog so one flag bit per task */

/* task IDs */
#define ID_POLL_BTN     (0)
#define ID_SHORT_BLK    (1)
#define ID_LONG_BLK     (2)

/* task periods (in msec) */
#define PERIOD_50_MSEC     50
#define PERIOD_100_MSEC    100
#define PERIOD_250_MSEC    250
#define PERIOD_500_MSEC    500
#define PERIOD_1000_MSEC   1000
#define PERIOD_1900_MSEC   1900
#define PERIOD_2000_MSEC   2000
#define PERIOD_5000_MSEC   5000
#define PERIOD_10000_MSEC  10000

/* task timing - see documentation for reasoning */
#define SCHEDULER_PERIOD        (PERIOD_100_MSEC)
#define POLL_BTN_TASK_PERIOD    (PERIOD_500_MSEC)
#define WATCHDOG_TASK_PERIOD    (PERIOD_500_MSEC)
#define SHORT_TASK_PERIOD       (PERIOD_2000_MSEC)
#define LONG_TASK_PERIOD        (PERIOD_1900_MSEC)

/* task blocking time (in Msec) */
#define LONG_BLOCK_TIME     (300)
#define SHORT_BLOCK_TIME    (100)

/* hardware pins and switches */
#define SW3_MASK    (0x0F)
#define LED_MASK    (0xF0)

/* set SW1 on Port P as 0 to enable input with pull-up*/
#define SET_MUTEX_DISABLE_BTN_INPUT() (DDRP = 0); \
                                      (PERP = 0xFF) 
#define GET_MUTEX_DISABLE_BTN() ( (~PTP) & 1 )

/* 
 * set bits of SW3 on PORT B as 0 to enable input
 * also set pull-up resistors to work properly with CPU module
 * see MC9S12C128V1 data sheet section 4.3.2.10
 */
#define SET_TASK_ENABLE_BTN_INPUT() (DDRB &= (~SW3_MASK)); \
                                  (PUCR_PUPBE = 1)
#define GET_TASK_ENABLE_BTN(BTN_N) (PORTB & (1 << BTN_N))

/* LEDs */
#define SET_LEDS_OUTPUT() (DDRB |= LED_MASK) 
#define SET_LEDS(val) (PORTB &= ~LED_MASK); \
                      (PORTB |= (~val << BYTE_LEN_BITS) & LED_MASK )
#define GET_LEDS() ( (PORTB >> BYTE_LEN_BITS) & (LED_MASK >> BYTE_LEN_BITS) )                        

/* Assorted LED light pattens  - 1 is ON, 0 is OFF */
#define LEDS_OFF (0x0)
#define LED_WATCHDOG    (0xF) /* 1111 */ 
#define LED_POLL_BTN    (0x2) /* 0010 */ 
#define LED_SHORT_BLK   (0x4) /* 0100 */ 
#define LED_LONG_BLK    (0x8) /* 1000 */ 

/*==================================
 * Exernal Globals
 *==================================*/

// TODO tasks
/* mutex for demo purposes only - it does not actual control any resource */
 mutex_t blockingMutex;

/*==================================
 * Local Globals
 *==================================*/

/* Button to disable mutexs */
static bool_t mutexDisableBtn;
/* Switches to enable/disable some tasks */
static bool_t taskEnableBtn[N_ENABLEABLE_TASKS];

// TODO PCB
static uint8_t watchdogFlags;

/*==================================
 * Public Functions
 *==================================*/

/*
 * main is where the program starts
 * This function initializes the demo and RTOS and then loops indefinaitly
 * Exiting this function would be an error
 */
void main(void);

/* ------ Tasks ------ */

/* 
 * pollBtnsTask modifies the mutexDisableBtn and taskEnableBtn variables by polling
 *  the current hardware button state
 */
void pollBtnsTask(void);

/* 
 * watchdogKickTask kicks the watchdog periodically
 *  Note: the watchdog is called the COP for our board
 */
void watchdogKickTask(void);

/* 
 * shortBlockingTask grabs a mutex and holds it for a short time
 */
void shortBlockingTask(void);

/* 
 * longBlockingTask grabs a mutex and holds it for a long time
 */
void longBlockingTask(void);

/*==================================
 * Private Functions
 *==================================*/

/* ------ Initialization ------ */

/* 
 * initBtns initializes the btns for pollBtnsTask 
 * - sets mutexDisableBtn and taskEnableBtn to defaults of False
 * - enables mutex diable btn as input
 * - enables switches for tasks as input
 */
void _initBtns(void);

/* 
 * initLEDs enables the LEDs and starts them OFF
 */
void _initLEDs(void);

/*
 * initWatchdog initializes the watchdog with the following settings:
 * - normal mode
 * - run in BDM mode
 * - timer rate set by macro WATCHDOG_PERIOD
 */
void _initWatchdog(void);

/* ------ Helper functions ------*/

/* _blockingDelayMsec runs for the given number of milliseconds 
 * This function is implemented with nop loops so times are approximate
 */
// TODO could we do this with another timer?
static void _blockingDelayMsec(uint16_t delayMS);

#endif