/* 
 * timer.h - A small timer library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#include "timer.h"

/*==================================
 * Local Globals
 *==================================*/

/* Current time in ms */
static uint32_t _timeCurrentMsec;

/* Current timer counter (ms) in 8.24 fixed point */
static uint32_t _timeCountMsec;

/* timeIncrement is the value to increment the timer counter by each rollover */
static uint32_t _timeIncrement;

/* timestamp for use in elapsed time */
static uint32_t _lastTimestamp;

/*==================================
 * Public Functions
 *==================================*/

void timerInit(freq_t freq) {
    /* from modclock - set the module to 8MHz */
    clockSetup();

    /* Reset current time, time count and last time stamp */
    _timeCurrentMsec = 0;
    _timeCountMsec = 0;
    _lastTimestamp = 0;

    /* set timer overflow interrupt */
    TSCR2_TOI = 1;

    /* set timer prescaler */
    TSCR2_PR = _freq2Prescaler(freq);

}

uint32_t timerGetCurrentMsec(void) {
    return _timeCurrentMsec;
}

void timerEnableInterrupt() {
    /* atomic */
    TSCR1_TEN = 1;
}

void timerDisableInterrupt() {
    /* atomic */
    TSCR1_TEN = 0;
}

void timerUpdateCurrent(void) {
    /* increment timer based on 8.24 fixed point calculation */
    _timeCountMsec += _timeIncrement;
    
    /* _timeCurrentMsec gets the top 8 bits of timeCount added to it */
    _timeCurrentMsec += (_timeCountMsec & TIME_COUNT_MSEC_MASK) >> TIME_COUNT_N_FRAC_BITS;
    
    /* _timeCountMsec is masked to remove the seconds which are now in the tineCurrentMsec var */
    _timeCountMsec &= TIME_COUNT_FRAC_SEC_MASK;
}

int32_t timerElapsedTime(void) {
    static uint32_t curTime;
    static int32_t elapsedTime;

    /* this cast is the undefined behavior */
    curTime = timerGetCurrentMsec();
    elapsedTime = (int32_t)(curTime - _lastTimestamp);
    /* update the global last timestamp */
    _lastTimestamp = curTime;
    return elapsedTime;
}

void timerBlockingDelayMsec(uint16_t delayMS) {
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

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _freq2Prescaler(freq_t freq) {
    uint8_t prescaler = 0;

    switch(freq) {
    case FREQ_8_MHZ:      
        prescaler = FREQ_8_MHZ_PRESCALER;
        _timeIncrement = FREQ_8_MHZ_INCR;
        break;
    case FREQ_4_MHZ:      
        prescaler = FREQ_4_MHZ_PRESCALER;
        _timeIncrement = FREQ_4_MHZ_INCR;
        break;
    case FREQ_2_MHZ:      
        prescaler = FREQ_2_MHZ_PRESCALER;
        _timeIncrement = FREQ_2_MHZ_INCR;
        break;
    case FREQ_1_MHZ:      
        prescaler = FREQ_1_MHZ_PRESCALER;
        _timeIncrement = FREQ_1_MHZ_INCR;
        break;
    }

    return prescaler & THREE_BIT_MASK;
}

