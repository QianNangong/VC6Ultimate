/***
*mbsspn.c - Search for init substring of chars from control string (MBCS).
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Search for init substring of chars from control string (MBCS).
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
#include <tchar.h>


/***
*ifndef _RETURN_PTR
* _mbsspn - Find first string char not in charset (MBCS)
*else
* _mbsspnp - Find first string char not in charset, return pointer (MBCS)
*endif
*
*Purpose:
*       Returns maximum leading segment of string consisting solely
*       of characters from charset.  Handles MBCS characters correctly.
*
*Entry:
*       unsigned char *string = string to search in
*       unsigned char *charset = set of characters to scan over
*
*Exit:
*
*ifndef _RETURN_PTR
*       Returns index of first char in string not in control.
*       Returns 0, if string begins with a character not in charset.
*else
*       Returns pointer to first character not in charset.
*       Returns NULL if string consists entirely of characters from charset.
*endif
*
*Exceptions:
*
*******************************************************************************/

#ifndef _RETURN_PTR

size_t __cdecl _mbsspn(
        const unsigned char *string,
        const unsigned char *charset
        )

#else  /* _RETURN_PTR */

unsigned char * __cdecl _mbsspnp(
        const unsigned char *string,
        const unsigned char *charset
        )

#endif  /* _RETURN_PTR */

{
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

#ifndef _RETURN_PTR
        return __mbsspn_mt(ptmbci, string, charset);
#else  /* _RETURN_PTR */
        return __mbsspnp_mt(ptmbci, string, charset);
#endif  /* _RETURN_PTR */
}

#ifndef _RETURN_PTR

size_t __cdecl __mbsspn_mt(
        pthreadmbcinfo ptmbci,
        const unsigned char *string,
        const unsigned char *charset
        )

#else  /* _RETURN_PTR */

unsigned char * __cdecl __mbsspnp_mt(
        pthreadmbcinfo ptmbci,
        const unsigned char *string,
        const unsigned char *charset
        )

#endif  /* _RETURN_PTR */

{
#endif  /* _MT */

        unsigned char *p, *q;

#ifndef _RETURN_PTR
#ifdef _MT
        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return strspn(string, charset);
#else  /* _RETURN_PTR */
#ifdef _MT
        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
        {
            size_t retval;
            retval = strspn(string, charset);
            return (unsigned char *)(*(string + retval) ? string + retval : NULL);
        }
#endif  /* _RETURN_PTR */

        /* loop through the string to be inspected */
        for (q = (char *)string; *q; q++) {

            /* loop through the charset */
            for (p = (char *)charset; *p; p++) {
#ifdef _MT
                if ( __ismbblead_mt(ptmbci, *p) ) {
#else  /* _MT */
                if ( _ismbblead(*p) ) {
#endif  /* _MT */
                    if (((*p == *q) && (p[1] == q[1])) || p[1] == '\0')
                        break;
                    p++;
                }
                else
                    if (*p == *q)
                        break;
            }

            if (*p == '\0')         /* end of charset? */
                break;              /* yes, no match on this char */

#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *q) )
#else  /* _MT */
            if ( _ismbblead(*q) )
#endif  /* _MT */
                if (*++q == '\0')
                    break;
        }

#ifndef _RETURN_PTR
        return((size_t) (q - string));          /* index */
#else  /* _RETURN_PTR */
        return((*q) ? q : NULL);        /* pointer */
#endif  /* _RETURN_PTR */

}

#endif  /* _MBCS */
