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

/*==================================
 * MACROS
 *==================================*/

/* timer */
#define TIMER_PRESCALER (0) /* prescaler bus_clk / 1 */
/* 
 * 32-bit value to increment timer counter on each timer overflow
 * This value is based on an 8MHz clk with prescaler 1/1 and 
 * a counter variable that is 8.24 fixed point msec.
 */
#define TIMER_COUNTER_INCR (0x083126E9) // 137438953

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

/* time variables - these are updated in timer ISR */
static uint32_t timeMsec; /* current time in msec */
static uint32_t timeCounter; /* 8.24 fixed point counter to keep track of time in msec*/

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
void kronosSetSchedulerPeriod(uint16_t period); // TODO restrict to set periods

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

/*
 * _initTimer sets the global time variables timeMsec and timeCounter to zero
 *  enables the timer, enables timer interrupt and sets the prescaler to the 
 *  given value
 */
static void _initTimer();

/*
 * getCurrentTimeMsec returns the current time in msec
 *  This function disables interrupts
 */
static uint32_t _getCurrentTimeMsec(void);

/*
 * _timerISR is triggered on a timer overflow which occurs at 
 *  the set period. This function increments the current time 
 *  as well as performs the task switching.
 *
 * Task Switching
 *  The ISR looks for the next highest priority task that is ready to run
 *  (as set by the scheduler) and sets this as the current task. It 
 *  switches the stack pointer to point to the new current task, backs-up
 *  the old stack-pointer and then uses RTI to restore state and begin
 *  executing the new task.
 */
static void interrupt 16 _timerISR(void);

#endif // _KRONOS_H
