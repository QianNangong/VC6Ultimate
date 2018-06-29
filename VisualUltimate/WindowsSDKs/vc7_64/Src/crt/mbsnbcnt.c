/***
*mbsnbcnt.c - Returns byte count of MBCS string
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Returns byte count of MBCS string
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _mbsnbcnt - Returns byte count of MBCS string
*
*Purpose:
*       Returns the number of bytes between the start of the supplied
*       string and the char count supplied.  That is, this routine
*       indicates how many bytes are in the first "ccnt" characters
*       of the string.
*
*Entry:
*       unsigned char *string = pointer to string
*       unsigned int ccnt = number of characters to scan
*
*Exit:
*       Returns number of bytes between string and ccnt.
*
*       If the end of the string is encountered before ccnt chars were
*       scanned, then the length of the string in bytes is returned.
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl _mbsnbcnt(
        const unsigned char *string,
        size_t ccnt
        )
{
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        return __mbsnbcnt_mt(ptmbci, string, ccnt);
}

size_t __cdecl __mbsnbcnt_mt(
        pthreadmbcinfo ptmbci,
        const unsigned char *string,
        size_t ccnt
        )
{
#endif  /* _MT */
        unsigned char *p;

        for (p = (char *)string; (ccnt-- && *p); p++) {
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *p) ) {
#else  /* _MT */
            if ( _ismbblead(*p) ) {
#endif  /* _MT */
                if (*++p == '\0') {
                    --p;
                    break;
                }
            }
        }

        return ((size_t) ((char *)p - (char *)string));
}

#endif  /* _MBCS */
