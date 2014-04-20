/* kronOS
 * 	A lightweight premtive multitasking RTOS with priority ceiling.
 * 
 * Spencer Barton (sebarton)
 * Connor Brem (cbrem)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef KRONOS_H
#define KRONOS_H

/*==================================
 * Public Functions
 *==================================*/

/* Users may start and stop the RTOS using: 
 */
void rtosStart(void);
void rtosShutdown(void);

/* The RTOS will internally represent tasks using the type task_t. Users may 
 *  configure the RTOS's task scheduler using the following.
 *  
 * NOTE: These configuration functions should be called before calling 
 *  rtosStart().
 */
void rtosSetSchedulerPeriod(uint16_t period);
void rtosSetTaskArray(task_t[] tasks, uint8_t numTasks);
void void rtosAddTask(uint8_t priority, uint16_t period, void (*task) (void));
        
/* Additionally, the RTOS will provide a mutex implementation. Mutexs will have
 *  type mutex_t. The RTOS will support the following functions for mutexes:
 */
rtosAddMutex(uint8_t priority, mutex_t *mutex);
rtosAcquireMutex(mutex_t *mutex);
rtosReleaseMutex(mutex_t *mutex);

/* Finally, the user may enable and disable various features and tasks while 
 *  the RTOS is running using the following methods:
 */
void rtosEnableDebug(bool_t enable);
void rtosEnableMutexes(bool_t enable);
void rtosEnableTask(bool_t enable);

/*==================================
 * Private Functions
 *==================================*/

// TODO remember to include static keyword

#endif