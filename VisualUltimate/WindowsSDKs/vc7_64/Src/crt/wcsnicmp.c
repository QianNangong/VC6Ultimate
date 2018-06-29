/***
*wcsnicmp.c - compare n chars of wide-character strings, ignoring case
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wcsnicmp() - Compares at most n characters of two wchar_t
*       strings, without regard to case.
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>

/***
*int _wcsnicmp(first, last, count) - compares count wchar_t of strings,
*       ignore case
*
*Purpose:
*       Compare the two strings for lexical order.  Stops the comparison
*       when the following occurs: (1) strings differ, (2) the end of the
*       strings is reached, or (3) count characters have been compared.
*       For the purposes of the comparison, upper case characters are
*       converted to lower case (wide-characters).
*
*Entry:
*       wchar_t *first, *last - strings to compare
*       size_t count - maximum number of characters to compare
*
*Exit:
*       -1 if first < last
*        0 if first == last
*        1 if first > last
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _wcsnicmp (
        const wchar_t * first,
        const wchar_t * last,
        size_t count
        )
{
    wchar_t f,l;
    int result = 0;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    if ( count ) {
        if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE ) {
            do {
                f = __ascii_towlower(*first);
                l = __ascii_towlower(*last);
                first++;
                last++;
            } while ( (--count) && f && (f == l) );
        }
        else {
            do {
#ifdef _MT
                f = __towlower_mt( ptloci, (unsigned short)(*(first++)) );
                l = __towlower_mt( ptloci, (unsigned short)(*(last++)) );
#else  /* _MT */
                f = towlower( (unsigned short)(*(first++)) );
                l = towlower( (unsigned short)(*(last++)) );
#endif  /* _MT */
            } while ( (--count) && f && (f == l) );
        }

        result = (int)(f - l);
    }
    return result;
}

