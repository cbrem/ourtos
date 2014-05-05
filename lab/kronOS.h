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
#include "frame_MC9S12C128.h"
#include "boolean.h"

/*==================================
 * MACROS
 *==================================*/

#define TIMER_INTERRUPT_VECTOR (16)

/*==================================
 * Types
 *==================================*/

/*
 * The kronOS's internal representation of a task's state.
 * Users should not manipulate value of this type.
 * The type if exposed to users only for memory-allocation purposes.
 */
typedef struct {
    void (*task) (void);
    frame_t frame;
    bool_t running;
    bool_t enabled;
    uint8_t normalPriority;
    uint8_t currentPriority;
    uint32_t nextRunTime;
} task_t;

/*
 * A mutex.
 *
 * NOTE: A mutex will function as intended only for tasks which are scheduled
 * by the kronOS to which the mutex belongs.
 */
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

// TODO: is it okay for this to be static?
static task_t taskArray[];

// TODO: this could be an enum...
static boot_t started;

/*==================================
 * Public Functions
 *==================================*/

/*
 * Set the RTOS to its initial state.
 * This function must be called before calling any other kronOS functions.
 * Provides the RTOS with an array, where it will store information about
 * tasks' state.
 * Also, sets the maximum number of tasks that this RTOS can support.
 */
void kronosInit(task_t tasks[], uint8_t numTasks);

/* ----- Functions for a stopped kronOS ----- */

/*
 * Starts the RTOS.
 */
// TODO allow user control over scheduler period
void kronosStart(void);

/*
 * Adds a task to the RTOS.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 * This function must be called after calling rtosSetTaskArray.
 * Returns true if the task was successfully added, false otherwise (e.g. if
 * this is not a valid priority, if the period was shorter than the scheduler
 * period, etc.).
 */
bool_t kronosAddTask(uint8_t priority, uint16_t period, void (*task) (void));
        
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
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex);

/* ----- Functions for a started kronOS ----- */

/*
 * Shuts down a started RTOS.
 */
void kronosShutdown(void);

/*
 * Acquires the given mutex in a task-safe manner.
 */
void kronosAcquireMutex(mutex_t *mutex);

/*
 * Releases the given mutex in a task-safe manner.
 */
void kronosReleaseMutex(mutex_t *mutex);

/* ----- Functions for a started/stopped kronOS ----- */

/*
 * Sets the maximum number of milliseconds between runs of the RTOS's
 * scheduler.
 * Returns true if the scheduler period was successfully set to the given
 * value, false if this was not possible.
 */
bool_t kronosSetSchedulerPeriod(uint16_t period);

/*
 * Configures the RTOS to print debug information whenever the scheduler runs.
 */
void kronosEnableDebug(bool_t enable);

/*
 * Enables or disables mutexes globally.
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

/*
 * Determines which task should be set to run next.
 * Returns this task's (normal) priority.
 */
void _scheduler(void);

/*==================================
 * Interrupts
 *==================================*/

/*
 * Handles a timer overflow event.
 */
// TODO: should this be in the header file? should any private functions?
void interrupt (TIMER_INTERRUPT_VECTOR) _timerInterruptHandler(void);

#endif // _KRONOS_H
