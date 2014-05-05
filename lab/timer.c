/* 
 * timer.h - A small timer library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

void timerInit(period_t period) {
    /* Reset current time. */
    currentTime = 0;

    /* enable timer */
    TSCR1_TN = 1;

    /* set timer overflow interrupt */
    TSCR2_TOF = 1;

    /* set timer prescaler */
    // TODO: this should be a function of period
    TSCR2_PR = 0;
}

uint32_t timerGetCurrent() {
    uint32_t timeTmp;

    /* need atomic access */
    DisableInterrupts;
    timeTmp = timeMsec;
    EnableInterrupts;

    return timeTmp;
}

void timerUpdateCurrent(void) {
    // TODO
}
