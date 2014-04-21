/* 
 * kronOS - A lightweight, preemptive multitasking RTOS with priority ceiling.
 * 
 * Spencer Barton (sebarton)
 * Connor Brem (cbrem)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef _KRONOS_H
#define _KRONOS_H

/*==================================
 * Globals
 *==================================*/

/*==================================
 * Types
 *==================================*/

typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef struct {
    // TODO
} task_t;

typedef struct {
    // TODO
} mutex_t;

/*==================================
 * Public Functions
 *==================================*/

/*
 * Starts the RTOS.
 */
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
void kronosSetTaskArray(task_t[] tasks, uint8_t numTasks);

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

// TODO remember to include static keyword

#endif // _KRONOS_H
