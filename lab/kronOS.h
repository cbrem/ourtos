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

#include <hidef.h>
#include "derivative.h"
#include "inttypes_MC9S12C128.h"
#include "boolean.h"
#include "fn.h"
#include "timer.h"

/*==================================
 * MACROS
 *==================================*/

/* Bytes of stack space per task */
#define TASK_STACK_SIZE (64)

/*==================================
 * Types
 *==================================*/

/*
 * The kronOS's internal representation of a task's state.
 * Users should not manipulate value of this type.
 * The type if exposed to users only for memory-allocation purposes.
 */
typedef struct {
    byte_t stack[TASK_STACK_SIZE];
    uint32_t nextRunTime;
    fn_t task;
    uint16_t stackPtr;
    bool_t running;
    bool_t enabled;
    uint8_t normalPriority;
    uint8_t currentPriority;
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

static bool_t _mutexEnabled;

static task_t* _taskArray;
static uint8_t _numTasks;

// TODO: this could be an enum...
static bool_t _started;

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
void kronosStart(void);

/*
 * Sets the maximum period between runs of the RTOS's scheduler.
 */
void kronosSetSchedulerFreq(freq_t freq);

/*
 * Adds a task to the RTOS.
 * This function must be called before calling rtosStart, and should not be
 * called afterward.
 * This function must be called after calling rtosSetTaskArray.
 * Returns true if the task was successfully added, false otherwise (e.g. if
 * this is not a valid priority).
 */
bool_t kronosAddTask(uint8_t priority, uint16_t period, fn_t fnPtr);
        
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

/*
 * Returns the (normal) priority of the highest-priority task that is ready
 * to run.
 */
static uint8_t _scheduler(void);

/*
 * Runs whenever a timer overflow interrupt fires.
 * It calls the scheduler to find the highest-priority task which is ready to
 * run. Then, it switches the stack pointer to point to the new current task,
 * backs-up the old stack-pointer, and uses RTI to restore state and begin
 * executing the new task.
 *
 * NOTE: Implementation is very platform dependent.
 */
static void interrupt (TIMER_INTERRUPT_VECTOR) _timerIsr(void);

#endif // _KRONOS_H
