/***
*mbscspn.c - Find first string char in charset (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Find first string char in charset (MBCS)
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
*ifndef _RETURN_PTR
* _mbscspn - Find first string char in charset (MBCS)
*else
* _mbspbrk - Find first string char in charset, pointer return (MBCS)
*endif
*
*Purpose:
*       Returns maximum leading segment of string
*       which consists solely of characters NOT from charset.
*       Handles MBCS chars correctly.
*
*Entry:
*       char *string = string to search in
*       char *charset = set of characters to scan over
*
*Exit:
*
*ifndef _RETURN_PTR
*       Returns the index of the first char in string
*       that is in the set of characters specified by control.
*
*       Returns 0, if string begins with a character in charset.
*else
*       Returns pointer to first character in charset.
*
*       Returns NULL if string consists entirely of characters
*       not from charset.
*endif
*
*Exceptions:
*
*******************************************************************************/

#ifndef _RETURN_PTR

size_t __cdecl _mbscspn(
        const unsigned char *string,
        const unsigned char *charset
        )
#else  /* _RETURN_PTR */

unsigned char * __cdecl _mbspbrk(
        const unsigned char *string,
        const unsigned char  *charset
        )
#endif  /* _RETURN_PTR */

{
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

#ifndef _RETURN_PTR
        return __mbscspn_mt(ptmbci, string, charset);
#else  /* _RETURN_PTR */
        return __mbspbrk_mt(ptmbci, string, charset);
#endif  /* _RETURN_PTR */
}

#ifndef _RETURN_PTR

size_t __cdecl __mbscspn_mt(
        pthreadmbcinfo ptmbci,
        const unsigned char *string,
        const unsigned char *charset
        )
#else  /* _RETURN_PTR */

unsigned char * __cdecl __mbspbrk_mt(
        pthreadmbcinfo ptmbci,
        const unsigned char *string,
        const unsigned char  *charset
        )
#endif  /* _RETURN_PTR */

{
#endif  /* _MT */
        unsigned char *p, *q;
#ifdef _MT
        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
#ifndef _RETURN_PTR
            return strcspn(string, charset);
#else  /* _RETURN_PTR */
            return strpbrk(string, charset);
#endif  /* _RETURN_PTR */

        /* loop through the string to be inspected */
        for (q = (char *)string; *q ; q++) {

            /* loop through the charset */
            for (p = (char *)charset; *p ; p++) {

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

            if (*p != '\0')         /* end of charset? */
                break;              /* no, match on this char */

#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *q) )
#else  /* _MT */
            if ( _ismbblead(*q) )
#endif  /* _MT */
                if (*++q == '\0')
                    break;
        }

#ifndef _RETURN_PTR
        return((size_t) (q - string));  /* index */
#else  /* _RETURN_PTR */
        return((*q) ? q : NULL);        /* pointer */
#endif  /* _RETURN_PTR */

}

#endif  /* _MBCS */
