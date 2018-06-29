/***
*wcsxfrm.c - Transform a wide-character string using locale information
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Transform a wide-character string using the locale information as set by
*       LC_COLLATE.
*
*******************************************************************************/


#include <cruntime.h>
#include <windows.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <stdlib.h>
#include <mtdll.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>

/***
*size_t wcsxfrm() - Transform a string using locale information
*
*Purpose:
*       Transform the wide string pointed to by _string2 and place the
*       resulting wide string into the array pointed to by _string1.
*       No more than _count wide characters are placed into the
*       resulting string (including the null).
*
*       The transformation is such that if wcscmp() is applied to
*       the two transformed strings, the return value is equal to
*       the result of wcscoll() applied to the two original strings.
*       Thus, the conversion must take the locale LC_COLLATE info
*       into account.
*
*       In the C locale, wcsxfrm() simply resolves to wcsncpy()/wcslen().
*
*Entry:
*       wchar_t *_string1       = result string
*       const wchar_t *_string2 = source string
*       size_t _count           = max wide chars to move
*
*       [If _count is 0, _string1 is permitted to be NULL.]
*
*Exit:
*       Length of the transformed string (not including the terminating
*       null).  If the value returned is >= _count, the contents of the
*       _string1 array are indeterminate.
*
*Exceptions:
*       Non-standard: if OM/API error, return INT_MAX.
*
*******************************************************************************/

size_t __cdecl wcsxfrm (
        wchar_t *_string1,
        const wchar_t *_string2,
        size_t _count
        )
{
    int size = INT_MAX;
#ifdef _MT
    _ptiddata ptd;
    pthreadlocinfo ptloci;
#endif  /* _MT */

    if ( _count > INT_MAX )
        return (size_t)size;

#ifdef _MT
    ptd = _getptd();
    ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_COLLATE] == _CLOCALEHANDLE )
    {
        wcsncpy(_string1, _string2, _count);
        return wcslen(_string2);
    }

    if ( 0 == (size = __crtLCMapStringW( __LC_HANDLE(ptloci)[LC_COLLATE],
                                         LCMAP_SORTKEY,
                                         _string2,
                                         -1,
                                         NULL,
                                         0,
                                         __LC_COLLATE_CP(ptloci) )) )
    {
        size = INT_MAX;
    } else
    {
        if ( (size_t)size <= _count)
        {
            if ( 0 == (size = __crtLCMapStringW( __LC_HANDLE(ptloci)[LC_COLLATE],
                                                 LCMAP_SORTKEY,
                                                 _string2,
                                                 -1,
                                                 (wchar_t *)_string1,
                                                 (int)_count,
                                                 __LC_COLLATE_CP(ptloci) )) )
            {
                size = INT_MAX; /* default error */
            }
        }
    }

    return (size_t)size;


}

