/* 
 * OurTOS - A lightweight, preemptive multitasking preemptive RTOS with priority ceiling.
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
// TODO: tasks can only hold one mutex at a time with this model!
//    can we just do this by...mutexes only replacing lower-priority
//    mutexes...and mutexes store the priority of the mutex they
//    superceeded...and then release replaces this priority...but
//    what if that's already been released independantly!!! ooh...
//    maybe the mutexes have to store a lot of state...
//
// TODO: what do we do if a mutex becomes clobbered (i.e. invalid)
// which a task has it, and then it releases that mutex?
//
// TODO: inconsistency between whether things are a no-op or return
// false when priority is wrong
//
// TODO: use started as a guard on addTask, etc.
//
// TODO: potential problems if you toggle shutdown/start quickly
//
// TODO: in _idle/main loop, maybe delay a little?

#include "OurTOS.h"

/*==================================
 * Public Functions
 *==================================*/

void ourtosInit(task_t taskArray[], uint8_t maxPriority, freq_t freq) {
	int i;

	/* Initialize globals.
	 *
	 * Note that we don't initialize _mainLoopStackPointer, since the timer ISR
	 * will always run at least once and initialize it before we try to use it.
	 */
	_maxPriority = maxPriority;
	_started = false;
	_debug = false;
	_mutexesEnabled = true;
	_currentTask = MAIN_LOOP_PRIORITY;

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

/* ----- Functions for a stopped OurTOS ----- */

void ourtosStart() {
	_started = true;
	EnableInterrupts;
	timerEnableInterrupt();

	/* Loop until the users stops the RTOS. At this point, return. */
	for (;;) {
		DisableInterrupts;
		if (!_started) { break; }
		EnableInterrupts;
	}
	EnableInterrupts;
}

bool_t ourtosAddTask(uint8_t priority, uint16_t period, fn_t task) {
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
        
bool_t ourtosAddMutex(uint8_t priority, mutex_t *mutex) {
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

/* ----- Functions for a started OurTOS ----- */

void ourtosShutdown(void) {
	/* Shutdown merely stops the interrupt from running and sets _started to
	 * false.
	 * Therefore, when the last scheduled task stops running, in _idle, it will
	 * RTI to the main loop (i.e. the will loop in ourtosStart) instead of
	 * being interrupted by the timer overflow isr.
	 */
	timerDisableInterrupt();
	_started = false;
}

void ourtosAcquireMutex(mutex_t *mutex) {
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
	_taskArray[_currentTask].currentPriority = priority;
}

void ourtosReleaseMutex(mutex_t *mutex) {
	/* Return the current task (i.e. the task that called this function) to its
	 * normal priority.
	 * Note that we do not care if mutexes are currently disabled.
	 */
	(void)mutex; // TODO mutex set-up may not be best
	_taskArray[_currentTask].currentPriority =
		_taskArray[_currentTask].normalPriority;	
}

/* ----- Functions for a started/stopped OurTOS ----- */

void ourtosEnableDebug(bool_t enable) {
	_debug = enable;
}

void ourtosEnableMutexes(bool_t enable) {
	_mutexesEnabled = enable;
}

void ourtosEnableTask(uint8_t priority, bool_t enable) {
	if ( priority >= _maxPriority
		|| _taskArray[priority].usage != USAGE_TASK) {
		/* This priority is either not valid for this RTOS, or does not
		 * currently correspond to a task.
		 * Do nothing.
		 */
		return;
	}

	_taskArray[priority].enabled = enable;
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _scheduler(void) {
	uint8_t priority;

	/* The lowest current priority over tasks that currently can run. */
	uint8_t lowestCurrentPriority;

	/* The normal priority of the task with lowestCurrentPriority.
	 * This can be used to identify the task outside of this function.
	 */
	uint8_t lowestNormalPriority;

	/* Add tasks that can run will have priorities less than _maxPriority. */
	lowestCurrentPriority = _maxPriority;

	/* If no tasks can run, we will return _maxPriority, which is a signal to
	 * run the main loop.
	 */
	lowestNormalPriority = MAIN_LOOP_PRIORITY;

	/* Find the normal priority of the task that can run that has the lowest
	 * current priority.
	 */
	for (priority = 0; priority < _maxPriority; priority++) {
		if (_taskArray[priority].usage == USAGE_TASK
				&& _taskArray[priority].enabled == true
				&& _taskArray[priority].timeToNextRun <= 0
				&& _taskArray[priority].currentPriority < lowestCurrentPriority) {
			lowestCurrentPriority = _taskArray[priority].currentPriority;
			lowestNormalPriority = _taskArray[priority].normalPriority;
		}
	}
	return lowestNormalPriority;
}

static void _idle() {
	// TODO: also set currentPriority to normalPriority for each task to
	// invalidate mutexes? Would this even work?

	/* Record that current task is no longer running. */
	DisableInterrupts;
	_taskArray[_currentTask].running = false;
	EnableInterrupts;

	/* As long as _started is true (i.e. the timer isr is enabled),
	 * wait for it to interrupt us.
	 */
	for (;;) {
		DisableInterrupts;
		if (!_started) { break; }
		EnableInterrupts;
	}
	EnableInterrupts;

	/* If _started becomes false (i.e. the timer isr is stopped),
	 * RTI into the main loop.
	 */
	{ asm LDS _mainLoopStackPtr; } 
	{ asm RTI; }
}

/* ---------- debug print --------------- */

static void _debugPrint(uint8_t scheduledTask) {
	uint16_t len;

	/* print top bar */
	len = sprintf(_debugMsgBuf, _debugMsgTop);
	serialWrite(_debugMsgBuf, len);

	/* print current task */
	len = sprintf(_debugMsgBuf, _debugMsgTaskID, scheduledTask);
	serialWrite(_debugMsgBuf, len);

	/* print header */
	len = sprintf(_debugMsgBuf, _debugMsgHeader);
	serialWrite(_debugMsgBuf, len);	

	/* print header bar */
	len = sprintf(_debugMsgBuf, _debugMsgHeaderBar);
	serialWrite(_debugMsgBuf, len);	

	_debugPrintTaskArray();
}

void _debugPrintTaskArray(void) {
	uint8_t priority
	uint16_t len;

	/* print each task info on a newline */
	for (priority = 0; priority < _maxPriority; priority++) {
		switch(_taskArray[priority].usage) {
			case USAGE_TASK:
				len = sprintf(_debugMsgBuf, _debugMsgTaskLine,
					priority, 
					_taskArray[priority].currentPriority,
					_taskArray[priority].timeToNextRun,
					_taskArray[priority].period,
					_taskArray[priority].running,
					_taskArray[priority].enabled);
				break;
			case USAGE_MUTEX:
				len = sprintf(_debugMsgBuf, _debugMsgMutexLine, priority);
				break;
			case USAGE_NONE:
				len = sprintf(_debugMsgBuf, _debugMsgNoneLine, priority);
				break;
		}

		/* print to serial */
		serialWrite(_debugMsgBuf, len);	
	}	
}

/* ---------- Timer ISR --------------- */

void interrupt (TIMER_INTERRUPT_VECTOR) _timerIsr(void) {

	/* make this local vars static so as to store on the heap instead
	 * of the stack. This avoids stack smashing.
	 */
	static uint8_t* stackPtrTmp;
	static uint8_t scheduledTask;
	static int32_t elapsedTime;

	/* acknowledge interrupt */
	TFLG2_TOF = 1;

	/* get current stackPtr */
	{ asm STS stackPtrTmp; }

	/* Save stack ptr
	 * special case on main loop. 
	 * The main loop stack pointer is saved in a seperate variable*/
	if (MAIN_LOOP_PRIORITY == _currentTask) {
		_mainLoopStackPtr = stackPtrTmp;
	} else {
		_taskArray[_currentTask].stackPtr = stackPtrTmp;		
	}

	/* Set current stack to ISR stack.
	 * The ISR stack keeps the ISR from stack smashing
	 * an uninitialized task's stack.
	 * start at lowest memory address in stack, note this is ptr arithmatic.
	 */
	stackPtrTmp = _ISRstack + TASK_STACK_SIZE - 1;
	{ asm LDS stackPtrTmp; }

	/* update time variables */
	timerUpdateCurrent();

	/* update task timing - note that this requires interrupts be disabled */
	elapsedTime = timerElapsedTime();
	_updateTaskTimes(elapsedTime);

	/* Run the scheduler every time through the ISR
	 * The scheduler determines which task to run once the 
	 * ISR returns.
	 */
	scheduledTask = _scheduler();

	/* If serial dubugging is enabled, print information about the current
	 * state of all tasks.
	 */
	if (_debug) {
		_debugPrint(scheduledTask);	
	}

	/* Special case if scheduler has no tasks to run.
	 * In this case the returned index is _numTasks. 
	 * The main loop will be run until a task is ready 
	 * to run. Otherwise simply use the designated task's
	 * stack ptr.
	 */
	if ( MAIN_LOOP_PRIORITY == scheduledTask ) {
		stackPtrTmp = _mainLoopStackPtr;
	
	} else {
		
		if ( false == _taskArray[scheduledTask].running ) {
			/* Create launch stack if not currently running
			 * This operation sets the stack pointer.
			 */
			_createNewStack(scheduledTask);
		}
		
		/* grab stack ptr */
		stackPtrTmp = _taskArray[scheduledTask].stackPtr;
		/* task is now runing */
		_taskArray[scheduledTask].running = true;
		
		/* Update next run time to be 1 period away.
		 * If timeToNextRun was more than 1 period away before then
		 * it stays negative which means that the task will 
		 * attempt to run again as soon as possible. This also
		 * signals that the task missed at least one prior deadline. 
		 */
		_taskArray[scheduledTask].timeToNextRun +=
			_taskArray[scheduledTask].period;
	}

	/* Set global current task to the scheduled task */
	_currentTask = scheduledTask;

	/* Set stack pointer to the current task stack pointer */
	{ asm LDS stackPtrTmp; }

	/* ISR does an RTI to return to the current stack. The 
	 * RTI restores all of the state (registers, cond codes, etc)
	 * if the task was running before or simply loads the dummy
	 * state (as set by the create stack function) if the task
	 * is being initialized. 
	 */
}

static void _createNewStack(uint8_t priority) {
	uint8_t stackPtr = TASK_STACK_SIZE;

	/* set-up the return fuction lowest on the stack
	 * This is the function that the task returns to upon
	 * completion.
	 */
	_taskArray[priority].stack[stackPtr - 1] = LOW_BYTE(&_idle);
	_taskArray[priority].stack[stackPtr - 2] = HIGH_BYTE(&_idle);

	/* set-up the function pointer. This is placed such that it becomes 
	 * the return function for the RTI from the timer ISR
	 */
	_taskArray[priority].stack[stackPtr - 3] = LOW_BYTE(&_taskArray[priority].task);
	_taskArray[priority].stack[stackPtr - 4] = HIGH_BYTE(&_taskArray[priority].task);

	/* set the various task struct elems. The stack pointer is the 
	 * location in the task's stack pointing to the top of the dummy 
	 * register values which are used in the timer ISR RTI.
	 */
	_taskArray[priority].stackPtr = &_taskArray[priority].stack[stackPtr - RTI_DUMY_REG_BYTES];
}

static void _updateTaskTimes(int32_t elapsedTime) {
	int i;

	/* iter throught valid tasks and subratract elapsedTime */
	for( i = 0; i < _maxPriority; i++ ) {
		if ( USAGE_TASK == _taskArray[i].usage ) {
			_taskArray[i].timeToNextRun -= elapsedTime;
		}
	}

}
