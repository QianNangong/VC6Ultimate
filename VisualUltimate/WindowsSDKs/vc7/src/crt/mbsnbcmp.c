/***
*mbsnbcmp.c - Compare n bytes of two MBCS strings
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Compare n bytes of two MBCS strings
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <string.h>
#include <mbstring.h>

/***
*int mbsnbcmp(s1, s2, n) - Compare n bytes of two MBCS strings
*
*Purpose:
*       Compares up to n bytes of two strings for lexical order.
*
*Entry:
*       unsigned char *s1, *s2 = strings to compare
*       size_t n = maximum number of bytes to compare
*
*Exit:
*       returns <0 if s1 < s2
*       returns  0 if s1 == s2
*       returns >0 if s1 > s2
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _mbsnbcmp(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n
        )
{
        unsigned short c1, c2;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;
#endif  /* _MT */

        if (n==0)
                return(0);

#ifdef _MT
        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return strncmp(s1, s2, n);

        while (n--) {

            c1 = *s1++;
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, c1) )
#else  /* _MT */
            if ( _ismbblead(c1) )
#endif  /* _MT */
            {
                if (n==0)
                {
                    c1 = 0; /* 'naked' lead - end of string */
#ifdef _MT
                    c2 = __ismbblead_mt(ptmbci, *s2) ? 0 : *s2;
#else  /* _MT */
                    c2 = _ismbblead(*s2) ? 0 : *s2;
#endif  /* _MT */
                    goto test;
                }
                c1 = ( (*s1 == '\0') ? 0 : ((c1<<8) | *s1++) );
            }

            c2 = *s2++;
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, c2) )
#else  /* _MT */
            if ( _ismbblead(c2) )
#endif  /* _MT */
            {
                if (n==0)
                {
                    c2 = 0; /* 'naked' lead - end of string */
                    goto test;
                }
                --n;
                c2 = ( (*s2 == '\0') ? 0 : ((c2<<8) | *s2++) );
            }
test:
            if (c1 != c2)
                return( (c1 > c2) ? 1 : -1);

            if (c1 == 0)
                return(0);
        }

        return(0);
}

#endif  /* _MBCS */
