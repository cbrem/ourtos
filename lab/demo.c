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

#include "demo.h"

/*==================================
* Public Functions
*==================================*/

/* ------ Initialization ------ */

void initBtns(void) {
	int i;

	/* set globals false */
	mutexDisableBtn = FALSE;
	for(i = 0; i < N_ENABLEABLE_TASKS; i++) {
		taskEnableBtn[i] = FALSE;
	}

}

void initWatchdog(void);

/* ------ Tasks ------ */

void pollBtnsTask(void);

void watchdogKickTask(void);

void shortBlockingTask(void);

void longBlockingTask(void);

/*==================================
 * Private Functions
 *==================================*/

/* ------ Helper functions ------*/

static void _blockingDelay(int delayMS);