/* Demo of kronOS
 *  This file demonstrates the functionality of kronOS. kronOS is a lightweight 
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
* Public Functions
*==================================*/

/* ------ Main ------ */

void main(void) {

    /* from modclock - set the module to 8MHz */
    clockSetup();
   
    _initBtns();
    _initLEDs();
    
    /* set-up the RTOS */
    // TODO kronosSetSchedulerPeriod(SCHEDULER_PERIOD);
    // TODO kronosSetTaskArray(PCB, N_TASKS);
    // TODO kronosAddTask(PRIORITY_WATCHDOG, WATCHDOG_TASK_PERIOD, &watchdogKickTask);
    // TODO kronosAddTask(PRIORITY_POLL_BTN, POLL_BTN_TASK_PERIOD, &pollBtnsTask);
    // TODO kronosAddTask(PRIORITY_SHORT, SHORT_TASK_PERIOD, &shortBlockingTask);
    // TODO kronosAddTask(PRIORITY_LONG, LONG_TASK_PERIOD, &longBlockingTask);
    // TODO kronosAddMutex(PRIORITY_MUTEX, &blockingMutex);
    // TODO kronosEnableMutexes(true);
    // TODO kronosEnableDebug(true);

    /* starts the watchdog */
    _initWatchdog();

    /* start the RTOS */
    // TODO kronosStart();


    for (;;)
    {
        /* infinite loop */

        // TODO temp test of functionality with cyclic exec
        pollBtnsTask();
        shortBlockingTask();
        longBlockingTask();
        watchdogKickTask();

    }

}

/* ------ Tasks ------ */

void watchdogKickTask(void) {

    SET_LEDS(LED_WATCHDOG_KICK);

    if ( WATCHDOG_FLG_ALL_SET == watchdogFlags ) {
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

    SET_LEDS(LED_POLL_BTN);

    if ( 1 == GET_MUTEX_DISABLE_BTN() ) {
        // kronosEnableMutexes(true) /* atomic */
    } else {
        // kronosEnableMutexes(false) /* atomic */
    }

    /* poll all task enable btns */
    for( i = 0; i < N_TASKS; i++) {
        if ( 0 != GET_TASK_ENABLE_BTN(i) ) {
            // kronosEnableTask(i, true);
        } else {
            // kronosEnableTask(i, false);
        }
    }

    SET_WATCHDOG_FLGS(ID_POLL_BTN);

}

void shortBlockingTask(void) {
    
    SET_LEDS(LED_SHORT_BLK);

    //kronosAcquireMutex(&blockingMutex);
    _blockingDelayMsec(SHORT_BLOCK_TIME);
    //kronosReleaseMutex(&blockingMutex);

    SET_WATCHDOG_FLGS(ID_SHORT_BLK);
}

void longBlockingTask(void) {

    SET_LEDS(LED_LONG_BLK);

    //kronosAcquireMutex(&blockingMutex);
    _blockingDelayMsec(LONG_BLOCK_TIME);
    //kronosReleaseMutex(&blockingMutex);

    SET_WATCHDOG_FLGS(ID_LONG_BLK);

}

/*==================================
 * Private Functions
 *==================================*/

/* ------ Interrupt Service Routines ------ */

void interrupt 2 _watchdogISR( void ) {
    
    SET_LEDS_OUTPUT();
    SET_LEDS(LED_WATCHDOG);

    for(;;) {
        /* Hang in error state */
    }
    // TODO restart?

}

/* ------ Initialization ------ */

void _initBtns(void) {
    
    int i;

    /* set mutex disable btn as input */
    SET_MUTEX_DISABLE_BTN_INPUT();
    SET_TASK_ENABLE_BTN_INPUT();

    /* set globals false */
    mutexDisableBtn = false;
    for(i = 0; i < N_TASKS; i++) {
        taskEnableBtn[i] = false;
    }

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