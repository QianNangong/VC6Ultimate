/***
*time.c - get current system time
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines time() - gets the current system time and converts it to
*                        internal (time_t) format time.
*
*******************************************************************************/


#include <cruntime.h>
#include <time.h>
#include <internal.h>
#include <windows.h>

/*
 * Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
 */
#define EPOCH_BIAS  116444736000000000i64

/*
 * Union to facilitate converting from FILETIME to unsigned __int64
 */
typedef union {
        unsigned __int64 ft_scalar;
        FILETIME ft_struct;
        } FT;

/***
*time_t time(timeptr) - Get current system time and convert to time_t value.
*
*Purpose:
*       Gets the current date and time and stores it in internal (time_t)
*       format. The time is returned and stored via the pointer passed in
*       timeptr. If timeptr == NULL, the time is only returned, not stored in
*       *timeptr. The internal (time_t) format is the number of seconds since
*       00:00:00, Jan 1 1970 (UTC).
*
*       Note: We cannot use GetSystemTime since its return is ambiguous. In
*       Windows NT, in return UTC. In Win32S, probably also Win32C, it
*       returns local time.
*
*Entry:
*       time_t *timeptr - pointer to long to store time in.
*
*Exit:
*       returns the current time.
*
*Exceptions:
*
*******************************************************************************/

time_t __cdecl time (
        time_t *timeptr
        )
{
        time_t tim;
        FT nt_time;

        GetSystemTimeAsFileTime( &(nt_time.ft_struct) );

        tim = (time_t)((nt_time.ft_scalar - EPOCH_BIAS) / 10000000i64);

        if (timeptr)
                *timeptr = tim;         /* store time if requested */

        return tim;
}

