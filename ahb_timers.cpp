
#include "ahb_timers.h"


// TODO: set number of timers via COInit
Timer<MAX_NB_TIMER> t;

TIMER_HANDLE SetAlarm(UNS8 id, timerCallback_t callback, TIMEVAL value, TIMEVAL period) {
    if(period) return t.every(period, callback, id);
    else return t.after(value, callback, id);
}

TIMER_HANDLE DelAlarm(TIMER_HANDLE handle) {
    t.stop(handle);
    return TIMER_NONE;
}

void TimeDispatch(void) {
    t.update();
}
