/***
*difftime.c - return difference between two times as a double
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Find difference between two time in seconds.
*
*******************************************************************************/


#include <cruntime.h>
#include <time.h>

/***
*double difftime(b, a) - find difference between two times
*
*Purpose:
*       returns difference between two times (b-a)
*
*Entry:
*       time_t a, b - times to difference
*
*Exit:
*       returns a double with the time in seconds between two times
*
*Exceptions:
*
*******************************************************************************/

double __cdecl difftime (
        time_t b,
        time_t a
        )
{
        return( (double)( b - a ) );
}

