
#ifndef __CO_timer_h__
#define __CO_timer_h__

#include "Timer.h"

#define INTEGER8 signed char
#define UNS8   unsigned char
#define UNS32  unsigned long

#define MAX_NB_TIMER 15

#define TIMER_HANDLE INTEGER8
#define TIMEVAL UNS32
#define TIMER_NONE -1
#define MS_TO_TIMEVAL(ms) (ms)


/* ---------  prototypes --------- */
/*#define SetAlarm(id, callback, value, period) printf("%s, %d, SetAlarm(%s, %s, %s, %s, %s)\n",__FILE__, __LINE__, #d, #id, #callback, #value, #period); _SetAlarm(id, callback, value, period)*/
/**
 * @ingroup timer
 * @brief Set an alarm to execute a callback function when expired.
 * @param *d Pointer to a CAN object data structure
 * @param id The alarm Id
 * @param callback A callback function
 * @param value Call the callback function at current time + value
 * @param period Call periodically the callback function
 * @return handle The timer handle
 */
TIMER_HANDLE SetAlarm(UNS8 id, timerCallback_t callback, TIMEVAL value, TIMEVAL period);

/**
 * @ingroup timer
 * @brief Delete an alarm before expiring.
 * @param handle A timer handle
 * @return The timer handle
 */
TIMER_HANDLE DelAlarm(TIMER_HANDLE handle);

void TimeDispatch(void);


#endif /* #define __timer_h__ */
