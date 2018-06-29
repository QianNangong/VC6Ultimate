/***
*mbsrchr.c - Search for last occurence of character (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Search for last occurence of character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <stddef.h>


/***
* _mbsrchr - Search for last occurence of character (MBCS)
*
*Purpose:
*       Find the last occurrence of the specified character in
*       the supplied string.  Handles MBCS chars/strings correctly.
*
*Entry:
*       unsigned char *str = string to search in
*       unsigned int c = character to search for
*
*Exit:
*       returns pointer to last occurrence of c in str
*       returns NULL if c not found
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbsrchr(
        const unsigned char *str,
        unsigned int c
        )
{
        char *r = NULL;
        unsigned int cc;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return strrchr(str, c);

        do {
            cc = *str;
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, cc) ) {
#else  /* _MT */
            if ( _ismbblead(cc) ) {
#endif  /* _MT */
                if(*++str) {
                    if (c == ((cc<<8)|*str))
                        r = (char *)str - 1;
                }
                else if(!r)
                    /* return pointer to '\0' */
                    r = (char *)str;
            }
            else if (c == cc)
                r = (char *)str;
        }
        while (*str++);

        return(r);
}

#endif  /* _MBCS */
