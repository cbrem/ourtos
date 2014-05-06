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
#include "inttypesMC9S12C128.h"
#include "boolean.h"
#include "fn.h"
#include "serial.h"
#include "timer.h"

/*==================================
 * MACROS
 *==================================*/

/* Bytes of stack space per task */
#define TASK_STACK_SIZE (64)

/* The scheduler schedules task _maxPriority to mean that the main loop should
 * run.
 */
#define MAIN_LOOP_INDEX (_maxPriority)

#define DEBUG_MESSAGE_SIZE 21

/*==================================
 * Constants
 *==================================*/

const static char *_debugMessageFormat = "Current task ID: %02X\n";

/*==================================
 * Types
 *==================================*/

/*
 * The kronOS's internal representation of a task's state.
 * Users should not manipulate value of this type.
 * The type if exposed to users only for memory-allocation purposes.
 */
typedef struct {
    usage_t usage;                  // The usage of this priority.
    byte_t stack[TASK_STACK_SIZE];  // Stack for this task.
    int32_t timeToNextRun;          // Time to next run in ms.
    uint16_t period;                // Number of ms between task runs.
    fn_t task;                      // Pointer to the function to run.
    uint16_t stackPtr;              // Saved stack pointer.
    bool_t running;                 // It is currently running?
    bool_t enabled;                 // Should it ever run?
    uint8_t normalPriority;         // Priority when not holding mutexes.
    uint8_t currentPriority;        // Priority, possibly elevated by mutexes.
} task_t;

/*
 * The usage of a priority.
 */
typedef enum {
    USAGE_NONE,
    USAGE_TASK,
    USAGE_MUTEX
} usage_t;

/*
 * A mutex.
 *
 * NOTE: A mutex will function as intended only for tasks which are scheduled
 * by the kronOS to which the mutex belongs.
 *
 * NOTE: If a task acquires a mutex, it must release the mutex before it
 * finishes running.
 */
typedef uint8_t mutex_t;

/*==================================
 * Exernal Globals
 *==================================*/

/*==================================
 * Local Globals
 *==================================*/

static bool_t _mutexesEnabled;

static task_t* _taskArray;

static uint8_t _maxPriority;

/* garabage stack for use during ISR */
static uint8_t _ISRstack[TASK_STACK_SIZE];

/* main loop stack pointer to enable running of main loop 
 * when nothing else wants to run.
*/
static uint8_t* _mainLoopStackPtr;

static bool_t _started;

static uint8_t _currentTask;

static char _debugMessageBuffer[DEBUG_MESSAGE_SIZE + 1];

/*==================================
 * Public Functions
 *==================================*/

/*
 * Set the RTOS to its initial state.
 * This function must be called before calling any other kronOS functions.
 * Provides the RTOS with an array, where it will store information about
 * tasks' state.
 *
 * Also sets the RTOS's max priorty (i.e. the maximum number of task/mutexes
 * that the RTOS can support). Note that the provided array must be at least
 * as long at maxPriority. Also note that this bound is *exclusive*. The range
 * of valid priorities for tasks/mutexes is [0, ..., maxPriority).
 *
 * Also sets the maximum period between runs of the RTOS's scheduler.
 */
void kronosInit(task_t taskAray[], uint8_t maxPriority, freq_t freq);

/* ----- Functions for a stopped kronOS ----- */

/*
 * Starts the RTOS.
 * This function will block until kronosShutdown is called from within a task.
 * This function will enable interrupts.
 */
void kronosStart(void);

/*
 * Adds a task to the RTOS.
 * This function must be called after calling kronosInit.
 * Returns true if the task was successfully added, false otherwise (e.g. if
 * this is not a valid priority).
 * Clobbers any existing task of mutex at this priority.
 */
bool_t kronosAddTask(uint8_t priority, uint16_t period, fn_t task);
        
/*
 * Add a mutex to the RTOS which uses the given priority for priority ceiling.
 * This priority should be strictly higher than the priority of any task which
 * will use the mutex, and should not conflict with the priorities of any other
 * mutexes or any other tasks.
 * This function may only be called before calling kronosStart, and should not be
 * called afterward.
 * Returns true if the mutex was successfully added, false otherwise (e.g. if
 * this is not a valid priority).
 * Clobbers any existing task or mutex at this priority.
 */
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex);

/* ----- Functions for a started kronOS ----- */

/*
 * Shuts down a started RTOS.
 */
void kronosShutdown(void);

/*
 * Acquires the given mutex in a task-safe manner.
 * Interrupts must be disabled in order to use this function safely.
 * A mutex is invalid if its priority in the RTOS is replaced by another task.
 * Acquiring such a mutex is a no-op.
 */
void kronosAcquireMutex(mutex_t *mutex);

/*
 * Releases the given mutex in a task-safe manner.
 * Interrupts must be disabled in order to use this function safely.
 * A mutex is invalid if its priority in the RTOS is replaced by another task.
 * Releasing such a mutex is a no-op.
 */
void kronosReleaseMutex(mutex_t *mutex);

/* ----- Functions for a started/stopped kronOS ----- */

/*
 * Configures the RTOS to print debug information whenever the scheduler runs.
 * Interrupts must be disabled in order to use this function safely.
 */
void kronosEnableDebug(bool_t enable);

/*
 * Enables or disables mutexes globally.
 * Specifically, determines whether tasks can newly acquire mutexes.
 * Tasks can still release mutexes when mutexes are "disabled", and
 * tasks already holding mutexes are not affected.
 * Interrupts must be disabled in order to use this function safely.
 */
void kronosEnableMutexes(bool_t enable);

/*
 * Enables or disables the task that was originally added at the given
 * priority.
 * Interrupts must be disabled in order to use this function safely.
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
 * Idles until it is either interrupted by the timer interrupt, or
 * the timer interrupt is turned off (i.e. _started is set to false).
 * In the later case, RTIs into the main loop in kronosStart.
 */
static void _idle(void);

/*
 * Print information about the state of the RTOS over serial.
 * This function is meant for debugging purposes only.
 */
static void _debugPrint(void);

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

/*
 * Creates a new stack for the task with the given priority.
 *
 * TODO: spensar write dis
 */
static void _createNewStack(uint8_t priority);

/* 
 * _updateTaskTimes iterates through the tasks and subtracts 
 * elapsed time from time to run. Time to run represents how far in the 
 * future the task should run. A positive value for time to run 
 * means the funtion is set to run in the future while a negative
 * value means that the task is overdue to run. Elapsed time is the
 * amount of time since the prior run of the scheduler.
 */
static void _updateTaskTimes(int32_t elapsedTime);

#endif // _KRONOS_H
