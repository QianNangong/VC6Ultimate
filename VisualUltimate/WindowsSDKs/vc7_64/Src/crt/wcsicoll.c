/***
*wcsicoll.c - Collate wide-character locale strings without regard to case
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*
*******************************************************************************/


#include <cruntime.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/***
*int _wcsicoll() - Collate wide-character locale strings without regard to case
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*       In the C locale, _wcsicmp() is used to make the comparison.
*
*Entry:
*       const wchar_t *s1 = pointer to the first string
*       const wchar_t *s2 = pointer to the second string
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

int __cdecl _wcsicoll (
        const wchar_t *_string1,
        const wchar_t *_string2
        )
{
    int ret;
    wchar_t f, l;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_COLLATE] == _CLOCALEHANDLE ) {
        do  {
            f = __ascii_towlower(*_string1);
            l = __ascii_towlower(*_string2);
            _string1++;
            _string2++;
        } while ( (f) && (f == l) );

        return (int)(f - l);
    }

    if ( 0 == (ret = __crtCompareStringW( __LC_HANDLE(ptloci)[LC_COLLATE],
                                          SORT_STRINGSORT | NORM_IGNORECASE,
                                          _string1,
                                          -1,
                                          _string2,
                                          -1,
                                          __LC_CODEPAGE(ptloci))) )
    {
            errno = EINVAL;
            return _NLSCMPERROR;
    }

    return (ret - 2);

}

