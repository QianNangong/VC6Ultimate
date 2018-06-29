/***
*stricmp.c - contains case-insensitive string comp routine _stricmp/_strcmpi
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _stricmp(), also known as _strcmpi()
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <mtdll.h>
#include <setlocal.h>
#include <ctype.h>
#include <locale.h>



/***
*int _strcmpi(dst, src), _strcmpi(dst, src) - compare strings, ignore case
*
*Purpose:
*       _stricmp/_strcmpi perform a case-insensitive string comparision.
*       For differences, upper case letters are mapped to lower case.
*       Thus, "abc_" < "ABCD" since "_" < "d".
*
*Entry:
*       char *dst, *src - strings to compare
*
*Return:
*       <0 if dst < src
*        0 if dst = src
*       >0 if dst > src
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _stricmp (
        const char * dst,
        const char * src
        )
{

    int f,l;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE ) {
        return __ascii_stricmp(dst, src);
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
        } while ( f && (f == l) );
    }

    return(f - l);
}

#ifndef _M_IX86

int __cdecl __ascii_stricmp (
        const char * dst,
        const char * src
        )
{
    int f, l;

    do {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') &&
             (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') &&
             (l <= 'Z') )
            l -= 'A' - 'a';
    } while ( f && (f == l) );

    return(f - l);
}

#endif  /* _M_IX86 */
