/* 
 * kronOS - A lightweight, preemptive multitasking RTOS with priority ceiling.
 * 
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef _KRONOS_H
#define _KRONOS_H

/*==================================
 * Includes
 *==================================*/

#include "inttypes_MC9S12C128.h"
#include "boolean.h"

/*==================================
 * MACROS
 *==================================*/


/*==================================
 * Types
 *==================================*/

typedef struct {
    // TODO
    int TODO;
} task_t;

typedef struct {
    // TODO
    int TODO;
} mutex_t;

/*==================================
 * Exernal Globals
 *==================================*/

/*==================================
 * Local Globals
 *==================================*/

static bool_t mutexEnabled;

/*==================================
 * Public Functions
 *==================================*/

/*
 * Starts the RTOS.
 */
// TODO allow user control over scheduler period
void kronosStart(void);

/*
 * Shuts down a started RTOS.
 */
void kronosShutdown(void);


/*
 * Sets the maximum number of milliseconds between runs of the RTOS's
 * scheduler.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 */
void kronosSetSchedulerPeriod(uint16_t period);

/*
 * Provides a previously allocated array in which the RTOS will store task
 * state.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 */
void kronosSetTaskArray(task_t tasks[], uint8_t numTasks);

/*
 * Adds a task to the RTOS.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 * This function must be called after calling rtosSetTaskArray.
 */
void kronosAddTask(uint8_t priority, uint16_t period, void (*task) (void));
        
/*
 * Add a mutex to the RTOS which uses the given priority for priority ceiling.
 * This priority should be strictly higher than the priority of any task which
 * will use the mutex, and should not conflict with the priorities of any other
 * mutexes or any other tasks.
 * This function may only be called before calling rtosStart, and should not be
 * called afterward.
 */
void kronosAddMutex(uint8_t priority, mutex_t *mutex);

/*
 * Acquires the given mutex in a task-safe manner.
 */
void kronosAcquireMutex(mutex_t *mutex);

/*
 * Releases the given mutex in a task-safe manner.
 */
void kronosReleaseMutex(mutex_t *mutex);

/*
 * Configures the RTOS to print debug information whenever the scheduler runs.
 */
void kronosEnableDebug(bool_t enable);

/*
 * Enables or disables mutexes globally.
 * // TODO lable all relevant things Atomic
 */
void kronosEnableMutexes(bool_t enable);

/*
 * Enables or disables the task that was originally added at the given
 * priority.
 */
void kronosEnableTask(uint8_t priority, bool_t enable);

/*==================================
 * Private Functions
 *==================================*/

static void _initTimer() {
    
}

#endif // _KRONOS_H
