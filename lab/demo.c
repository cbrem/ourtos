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

/* ------ Main ------ */

void main(void) {

	_initBtns();
	_initLEDs();
	_initWatchdog();

	for (;;)
	{
		/* infinite loop */

		// TODO temperary
		_FEED_COP(); /* feeds the dog */
	}

}

/* ------ Initialization ------ */

void _initBtns(void) {
	
	int i;

	/* set mutex disable btn as input */
	SET_MUTEX_DISABLE_BTN_INPUT();
	SET_TASK_ENABLE_BTN_INPUT();

	/* set globals false */
	mutexDisableBtn = false;
	for(i = 0; i < N_ENABLEABLE_TASKS; i++) {
		taskEnableBtn[i] = false;
	}

}

void _initLEDs(void) {

	SET_LEDS_OUTPUT();
	SET_LEDS(LEDS_OFF);

}

void _initWatchdog(void) {

	/* normal mode */
	COPCTL_WCOP = 0;

	/* run in BDM mode */
	COPCTL_RSBCK = 0;

	/* set time period */
	_ENABLE_COP(WATCHDOG_PERIOD);

}

/* ------ Tasks ------ */

void pollBtnsTask(void) {
	int i;

	if ( 1 == GET_MUTEX_DISABLE_BTN() ) {
		mutexDisableBtn = true;
	} else {
		mutexDisableBtn = false;		
	}

	/* poll all task enable btns */
	for( i = 0; i < N_ENABLEABLE_TASKS; i++) {
		if ( 1 == GET_TASK_ENABLE_BTN(i) ) {
			taskEnableBtn[i] == true;
		} else {
			taskEnableBtn[i] == false;
		}
	}

}

void watchdogKickTask(void) {
	_FEED_COP();
}

void shortBlockingTask(void);

void longBlockingTask(void);

/* ------ Interrupt Service Routines ------ */

// TODO watchdog reset vector
/*==================================
 * Private Functions
 *==================================*/

/* ------ Helper functions ------*/

static void _blockingDelay(uint16_t delayMS);