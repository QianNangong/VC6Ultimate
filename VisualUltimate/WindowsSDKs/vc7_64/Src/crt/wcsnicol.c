/***
*wcsnicoll.c - Collate wide-character locale strings without regard to case
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*       Compares at most n characters of two strings.
*
*******************************************************************************/


#include <cruntime.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/***
*int _wcsnicoll() - Collate wide-character locale strings without regard to case
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*       Compares at most n characters of two strings.
*       In the C locale, _wcsicmp() is used to make the comparison.
*
*Entry:
*       const wchar_t *s1 = pointer to the first string
*       const wchar_t *s2 = pointer to the second string
*       size_t count - maximum number of characters to compare
*
*Exit:
*       -1 = first string less than second string
*        0 = strings are equal
*        1 = first string greater than second string
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*       _NLSCMPERROR    = error
*       errno = EINVAL
*
*******************************************************************************/

int __cdecl _wcsnicoll (
        const wchar_t *_string1,
        const wchar_t *_string2,
        size_t count
        )
{

    int ret;
#ifdef _MT
    _ptiddata ptd;
    pthreadlocinfo ptloci;
#endif  /* _MT */

    if (!count)
        return 0;

    if ( count > INT_MAX ) {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

#ifdef _MT
    ptd = _getptd();
    ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    if ( __LC_HANDLE(ptloci)[LC_COLLATE] == _CLOCALEHANDLE ) {
        wchar_t f, l;

        do {
            f = __ascii_towlower(*_string1);
            l = __ascii_towlower(*_string2);
            _string1++;
            _string2++;
        } while ( (--count) && f && (f == l) );

        return (int)(f - l);
    }

    if ( 0 == (ret = __crtCompareStringW( __LC_HANDLE(ptloci)[LC_COLLATE],
                                          SORT_STRINGSORT | NORM_IGNORECASE,
                                          _string1,
                                          (int)count,
                                          _string2,
                                          (int)count,
                                          __LC_COLLATE_CP(ptloci) )) )
    {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    return (ret - 2);

}

