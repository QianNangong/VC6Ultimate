/***
*memicmp.c - compare memory, ignore case
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _memicmp() - compare two blocks of memory for lexical
*       order.  Case is ignored in the comparison.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <mtdll.h>
#include <ctype.h>
#include <setlocal.h>
#include <locale.h>

/***
*int _memicmp(first, last, count) - compare two blocks of memory, ignore case
*
*Purpose:
*       Compares count bytes of the two blocks of memory stored at first
*       and last.  The characters are converted to lowercase before
*       comparing (not permanently), so case is ignored in the search.
*
*Entry:
*       char *first, *last - memory buffers to compare
*       size_t count - maximum length to compare
*
*Exit:
*       returns < 0 if first < last
*       returns 0 if first == last
*       returns > 0 if first > last
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _memicmp (
        const void * first,
        const void * last,
        size_t count
        )
{
    int f = 0, l = 0;
    const char *dst = first, *src = last;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE )
    {
        return __ascii_memicmp(first, last, count);
    }
    else {
        while (count-- && f==l)
        {
#ifdef _MT
            f = __tolower_mt( ptloci, (unsigned char)(*(dst++)) );
            l = __tolower_mt( ptloci, (unsigned char)(*(src++)) );
#else  /* _MT */
            f = tolower( (unsigned char)(*(dst++)) );
            l = tolower( (unsigned char)(*(src++)) );
#endif  /* _MT */
        }
    }

    return ( f - l );
}


#ifndef _M_IX86

int __cdecl __ascii_memicmp (
        const void * first,
        const void * last,
        size_t count
        )
{
    int f = 0;
    int l = 0;
    while ( count-- )
    {
        if ( (*(unsigned char *)first == *(unsigned char *)last) ||
             ((f = __ascii_tolower( *(unsigned char *)first )) ==
              (l = __ascii_tolower( *(unsigned char *)last ))) )
        {
                first = (char *)first + 1;
                last = (char *)last + 1;
        }
        else
            break;
    }
    return ( f - l );
}

#endif  /* _M_IX86 */
