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

#include <stdint.h>
#include "boolean.h"
#include "kronOS.h"

/*==================================
 * MACROS
 *==================================*/

#define N_TASKS (4)
/* some tasks are enabled by hardware switches */
#define N_ENABLEABLE_TASKS (3)

// TODO task/watchdog timing

/* hardware pins and switches */
#define SW3_1  (0x01)
#define SW3_2  (0x02)
#define SW3_3  (0x04)
#define SW3_4  (0x08)
#define SW3    (0x0F)

#define SET_MUTEX_DISABLE_BTN_OUTPUT (TODO)
#define MUTEX_DISABLE_BTN (TODO)

/* 
 * set bits of SWI on PORT B as 0 to enable 
 * also set pull-up resistors to work properly with CPU module
 * see MC9S12C128V1 data sheet section 4.3.2.10
 */
#define SET_TASK_ENABLE_BTN_OUTPUT ( DDRB &= (~SW3); \
  									 PUCR |= 0x02;
  								   )
#define GET_TASK_ENABLE_BTN1 (PORTB & SW3_1)
#define GET_TASK_ENABLE_BTN2 (PORTB & SW3_2)
#define GET_TASK_ENABLE_BTN3 (PORTB & SW3_3)
#define GET_TASK_ENABLE_BTN4 (PORTB & SW3_4)
#define GET_TASK_ENABLE_BTNS (PORTB & SW3)

/*==================================
 * Exernal Globals
 *==================================*/

// TODO tasks
// TODO mutex

/*==================================
 * Local Globals
 *==================================*/

/* Button to disable mutexs */
static boolean mutexDisableBtn;
/* Switches to enable/disable some tasks */
static boolean* taskEnableBtn[N_ENABLEABLE_TASKS];

/*==================================
 * Public Functions
 *==================================*/

/* ------ Initialization ------ */

/* 
 * initBtns initializes the btns for pollBtnsTask 
 * - sets mutexDisableBtn and taskEnableBtn to defaults of False
 * - enables mutex diable btn as input
 * - enables switches for tasks as input
 */
void initBtns(void);

/*
 * initWatchdog initializes the watchdog with the following settings:
 * TODO
 */
// TODO settings, may want arguments
void initWatchdog(void);

/* ------ Tasks ------ */

/* 
 * pollBtnsTask modifies the mutexDisableBtn and taskEnableBtn variables by polling
 *  the current hardware button state
 */
void pollBtnsTask(void);

/* 
 * watchdogKickTask kicks the watchdog periodically
 */
void watchdogKickTask(void);

/* 
 * shortBlockingTask grabs a mutex and holds it for a short time
 */
void shortBlockingTask(void);

/* 
 * longBlockingTask grabs a mutex and holds it for a short long
 */
void longBlockingTask(void);

/*==================================
 * Private Functions
 *==================================*/

/* ------ Helper functions ------*/

/* blockingDelay runs for the given number of milliseconds 
 * This function is implemented with nop loops so times are approximate
 */
static void _blockingDelay(int delayMS);



#endif