/***
*wcsicmp.c - contains case-insensitive wide string comp routine _wcsicmp
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _wcsicmp()
*
*******************************************************************************/


#include <cruntime.h>
#include <setlocal.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <setlocal.h>
#include <mtdll.h>

/***
*int _wcsicmp(dst, src) - compare wide-character strings, ignore case
*
*Purpose:
*       _wcsicmp perform a case-insensitive wchar_t string comparision.
*       _wcsicmp is independent of locale.
*
*Entry:
*       wchar_t *dst, *src - strings to compare
*
*Return:
*       <0 if dst < src
*        0 if dst = src
*       >0 if dst > src
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _wcsicmp (
        const wchar_t * dst,
        const wchar_t * src
        )
{
    wchar_t f,l;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE) {
        do  {
            f = __ascii_towlower(*dst);
            l = __ascii_towlower(*src);
            dst++;
            src++;
        } while ( (f) && (f == l) );
    }
    else {
        do  {
#ifdef _MT
            f = __towlower_mt( ptloci, (unsigned short)(*(dst++)) );
            l = __towlower_mt( ptloci, (unsigned short)(*(src++)) );
#else  /* _MT */
            f = towlower( (unsigned short)(*(dst++)) );
            l = towlower( (unsigned short)(*(src++)) );
#endif  /* _MT */
        } while ( (f) && (f == l) );
    }

    return (int)(f - l);
}

