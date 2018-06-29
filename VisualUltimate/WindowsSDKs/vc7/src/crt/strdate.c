/***
*strdate.c - contains the function "_strdate()"
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the function _strdate()
*
*******************************************************************************/


#include <cruntime.h>
#include <tchar.h>
#include <time.h>
#include <oscalls.h>


/***
*_TSCHAR *_strdate(buffer) - return date in string form
*
*Purpose:
*       _strdate() returns a string containing the date in "MM/DD/YY" form
*
*Entry:
*       _TSCHAR *buffer = the address of a 9-byte user buffer
*
*Exit:
*       returns buffer, which contains the date in "MM/DD/YY" form
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tstrdate (
        _TSCHAR *buffer
        )
{
        int month, day, year;
        SYSTEMTIME dt;                  /* Win32 time structure */

        GetLocalTime(&dt);
        month = dt.wMonth;
        day = dt.wDay;
        year = dt.wYear % 100;          /* change year into 0-99 value */

        /* store the components of the date into the string */
        /* store seperators */
        buffer[2] = buffer[5] = _T('/');
        /* store end of string */
        buffer[8] = _T('\0');
        /* store tens of month */
        buffer[0] = (_TSCHAR) (month / 10 + _T('0'));
        /* store units of month */
        buffer[1] = (_TSCHAR) (month % 10 + _T('0'));
        /* store tens of day */
        buffer[3] = (_TSCHAR) (day   / 10 + _T('0'));
        /* store units of day */
        buffer[4] = (_TSCHAR) (day   % 10 + _T('0'));
        /* store tens of year */
        buffer[6] = (_TSCHAR) (year  / 10 + _T('0'));
        /* store units of year */
        buffer[7] = (_TSCHAR) (year  % 10 + _T('0'));

        return buffer;
}

