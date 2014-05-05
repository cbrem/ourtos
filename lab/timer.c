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
    _currentTime = 0;

    /* set timer overflow interrupt */
    TSCR2_TOI = 1;

    /* set timer prescaler */
    // TODO: this should be a function of period
    TSCR2_PR = 0;
}

void timerEnableInterrupt() {
    TSCR1_TEN = 1;
}

void timerDisableInterrupt() {
    TSCR1_TEN = 0;
}

uint32_t timerGetCurrent() {
    return _currentTime;
}

void timerUpdateCurrent(void) {
    // TODO
}
