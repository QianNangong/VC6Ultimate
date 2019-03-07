/***
*wcsncoll.c - Collate wide-character locale strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information.
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
*int _wcsncoll() - Collate wide-character locale strings
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       Compares at most n characters of two strings.
*       In the C locale, _wcsncmp() is used to make the comparison.
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

int __cdecl _wcsncoll (
        const wchar_t *_string1,
        const wchar_t *_string2,
        size_t count
        )
{

        int ret;
#ifdef _MT
        pthreadlocinfo ptloci;
#endif  /* _MT */

        if (!count)
            return 0;

        if ( count > INT_MAX ) {
            errno = EINVAL;
            return _NLSCMPERROR;
        }

#ifdef _MT
        ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();
#endif  /* _MT */

#ifdef _MT
        if ( ptloci->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#else  /* _MT */
        if ( __lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#endif  /* _MT */
            return wcsncmp(_string1, _string2, count);

#ifdef _MT
        if ( 0 == (ret = __crtCompareStringW( ptloci->lc_handle[LC_COLLATE],
#else  /* _MT */
        if ( 0 == (ret = __crtCompareStringW( __lc_handle[LC_COLLATE],
#endif  /* _MT */
                                              SORT_STRINGSORT,
                                              _string1,
                                              (int)count,
                                              _string2,
                                              (int)count,
#ifdef _MT
                                              ptloci->lc_collate_cp )) )
#else  /* _MT */
                                              __lc_collate_cp )) )
#endif  /* _MT */
        {
            errno = EINVAL;
            return _NLSCMPERROR;
        }

        return (ret - 2);

}

