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
//
// TODO: do we even need _started?
//
// TODO: make sure task_t.toNextRun doesn't get implicitly cast
// to a signed value.
//
// TODO: tasks can only hold one priority at a time with this model!
//
// TODO: what do we do if a mutex becomes clobbered (i.e. invalid)
// which a task has it, and then it releases that mutex?

#include "kronOS.h"

/*==================================
 * Public Functions
 *==================================*/

void kronosInit(task_t taskArray[], uint8_t maxPriority, freq_t freq) {
	int i;

	/* Initialize globals.
	 *
	 * Note that we don't initialize _mainLoopStackPointer, since the timer ISR
	 * will always run at least once and initialize it before we try to use it.
	 */
	_maxPriority = maxPriority;
	_started = false;
	_mutexesEnabled = true;
	_currentTask = MAIN_LOOP_INDEX;

	/* Initialize task array.
	 * Mark that all priorities are not yet devoted to either mutexes or tasks.
	 */
	_taskArray = taskArray;
	for (i = 0; i < _maxPriority; i++) {
		_taskArray[i].usage = USAGE_NONE;
	}

	timerInit(freq);
	serialInit(BAUD_9600);
}

/* ----- Functions for a stopped kronOS ----- */

void kronosStart() {
	_started = true;
	EnableInterrupts;
	timerEnableInterrupt();

	/* Loop until the users stops the RTOS. At this point, return. */
	while (true) {
		DisableInterrupts;
		if (_started) { break; }
		EnableInterrupts;
	}
	EnableInterrupts;
}

bool_t kronosAddTask(uint8_t priority, uint16_t period, fn_t task) {
	if (priority >= _maxPriority) {
		/* This is an invalid priority. */
		return false;
	}

	/* Fill the struct for this task, clobbering any existing task or mutex
	 * at this priority.
	 */
	_taskArray[priority].usage = USAGE_TASK;
	/* Leave _taskArray[priority].stack uninitialized */
	_taskArray[priority].timeToNextRun = period;	// TODO: unsigned -> signed cast here?
	_taskArray[priority].period = period;
    _taskArray[priority].task = task;
    /* Leave _taskArray[priority].stackPtr uninitialized */
    _taskArray[priority].running = false;
    _taskArray[priority].enabled = true;
    _taskArray[priority].normalPriority = priority;
    _taskArray[priority].currentPriority = priority;

	return true;
}
        
bool_t kronosAddMutex(uint8_t priority, mutex_t *mutex) {
	if (priority >= _maxPriority) {
		/* This is an invalid priority. */
		return false;
	}

	/* Mark in _taskArray that this priority is reserved for a mutex, and that
	 * there is no task here.
	 * Note that this will clobber any task currently here.
	 */
	_taskArray[priority].usage = USAGE_MUTEX;

	/* Make the mutex reference the given priority so that it remains
	 * associated with this priority in the user's program.
	 */
	*mutex = priority;

	return true;
}

/* ----- Functions for a started kronOS ----- */

void kronosShutdown(void) {
	/* Shutdown merely stops the interrupt from running and sets _started to
	 * false.
	 * Therefore, when the last scheduled task stops running, in _idle, it will
	 * RTI to the main loop (i.e. the will loop in kronosStart) instead of
	 * being interrupted by the timer overflow isr.
	 */
	timerDisableInterrupt();
	_started = false;
}

void kronosAcquireMutex(mutex_t *mutex) {
	uint8_t priority;

	/* The mutex is merely a reference which holds the priority that it
	 * occupies in the RTOS.
	 */
	priority = *mutex;

	if (_taskArray[priority].usage != USAGE_MUTEX
		|| _mutexesEnabled) {
		/* This mutex has been clobbered by a task since it was created,
		 * or mutexes are disabled.
		 * Do nothing.
		 */
		return;
	}

	/* Promote the current task (i.e. the task that called this function) to
	 * the priority associated with the mutex.
	 */
	_taskArray[_currentTask].currentPriority = priorty;
}

void kronosReleaseMutex(mutex_t *mutex) {
	/* Return the current task (i.e. the task that called this function) to its
	 * normal priority.
	 * Note that we do not care if mutexes are currently disabled.
	 */
	_taskArray[_currentTask].currentPriority =
		_taskArray[_currentTask].normal;	
}

/* ----- Functions for a started/stopped kronOS ----- */

void kronosEnableDebug(bool_t enable) {
	_debug = enable;
}

void kronosEnableMutexes(bool_t enable) {
	_mutexesEnabled = enable;
}

void kronosEnableTask(uint8_t priority, bool_t enable) {
	if ( priority >= _maxPriority
		|| _taskArray[priority].usage != USAGE_TASK) {
		/* This priority is either not valid for this RTOS, or does not
		 * currently correspond to a task.
		 * Do nothing.
		 */
		return;
	}

	_taskArray[priority].enabled != enable;
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _scheduler(void) {
	
	// TODO: return _maxPriority if mainloop should run

	// To schedule a task:
	//  - no lower-current-priority tasks can be scheduled
	//  - timeToNextRun <= 0
	//  - enabled
	//  - currently being used as a task

	return 0;
}

static void _idle() {
	// TODO: also set currentPriority to normalPriority for each task to
	// invalidate mutexes? Would this even work?

	/* Record that current task is no longer running. */
	DisableInterrupts;
	_taskArray[_currentTask].running = false;
	EnableInterrupts.

	/* As long as _started is true (i.e. the timer isr is enabled),
	 * wait for it to interrupt us.
	 */
	while (true) {
		DisableInterrupts;
		if (!_started) { break; }
		EnableInterrupts;
	}
	EnableInterrupts;

	/* If _started becomes false (i.e. the timer isr is stopped),
	 * RTI into the main loop.
	 */
	// TODO: is this maybe right...?
	{
		asm LDS _mainLoopStackPointer;
		asm RTI;
	}
}

static void _debugPrint() {
	// TODO: write this, and also integrate it into the ISR
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
