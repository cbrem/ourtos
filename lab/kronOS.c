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

void kronosInit(task_t tasks[], uint8_t numTasks) {
	// TODO
}

/* ----- Functions for a stopped kronOS ----- */

/*
 * Starts the RTOS.
 */
void kronosStart(void) {
	// TODO
}

/*
 * Sets the maximum period between runs of the RTOS's scheduler.
 */
void kronosSetSchedulerPeriod(period_t period) {
	// TODO
}

/*
 * Adds a task to the RTOS.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 * This function must be called after calling rtosSetTaskArray.
 * Returns true if the task was successfully added, false otherwise (e.g. if
 * this is not a valid priority).
 */
bool_t kronosAddTask(uint8_t priority, period_t period, taskFn_t) {
	// TODO
	return false;
}
        
/*
 * Add a mutex to the RTOS which uses the given priority for priority ceiling.
 * This priority should be strictly higher than the priority of any task which
 * will use the mutex, and should not conflict with the priorities of any other
 * mutexes or any other tasks.
 * This function may only be called before calling rtosStart, and should not be
 * called afterward.
 * Returns true if the mutex was successfully added, false otherwise (e.g. if
 * this is not a valid priority).
 */
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex) {
	// TODO
	return false;
}

/* ----- Functions for a started kronOS ----- */

/*
 * Shuts down a started RTOS.
 */
void kronosShutdown(void) {
	// TODO
}

/*
 * Acquires the given mutex in a task-safe manner.
 */
void kronosAcquireMutex(mutex_t *mutex) {
	// TODO
}

/*
 * Releases the given mutex in a task-safe manner.
 */
void kronosReleaseMutex(mutex_t *mutex) {
	// TODO
}

/* ----- Functions for a started/stopped kronOS ----- */

/*
 * Configures the RTOS to print debug information whenever the scheduler runs.
 */
void kronosEnableDebug(bool_t enable) {
	// TODO
}

/*
 * Enables or disables mutexes globally.
 * // TODO lable all relevant things Atomic
 */
void kronosEnableMutexes(bool_t enable) {
	// TODO
}

/*
 * Enables or disables the task that was originally added at the given
 * priority.
 */
void kronosEnableTask(uint8_t priority, bool_t enable) {
	// TODO
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _scheduler(void) {
	// TODO
	return 0;
}

static void interrupt (TIMER_INTERRUPT_VECTOR) _timerIsr(void) {
	// TODO
}
