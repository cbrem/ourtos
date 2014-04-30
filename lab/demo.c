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

	/* set mutex disable btn as input */
	SET_MUTEX_DISABLE_BTN_INPUT;
	SET_TASK_ENABLE_BTN_INPUT;

	/* set globals false */
	mutexDisableBtn = FALSE;
	for(i = 0; i < N_ENABLEABLE_TASKS; i++) {
		taskEnableBtn[i] = FALSE;
	}

}

void initWatchdog(uint8_t watchdogPeriod) {

	/* normal mode */
	COPCTL_WCOP = 0;

	/* run in BDM mode */
	COPCTL_RSBCK = 0;

	/* set time period */
	COPCTL_CR = watchdogPeriod;

}

/* ------ Tasks ------ */

void pollBtnsTask(void);

void watchdogKickTask(void);

void shortBlockingTask(void);

void longBlockingTask(void);

/* ------ Interrupt Service Routines ------ */

// TODO watchdog reset vector
/*==================================
 * Private Functions
 *==================================*/

/* ------ Helper functions ------*/

static void _blockingDelay(uint16_t delayMS);