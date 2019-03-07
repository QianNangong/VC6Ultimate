/***
*strxfrm.c - Transform a string using locale information
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Transform a string using the locale information as set by
*       LC_COLLATE.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <awint.h>
#include <mtdll.h>

/***
*size_t strxfrm() - Transform a string using locale information
*
*Purpose:
*       Transform the string pointer to by _string2 and place the
*       resulting string into the array pointer to by _string1.
*       No more than _count characters are place into the
*       resulting string (including the null).
*
*       The transformation is such that if strcmp() is applied to
*       the two transformed strings, the return value is equal to
*       the result of strcoll() applied to the two original strings.
*       Thus, the conversion must take the locale LC_COLLATE info
*       into account.
*       [ANSI]
*
*       The value of the following expression is the size of the array
*       needed to hold the transformation of the source string:
*
*           1 + strxfrm(NULL,string,0)
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*       Thus, strxfrm() simply resolves to strncpy()/strlen().
*
*Entry:
*       char *_string1       = result string
*       const char *_string2 = source string
*       size_t _count        = max chars to move
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

size_t __cdecl strxfrm (
        char *_string1,
        const char *_string2,
        size_t _count
        )
{
        int dstlen;
        int retval = INT_MAX;   /* NON-ANSI: default if OM or API error */
#ifdef _MT
        pthreadlocinfo ptloci;
#endif  /* _MT */

        if ( _count > INT_MAX )
            return (size_t)retval;

#ifdef _MT
        ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        if ( (ptloci->lc_handle[LC_COLLATE] == _CLOCALEHANDLE) &&
             (ptloci->lc_collate_cp == _CLOCALECP) )
#else  /* _MT */

        if ( (__lc_handle[LC_COLLATE] == _CLOCALEHANDLE) &&
             (__lc_collate_cp == _CLOCALECP) )
#endif  /* _MT */
        {
            strncpy(_string1, _string2, _count);
            return strlen(_string2);
        }

        /* Inquire size of dst string in BYTES */
#ifdef _MT
        if ( 0 == (dstlen = __crtLCMapStringA( ptloci->lc_handle[LC_COLLATE],
#else  /* _MT */
        if ( 0 == (dstlen = __crtLCMapStringA( __lc_handle[LC_COLLATE],
#endif  /* _MT */
                                               LCMAP_SORTKEY,
                                               _string2,
                                               -1,
                                               NULL,
                                               0,
#ifdef _MT
                                               ptloci->lc_collate_cp,
#else  /* _MT */
                                               __lc_collate_cp,
#endif  /* _MT */
                                               TRUE )) )
            goto error_cleanup;

        retval = dstlen;

        /* if not enough room, return amount needed */
        if ( dstlen > (int)_count )
            goto error_cleanup;

        /* Map src string to dst string */
#ifdef _MT
        if ( 0 == __crtLCMapStringA( ptloci->lc_handle[LC_COLLATE],
#else  /* _MT */
        if ( 0 == __crtLCMapStringA( __lc_handle[LC_COLLATE],
#endif  /* _MT */
                                     LCMAP_SORTKEY,
                                     _string2,
                                     -1,
                                     _string1,
                                     (int)_count,
#ifdef _MT
                                     ptloci->lc_collate_cp,
#else  /* _MT */
                                     __lc_collate_cp,
#endif  /* _MT */
                                     TRUE ) )
            retval = INT_MAX;

error_cleanup:
        return (size_t)retval;
}
