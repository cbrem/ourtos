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
 * Spencer Barton (sebarton)
 * Connor Brem (cbrem)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef DEMO_H
#define DEMO_H

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

/*==================================
 * Exernal Globals
 *==================================*/

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

/*==================================
 * Private Functions
 *==================================*/

/* Tasks */

/* pollBtns modifies the mutexDisableBtn and taskEnableBtn variables by polling
 * the current hardware button state
 */
static void pollBtns(void);



#endif