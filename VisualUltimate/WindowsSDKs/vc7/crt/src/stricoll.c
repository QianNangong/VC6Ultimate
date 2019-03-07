/***
*stricoll.c - Collate locale strings without regard to case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/***
*int _stricoll() - Collate locale strings without regard to case
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information
*       without regard to case.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
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

int __cdecl _stricoll (
        const char *_string1,
        const char *_string2
        )
{

        int ret;
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        if ( ptloci->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#else  /* _MT */
        if ( __lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#endif  /* _MT */
            return _stricmp(_string1, _string2);

#ifdef _MT
        if ( 0 == (ret = __crtCompareStringA( ptloci->lc_handle[LC_COLLATE],
#else  /* _MT */
        if ( 0 == (ret = __crtCompareStringA( __lc_handle[LC_COLLATE],
#endif  /* _MT */
                                              SORT_STRINGSORT | NORM_IGNORECASE,
                                              _string1,
                                              -1,
                                              _string2,
                                              -1,
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
