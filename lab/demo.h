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

#define N_TASKS
/* some tasks are enabled by hardware switches */
#define N_ENABLEABLE_TASKS (4)

// TODO task/watchdog timing

// TODO hardware pin defs

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

/* 
 * pollBtns modifies the mutexDisableBtn and taskEnableBtn variables by polling
 *  the current hardware button state
 */
void pollBtns(void);

/* 
 * watchdogKick kicks the watchdog periodically
 */
void watchdogKick(void);

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

/* -- Helper functions -------------*/

/* blockingDelay runs for the given number of milliseconds 
 * This function is implemented with nop loops so times are approximate
 */
static blockingDelay(int delayMS);



#endif