/***
*mbsnbico.c - Collate n bytes of strings, ignoring case (MBCS)
*
*       Copyright (c) 1994-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Collate n bytes of strings, ignoring case (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <internal.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _mbsnbicoll - Collate n bytes of strings, ignoring case (MBCS)
*
*Purpose:
*       Collates up to n bytes of two strings for lexical order.
*       Strings are collated on a character basis, not a byte basis.
*       Case of characters is not considered.
*
*Entry:
*       unsigned char *s1, *s2 = strings to collate
*       size_t n = maximum number of bytes to collate
*
*Exit:
*       returns <0 if s1 < s2
*       returns  0 if s1 == s2
*       returns >0 if s1 > s2
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _mbsnbicoll(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n
        )
{
        int ret;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();
#endif  /* _MT */

        if (n == 0)
            return 0;

#ifdef _MT
        if ( 0 == (ret = __crtCompareStringA( ptmbci->mblcid,
#else  /* _MT */
        if ( 0 == (ret = __crtCompareStringA( __mblcid,
#endif  /* _MT */
                                              NORM_IGNORECASE,
                                              s1,
                                              (int)n,
                                              s2,
                                              (int)n,
#ifdef _MT
                                              ptmbci->mbcodepage )) )
#else  /* _MT */
                                              __mbcodepage )) )
#endif  /* _MT */
            return _NLSCMPERROR;

        return ret - 2;

}

#endif  /* _MBCS */
