/***
*wcsftime.c - String Format Time
*
*       Copyright (c) 1993-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/


#include <cruntime.h>
#include <internal.h>
#include <mtdll.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#include <dbgint.h>
#include <malloc.h>
#include <excpt.h>

#ifdef _MT
size_t __cdecl _Strftime_mt (pthreadlocinfo ptloci, char *string, size_t maxsize,
        const char *format, const struct tm *timeptr, void *lc_time_arg);
#else  /* _MT */
size_t __cdecl _Strftime (char *string, size_t maxsize, const char *format,
        const struct tm *timeptr, void *lc_time_arg);
#endif  /* _MT */


/***
*size_t wcsftime(wstring, maxsize, format, timeptr) - Format a time string
*
*Purpose:
*       The wcsftime functions is equivalent to to the strftime function, except
*       that the argument 'wstring' specifies an array of a wide string into
*       which the generated output is to be placed. The wcsftime acts as if
*       strftime were called and the result string converted by mbstowcs().
*       [ISO]
*
*Entry:
*       wchar_t *wstring = pointer to output string
*       size_t maxsize = max length of string
*       const wchar_t *format = format control string
*       const struct tm *timeptr = pointer to tb data structure
*
*Exit:
*       !0 = If the total number of resulting characters including the
*       terminating null is not more than 'maxsize', then return the
*       number of wide chars placed in the 'wstring' array (not including the
*       null terminator).
*
*       0 = Otherwise, return 0 and the contents of the string are
*       indeterminate.
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl wcsftime (
        wchar_t *wstring,
        size_t maxsize,
        const wchar_t *wformat,
        const struct tm *timeptr
        )
{
    size_t retval = 0;
    char *format = NULL;
    char *string = NULL;
    size_t flen = wcslen(wformat) + 1;
    int malloc_flag1 = 0;
    int malloc_flag2 = 0;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    __try {
        string = (char *)_alloca(sizeof(char) * maxsize * 2);
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
        _resetstkoflw();
        string = NULL;
    }

    if ( string == NULL ) {
        if ((string = (char *)_malloc_crt(sizeof(char) * maxsize * 2)) == NULL)
            return 0;
        else
            malloc_flag1++;
    }

    __try {
        format = (char *)_alloca(sizeof(char) * flen * 2);
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
        _resetstkoflw();
        format = NULL;
    }

    if ( format == NULL ) {
        if ((format = (char *)_malloc_crt(sizeof(char) * flen * 2)) == NULL)
            goto done;
        else
            malloc_flag2++;
    }

#ifdef _MT
    if (__wcstombs_mt(ptloci, format, wformat, flen * 2) == -1)
#else  /* _MT */
    if (wcstombs(format, wformat, flen * 2) == -1)
#endif  /* _MT */
        goto done;

#ifdef _MT
    if (_Strftime_mt(ptloci, string, maxsize * 2, format, timeptr, 0))
#else  /* _MT */
    if (_Strftime(string, maxsize * 2, format, timeptr, 0))
#endif  /* _MT */
    {
#ifdef _MT
        if ((retval = __mbstowcs_mt(ptloci, wstring, string, maxsize))
            == -1)
#else  /* _MT */
        if ((retval = mbstowcs(wstring, string, maxsize)) == -1)
#endif  /* _MT */
            retval = 0;
    }

done:
    if ( malloc_flag1 )
        _free_crt(string);

    if ( malloc_flag2 )
        _free_crt(format);

    return retval;
}

