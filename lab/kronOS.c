/* 
 * kronOS - A lightweight, preemptive multitasking RTOS with priority ceiling.
 * 
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#include "kronOS.h"

/*==================================
 * Public Functions
 *==================================*/

void kronosInit(task_t tasks[], uint8_t numTasks) {

}

/* ----- Functions for a stopped kronOS ----- */

void kronosStart(void) {

}

void kronosSetSchedulerPeriod(uint16_t period) {

}

void kronosAddTask(uint8_t priority, uint16_t period, void (*task) (void)) {

}

void kronosAddMutex(uint8_t priority, mutex_t *mutex) {

}

/* ----- Functions for a started kronOS ----- */

void kronosShutdown(void) {

}

void kronosAcquireMutex(mutex_t *mutex) {

}

void kronosReleaseMutex(mutex_t *mutex) {

}

/* ----- Functions for a started/stopped kronOS ----- */

void kronosEnableDebug(bool_t enable) {

}

void kronosEnableMutexes(bool_t enable) {

}

void kronosEnableTask(uint8_t priority, bool_t enable) {

}

/*==================================
 * Private Functions
 *==================================*/

uint8_t _scheduler(void) {

}

/*==================================
 * Interrupts
 *==================================*/

void interrupt (TIMER_INTERRUPT_VECTOR) _timerInterruptHandler(void) {
    uint8_t taskToRun;

    taskToRun = _scheduler();
    
}

// TODO: somehow track which priorities actually have associated
// values. because we shouldn't try to run tasks that are null!
