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

void kronosInit(task_t taskArray[], uint8_t numTasks) {
	_taskArray = taskArray;
	_numTasks = numTasks;
}

/* ----- Functions for a stopped kronOS ----- */

void kronosStart(void) {
	// TODO
}

void kronosSetSchedulerFreq(freq_t freq) {
	// TODO
	timerInit(freq);
}

bool_t kronosAddTask(uint8_t priority, uint16_t period, fn_t fnPtr) {
	// TODO
	return false;
}
        
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex) {
	// TODO
	return false;
}

/* ----- Functions for a started kronOS ----- */

void kronosShutdown(void) {
	// TODO
}

void kronosAcquireMutex(mutex_t *mutex) {
	// TODO
}

void kronosReleaseMutex(mutex_t *mutex) {
	// TODO
}

/* ----- Functions for a started/stopped kronOS ----- */

void kronosEnableDebug(bool_t enable) {
	// TODO
}

void kronosEnableMutexes(bool_t enable) {
	// TODO
}

void kronosEnableTask(uint8_t priority, bool_t enable) {
	// TODO
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _scheduler(void) {
	
	return 0;
}

static void interrupt (TIMER_INTERRUPT_VECTOR) _timerIsr(void) {

	/* make this local vars static so as to store on the heap instead
	 * of the stack. This avoids stack smashing.
	 */
	static uint8_t* stackPtrTmp;
	static uint8_t taskIndex;

	/* acknowledge interrupt */
	TFLG2_TOF = 1;

	/* get current stackPtr */
	{ asm STS stackPtrTmp; }

	/* Save stack ptr
	 * special case on main loop. 
	 * The main loop stack pointer is saved in a seperate variable*/
	if (MAIN_LOOP_INDEX == _currentTask) {
		_mainLoopStackPtr = stackPtrTmp;
	} else {
		_taskArray[_currentTask].stackPtr = stackPtrTmp;		
	}

	/* Set current stack to ISR stack.
	 * The ISR stack keeps the ISR from stack smashing
	 * an uninitialized task's stack.
	 */
	stackPtrTmp = _ISRstack;
	{ asm LDS stackPtrTmp; }

	/* update time variables */
	timerUpdateCurrent();

	/* update task timing - note that this requires interrupts be disabled */
	_updateTaskTimes(timerElapsedTime());

	/* Run the scheduler every time through the ISR
	 * The scheduler determines which task to run once the 
	 * ISR returns.
	 */
	taskIndex = _scheduler();

	/* Special case if scheduler has no tasks to run.
	 * In this case the returned index is _numTasks. 
	 * The main loop will be run until a task is ready 
	 * to run. Otherwise simply use the designated task's
	 * stack ptr.
	 */
	if ( MAIN_LOOP_INDEX == taskIndex ) {
		stackPtrTmp = _mainLoopStackPtr;
	
	} else {
		
		if ( false == _taskArray[taskIndex].running ) {
			/* Create launch stack if not currently running
			 * This operation sets the stack pointer
			 */
			_createNewStack(taskIndex);
		}
		
		/* grab stack ptr */
		stackPtrTmp = _taskArray[taskIndex].stackPtr;
		/* task is now runing */
		_taskArray[taskIndex].running = true;
		
		/* update next run time to be 1 period away */
		_taskArray[taskIndex].timeToNextRun += _taskArray[taskIndex].period;

	}

	/* Set global current task to the scheduled task */
	_currentTask = taskIndex;

	/* Set stack pointer to the current task stack pointer */
	{ asm LDS stackPtrTmp; }

	/* ISR does an RTI to return to the current stack. The 
	 * RTI restores all of the state (registers, cond codes, etc)
	 * if the task was running before or simply loads the dummy
	 * state (as set by the create stack function) if the task
	 * is being initialized. 
	 */
}

static void _updateTaskTimes(int32_t elapsedTime) {
	int i;

	/* iter throught valid tasks and subratract elapsedTime */
	for( i = 0; i < _numTasks; i++ ) {
		if ( USAGE_TASK == _taskArray[i].usage ) {
			_taskArray[i].timeToNextRun -= elapsedTime;
		}
	}

}