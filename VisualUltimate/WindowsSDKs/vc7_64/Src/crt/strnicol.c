/***
*strnicoll.c - Collate locale strings without regard to case
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/***
*int _strnicoll() - Collate locale strings without regard to case
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information
*       without regard to case.
*       Compares at most n characters of two strings.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
*       size_t count - maximum number of characters to compare
*
*Exit:
*       Less than 0    = first string less than second string
*       0              = strings are equal
*       Greater than 0 = first string greater than second string
*
*Exceptions:
*       _NLSCMPERROR    = error
*       errno = EINVAL
*
*******************************************************************************/

int __cdecl _strnicoll (
        const char *_string1,
        const char *_string2,
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

    if ( __LC_HANDLE(ptloci)[LC_COLLATE] == _CLOCALEHANDLE )
        return _strnicmp(_string1, _string2, count);

    if ( 0 == (ret = __crtCompareStringA( __LC_HANDLE(ptloci)[LC_COLLATE],
                                          SORT_STRINGSORT | NORM_IGNORECASE,
                                          _string1,
                                          (int)count,
                                          _string2,
                                          (int)count,
                                          __LC_COLLATE_CP(ptloci))) )
    {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    return (ret - 2);

}
