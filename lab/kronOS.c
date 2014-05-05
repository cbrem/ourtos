/* 
 * kronOS - A lightweight, preemptive multitasking RTOS with priority ceiling.
 * 
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

// TODO: somehow track which priorities actually have associated
// values. because we shouldn't try to run tasks that are null!

#include "kronOS.h"

/*==================================
 * Public Functions
 *==================================*/

void kronosInit(task_t taskArray[], uint8_t numTasks) {
	_taskArray = taskArray;
	_numTasks = numTasks;
}

/* ----- Functions for a stopped kronOS ----- */

void kronosStart(void) {
	// TODO
}

void kronosSetSchedulerFreq(freq_t freq) {
	// TODO
	timerInit(freq);
}

bool_t kronosAddTask(uint8_t priority, uint16_t period, fn_t fnPtr) {
	// TODO
	return false;
}
        
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex) {
	// TODO
	return false;
}

/* ----- Functions for a started kronOS ----- */

void kronosShutdown(void) {
	// TODO
}

void kronosAcquireMutex(mutex_t *mutex) {
	// TODO
}

void kronosReleaseMutex(mutex_t *mutex) {
	// TODO
}

/* ----- Functions for a started/stopped kronOS ----- */

void kronosEnableDebug(bool_t enable) {
	// TODO
}

void kronosEnableMutexes(bool_t enable) {
	// TODO
}

void kronosEnableTask(uint8_t priority, bool_t enable) {
	// TODO
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _scheduler(void) {
	
	return 0;
}

static void interrupt (TIMER_INTERRUPT_VECTOR) _timerIsr(void) {

	/* acknowledge interrupt */
	TFLG2_TOF = 1;

	/
}
