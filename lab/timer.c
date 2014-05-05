/* 
 * timer.h - A small timer library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

/*==================================
 * Public Functions
 *==================================*/

void timerInit(freq_t freq) {
    /* from modclock - set the module to 8MHz */
    clockSetup();

    /* Reset current time and time count. */
    _timeCurrentMsec = 0;
    _timeCountMsec = 0;

    /* enable timer */
    TSCR1_TN = 1;

    /* set timer overflow interrupt */
    TSCR2_TOF = 1;

    /* set timer prescaler */
    TSCR2_PR = _freq2Prescaler(period);
}

uint32_t timerGetCurrentMsec() {
    return _timeCurrentMsec;
}

void timerUpdateCurrent(void) {
    /* increment timer based on 8.24 fixed point calculation */
    _timeCountMsec += _timeIncrement;
    
    /* _timeCurrentMsec gets the top 8 bits of timeCount added to it */
    _timeCurrentMsec += (_timeCountMsec & TIME_COUNT_MSEC_MASK) >> TIME_COUNT_N_FRAC_BITS;
    
    /* _timeCountMsec is masked to remove the seconds which are now in the tineCurrentMsec var */
    _timeCountMsec &= TIME_COUNT_FRAC_SEC_MASK;
}

/*==================================
 * Private Functions
 *==================================*/

static uint8_t _periodPrescaler(freq_t freq) {
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
