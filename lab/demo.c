/* Demo of OurTOS
 *  This file demonstrates the functionality of OurTOS. OurTOS is a lightweight 
 *  premtive multitasking RTOS with priority ceiling. 
 * 
 *  In the demo a series of tasks run and share a resource. The scheduler prints
 *  out the task state over serial whenever it runs. A watchdog task is 
 *  implemented as well as a poll button task.
 * 
 *  See documentation for further details
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#include "demo.h"

/*==================================
 * Local Globals
 *==================================*/

// TODO move local globals to c files

/* each task sets a flag for the watchdog */
static uint8_t _watchdogFlags;

/*==================================
* Public Functions
*==================================*/

/* ------ Main ------ */

void main(void) {
    bool_t errCode; // TODO do something with this

    _initBtns();
    _initLEDs();
        
    /* set-up the RTOS */
    // TODO currently ignore error code
    ourtosInit(MAX_PRIORITY, SCHEDULER_FREQ);
    errCode = ourtosAddTask(PRIORITY_WATCHDOG, WATCHDOG_TASK_PERIOD, &watchdogKickTask);
    errCode = ourtosAddTask(PRIORITY_POLL_BTN, POLL_BTN_TASK_PERIOD, &pollBtnsTask);
    errCode = ourtosAddTask(PRIORITY_SHORT, SHORT_TASK_PERIOD, &shortBlockingTask);
    errCode = ourtosAddTask(PRIORITY_LONG, LONG_TASK_PERIOD, &longBlockingTask);
    errCode = ourtosAddMutex(PRIORITY_MUTEX, &blockingMutex);
    ourtosEnableDebug(false);

    /* starts the watchdog */
    //_initWatchdog();

    /* start the RTOS. This enables interrupts. */
    ourtosStart();

    for (;;)
    {
        /* infinite loop */
    }

}

/* ------ Tasks ------ */

void watchdogKickTask(void) {

    SET_LEDS(LED_WATCHDOG_KICK);

    if ( WATCHDOG_FLG_ALL_SET == _watchdogFlags ) {
        _FEED_COP();

        /* watchdog clear must be atomic so that tasks setting flags
         * are not intermingled with clearing the flags.
         */
        DisableInterrupts;
        CLEAR_WATCHDOG_FLGS();
        EnableInterrupts;

    } else {
        /* not all of the watchdog flags are set
         * so watchdog cannot be kicked yet 
         */
    }
}

void pollBtnsTask(void) {
    int i;
    bool_t taskEnabled[N_TASKS];

    SET_LEDS(LED_POLL_BTN);

    if ( 1 == GET_MUTEX_DISABLE_BTN() ) {
        DisableInterrupts;
        ourtosEnableMutexes(true);
        EnableInterrupts;
    } else {
        DisableInterrupts;
        ourtosEnableMutexes(false);
        EnableInterrupts;
    }

    /* poll all task enable btns */
    for( i = 0; i < N_TASKS; i++) {
        if ( 0 != GET_TASK_ENABLE_BTN(i) ) {
            taskEnabled[i] = true;
        } else {
            taskEnabled[i] = false;
        }
    }

    /* disable or enable tasks based upon btn */
    DisableInterrupts;
    ourtosEnableTask(PRIORITY_WATCHDOG, taskEnabled[0]);
    ourtosEnableTask(PRIORITY_POLL_BTN, taskEnabled[1]);
    ourtosEnableTask(PRIORITY_SHORT, taskEnabled[2]);
    ourtosEnableTask(PRIORITY_LONG, taskEnabled[3]);
    EnableInterrupts;

    SET_WATCHDOG_FLGS(ID_POLL_BTN);

}

void shortBlockingTask(void) {
    
    SET_LEDS(LED_SHORT_BLK);

    DisableInterrupts;
    ourtosAcquireMutex(&blockingMutex);
    EnableInterrupts;

    _blockingDelayMsec(SHORT_BLOCK_TIME);
    
    DisableInterrupts;
    ourtosReleaseMutex(&blockingMutex);
    EnableInterrupts;

    SET_WATCHDOG_FLGS(ID_SHORT_BLK);
}

void longBlockingTask(void) {

    SET_LEDS(LED_LONG_BLK);

    DisableInterrupts;
    ourtosAcquireMutex(&blockingMutex);
    EnableInterrupts;

    _blockingDelayMsec(LONG_BLOCK_TIME);
    
    DisableInterrupts;
    ourtosReleaseMutex(&blockingMutex);
    EnableInterrupts;

    SET_WATCHDOG_FLGS(ID_LONG_BLK);

}

/*==================================
 * Private Functions
 *==================================*/

/* ------ Interrupt Service Routines ------ */

void interrupt 2 _watchdogISR( void ) {
    
    ourtosShutdown();

    SET_LEDS_OUTPUT();
    SET_LEDS(LED_WATCHDOG);

    for(;;) {
        /* Hang in error state */
    }
}

/* ------ Initialization ------ */

void _initBtns(void) {
    
    /* set mutex disable btn as input */
    SET_MUTEX_DISABLE_BTN_INPUT();
    SET_TASK_ENABLE_BTN_INPUT();

}

void _initLEDs(void) {

    SET_LEDS_OUTPUT();
    SET_LEDS(LED_OFF);

}

void _initWatchdog(void) {

    /* normal mode */
    COPCTL_WCOP = 0;

    /* run in BDM mode */
    COPCTL_RSBCK = 0;

    CLEAR_WATCHDOG_FLGS();

    /* set time period */
    _ENABLE_COP(WATCHDOG_PERIOD);

}

/* ------ Helper functions ------*/

void _blockingDelayMsec(uint16_t delayMS) {
    uint16_t i;

    for (;delayMS > 0; delayMS--) {
        for (i = 0; i < CYCLES_PER_MS; i++) {
            asm NOP;
              asm NOP;
              asm NOP;
              asm NOP;
          }
    }
}