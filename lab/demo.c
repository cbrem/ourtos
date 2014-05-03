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
	
	// TODO kronosAddMutex

	/* starts the watchdog */
	_initWatchdog();

	for (;;)
	{
		/* infinite loop */

		// TODO temp test of functionality with cyclic exec
		pollBtnsTask();
		shortBlockingTask();
		longBlockingTask();
		watchdogKickTask();

	}

}

/* ------ Tasks ------ */

void watchdogKickTask(void) {

	SET_LEDS(LED_WATCHDOG);

	if ( WATCHDOG_FLG_ALL_SET == watchdogFlags ) {
		_FEED_COP();
		CLEAR_WATCHDOG_FLGS();
	} else {
		/* not all of the watchdog flags are set
		 * so watchdog cannot be kicked yet 
		 */
	}
}

void pollBtnsTask(void) {
	int i;

	SET_LEDS(LED_POLL_BTN);

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

	SET_WATCHDOG_FLGS(ID_POLL_BTN);

}

void shortBlockingTask(void) {
	
	SET_LEDS(LED_SHORT_BLK);

	//kronosAcquireMutex(&blockingMutex);
	_blockingDelayMsec(SHORT_BLOCK_TIME);
	//kronosReleaseMutex(&blockingMutex);

	SET_WATCHDOG_FLGS(ID_SHORT_BLK);
}

void longBlockingTask(void) {

	SET_LEDS(LED_LONG_BLK);

	//kronosAcquireMutex(&blockingMutex);
	_blockingDelayMsec(LONG_BLOCK_TIME);
	//kronosReleaseMutex(&blockingMutex);

	SET_WATCHDOG_FLGS(ID_LONG_BLK);

}

/* ------ Interrupt Service Routines ------ */

/*==================================
 * Private Functions
 *==================================*/

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

	CLEAR_WATCHDOG_FLGS();

	/* set time period */
	_ENABLE_COP(WATCHDOG_PERIOD);

}

/* ------ Helper functions ------*/

static void _blockingDelayMsec(uint16_t delayMS) {
	// TODO stole from Koopman
	uint16_t i;

	for (;delayMS > 0; delayMS--) {
    	for (i = 0; i < 200; i++) {
    		asm NOP;
      		asm NOP;
      		asm NOP;
      		asm NOP;
      	}
    }
}