/***
*strnicmp.c - compare n chars of strings, ignoring case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _strnicmp() - Compares at most n characters of two strings,
*       without regard to case.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <mtdll.h>
#include <ctype.h>
#include <setlocal.h>
#include <locale.h>

/***
*int _strnicmp(first, last, count) - compares count char of strings, ignore case
*
*Purpose:
*       Compare the two strings for lexical order.  Stops the comparison
*       when the following occurs: (1) strings differ, (2) the end of the
*       strings is reached, or (3) count characters have been compared.
*       For the purposes of the comparison, upper case characters are
*       converted to lower case.
*
*Entry:
*       char *first, *last - strings to compare
*       size_t count - maximum number of characters to compare
*
*Exit:
*       returns <0 if first < last
*       returns 0 if first == last
*       returns >0 if first > last
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _strnicmp (
        const char * dst,
        const char * src,
        size_t count
        )
{
        int f,l;
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();
#endif  /* _MT */

        if ( count )
        {
#ifdef _MT
            if ( ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
#else  /* _MT */
            if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
#endif  /* _MT */
                return __ascii_strnicmp(dst, src, count);
            }
            else {
                do {
#ifdef _MT
                    f = __tolower_mt( ptloci, (unsigned char)(*(dst++)) );
                    l = __tolower_mt( ptloci, (unsigned char)(*(src++)) );
#else  /* _MT */
                    f = tolower( (unsigned char)(*(dst++)) );
                    l = tolower( (unsigned char)(*(src++)) );
#endif  /* _MT */
                } while (--count && f && (f == l) );
            }

            return( f - l );
        }

        return( 0 );
}


#ifndef _M_IX86

int __cdecl __ascii_strnicmp (
        const char * first,
        const char * last,
        size_t count
        )
{
        int f, l;

        do {

            if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                 (f <= 'Z') )
                f -= 'A' - 'a';

            if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                 (l <= 'Z') )
                l -= 'A' - 'a';

        } while ( --count && f && (f == l) );

        return ( f - l );
}

#endif  /* _M_IX86 */
