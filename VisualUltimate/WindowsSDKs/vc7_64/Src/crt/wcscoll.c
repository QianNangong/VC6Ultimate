/***
*wcscoll.c - Collate wide-character locale strings
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information.
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
*int wcscoll() - Collate wide-character locale strings
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information.
*       In the C locale, wcscmp() is used to make the comparison.
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

int __cdecl wcscoll (
        const wchar_t *_string1,
        const wchar_t *_string2
        )
{

    int ret;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_COLLATE] == _CLOCALEHANDLE )
            return (wcscmp(_string1, _string2));

    if ( 0 == (ret = __crtCompareStringW( __LC_HANDLE(ptloci)[LC_COLLATE],
                                          SORT_STRINGSORT,
                                          _string1,
                                          -1,
                                          _string2,
                                          -1,
                                          __LC_COLLATE_CP(ptloci))) )
    {
            errno = EINVAL;
            return _NLSCMPERROR;
    }

    return (ret - 2);

}

