/***
*mbsicmp.c - Case-insensitive string comparision routine (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Case-insensitive string comparision routine (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <string.h>
#include <mbstring.h>

/***
* _mbsicmp - Case-insensitive string comparision routine (MBCS)
*
*Purpose:
*       Compares two strings for lexical order without regard to case.
*       Strings are compared on a character basis, not a byte basis.
*
*Entry:
*       char *s1, *s2 = strings to compare
*
*Exit:
*       returns <0 if s1 < s2
*       returns  0 if s1 == s2
*       returns >0 if s1 > s2
*       returns _NLSCMPERROR if NLS error
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _mbsicmp(
        const unsigned char *s1,
        const unsigned char *s2
        )
{
        unsigned short c1, c2;
        int    retval;
        unsigned char szResult[4];
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return _stricmp(s1, s2);

        for (;;)
        {
            c1 = *s1++;
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, c1) )
#else  /* _MT */
            if ( _ismbblead(c1) )
#endif  /* _MT */
            {
                if (*s1 == '\0')
                    c1 = 0;
                else
                {
#ifdef _MT
                    retval = __crtLCMapStringA( ptmbci->mblcid, LCMAP_UPPERCASE,
                                                s1 - 1, 2, szResult, 2,
                                                ptmbci->mbcodepage, TRUE );
#else  /* _MT */
                    retval = __crtLCMapStringA( __mblcid, LCMAP_UPPERCASE,
                                                s1 - 1, 2, szResult, 2,
                                                __mbcodepage, TRUE );
#endif  /* _MT */
                    if (retval == 1)
                        c1 = szResult[0];
                    else if (retval == 2)
                        c1 = (szResult[0] << 8) + szResult[1];
                    else
                        return _NLSCMPERROR;
                    s1++;
                }
            }
            else
#ifdef _MT
                c1 = __mbbtolower_mt(ptmbci, c1);
#else  /* _MT */
                c1 = _mbbtolower(c1);
#endif  /* _MT */

            c2 = *s2++;
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, c2) )
#else  /* _MT */
            if ( _ismbblead(c2) )
#endif  /* _MT */
            {
                if (*s2 == '\0')
                    c2 = 0;
                else
                {
#ifdef _MT
                    retval = __crtLCMapStringA( ptmbci->mblcid, LCMAP_UPPERCASE,
                                                s2 - 1, 2, szResult, 2,
                                                ptmbci->mbcodepage, TRUE );
#else  /* _MT */
                    retval = __crtLCMapStringA( __mblcid, LCMAP_UPPERCASE,
                                                s2 - 1, 2, szResult, 2,
                                                __mbcodepage, TRUE );
#endif  /* _MT */
                    if (retval == 1)
                        c2 = szResult[0];
                    else if (retval == 2)
                        c2 = (szResult[0] << 8) + szResult[1];
                    else
                        return _NLSCMPERROR;
                    s2++;
                }
            }
            else
#ifdef _MT
                c2 = __mbbtolower_mt(ptmbci, c2);
#else  /* _MT */
                c2 = _mbbtolower(c2);
#endif  /* _MT */

            if (c1 != c2)
                return( (c1 > c2) ? 1 : -1 );

            if (c1 == 0)
                return(0);
        }
}

#endif  /* _MBCS */
