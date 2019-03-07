/***
*xstrcoll.c - Collate locale strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <xlocinfo.h>   /* for _Collvec, _Strcoll */
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/* Define _CRTIMP2 */
#ifndef _CRTIMP2
#ifdef CRTDLL2
#define _CRTIMP2 __declspec(dllexport)
#else  /* CRTDLL2 */
#ifdef _DLL
#define _CRTIMP2 __declspec(dllimport)
#else  /* _DLL */
#define _CRTIMP2
#endif  /* _DLL */
#endif  /* CRTDLL2 */
#endif  /* _CRTIMP2 */

/***
*int _Strcoll() - Collate locale strings
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       [ANSI].
*
*       Non-C locale support available under _INTL switch.
*       In the C locale, strcoll() simply resolves to strcmp().
*Entry:
*       const char *s1b = pointer to beginning of the first string
*       const char *s1e = pointer past end of the first string
*       const char *s2b = pointer to beginning of the second string
*       const char *s1e = pointer past end of the second string
*       const _Collvec *ploc = pointer to locale info
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

_CRTIMP2 int __cdecl _Strcoll (
        const char *_string1,
        const char *_end1,
        const char *_string2,
        const char *_end2,
        const _Collvec *ploc
        )
{
        int ret;
        LCID handle;
#ifdef _MT
        int local_lock_flag;
#endif  /* _MT */
        int n1 = (int)(_end1 - _string1);
        int n2 = (int)(_end2 - _string2);

        _lock_locale( local_lock_flag )
        __TRY

            if (ploc == 0)
                handle = ___lc_handle_func()[LC_COLLATE];
            else
                handle = ploc->_Hand;

            if (handle == _CLOCALEHANDLE) {
                int ans;
                ans = memcmp(_string1, _string2, n1 < n2 ? n1 : n2);
                return ans != 0 || n1 == n2 ? ans : n1 < n2 ? -1 : +1;
            }

            if ( 0 == (ret = __crtCompareStringA( handle,
                                                  0,
                                                  _string1,
                                                  n1,
                                                  _string2,
                                                  n2,
                                                  ___lc_collate_cp_func() )) )
                goto error_cleanup;

        __FINALLY
            _unlock_locale( local_lock_flag )
        __END_TRY_FINALLY
        return (ret - 2);

error_cleanup:

        errno = EINVAL;
        return _NLSCMPERROR;
}


/***
*_Collvec _Getcoll() - get collation info for current locale
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP2 _Collvec __cdecl _Getcoll()
{
        _Collvec coll;
#ifdef _MT
        int local_lock_flag;
#endif  /* _MT */
        _lock_locale( local_lock_flag )
        __TRY
            coll._Hand = ___lc_handle_func()[LC_COLLATE];
            coll._Page = ___lc_collate_cp_func();
        __FINALLY
            _unlock_locale( local_lock_flag )
        __END_TRY_FINALLY

        return (coll);
}
