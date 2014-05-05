/* 
 * kronOS - A lightweight, preemptive multitasking RTOS with priority ceiling.
 * 
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#include "kronOS.h"

// myTOS, yourTOS, ourTOS!

 /*==================================
 * Public Functions
 *==================================*/

void kronosStart(void);

void kronosShutdown(void);

void kronosSetSchedulerPeriod(uint16_t period);

void kronosSetTimerPeriod(uint8_t prescaler);

void kronosSetTaskArray(task_t tasks[], uint8_t numTasks);

void kronosAddTask(uint8_t priority, uint16_t period, void (*task) (void));
        
void kronosAddMutex(uint8_t priority, mutex_t *mutex);

void kronosAcquireMutex(mutex_t *mutex);

void kronosReleaseMutex(mutex_t *mutex);

void kronosEnableDebug(bool_t enable);

void kronosEnableMutexes(bool_t enable);

void kronosEnableTask(uint8_t priority, bool_t enable);

/*==================================
 * Private Functions
 *==================================*/

static void _initTimer(void) {

	/* enable timer */
	TSCR1_TN = 1;

	/* set timer overflow interrupt */
	TSCR2_TOF = 1;

	/* set timer prescaler */
	TSCR2_PR = TIMER_PRESCALER;

}

static uint32_t _getCurrentTimeMsec(void) {
	uint32_t timeTmp;

	/* need atomic access */
	DisableInterrupts;
	timeTmp = timeMsec;
	EnableInterruprs;

	return timeTmp;
}

static void interrupt 16 _timerISR(void) {

	
}

