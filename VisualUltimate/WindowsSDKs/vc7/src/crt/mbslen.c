/***
*mbslen.c - Find length of MBCS string
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Find length of MBCS string
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _mbslen - Find length of MBCS string
*
*Purpose:
*       Find the length of the MBCS string (in characters).
*
*Entry:
*       unsigned char *s = string
*
*Exit:
*       Returns the number of MBCS chars in the string
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl _mbslen(
        const unsigned char *s
        )
{
        int n;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return strlen(s);

        for (n = 0; *s; n++, s++) {
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *s) ) {
#else  /* _MT */
            if ( _ismbblead(*s) ) {
#endif  /* _MT */
                if (*++s == '\0')
                    break;
            }
        }

        return(n);
}

#endif  /* _MBCS */
