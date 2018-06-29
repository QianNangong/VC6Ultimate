/***
*towlower.c - convert wide character to lower case
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines towlower().
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <stdio.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <awint.h>

/***
*wchar_t towlower(c) - convert wide character to lower case
*
*Purpose:
*       towlower() returns the lowercase equivalent of its argument
*
*Entry:
*       c - wchar_t value of character to be converted
*
*Exit:
*       if c is an upper case letter, returns wchar_t value of lower case
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/

wchar_t __cdecl towlower (
        wchar_t c
        )
{

#ifdef _MT

    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    if ( c == WEOF )
        return c;

    if ( ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
        return __ascii_towlower(c);

    return __towlower_mt(ptloci, c);
}

/***
*wchar_t __towlower_mt(ptloci, c) - convert wide character to lower case
*
*Purpose:
*       Multi-thread function only! Non-locking version of towlower.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

wchar_t __cdecl __towlower_mt (
        pthreadlocinfo ptloci,
        wchar_t c
        )
{

#endif  /* _MT */

        wchar_t widechar;

        if (c == WEOF)
            return c;

#ifndef _MT
        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
            return __ascii_towlower(c);
#endif  /* _MT */

        /* if checking case of c does not require API call, do it */
        if ( c < 256 ) {
#ifdef _MT
            if ( !__iswupper_mt(ptloci, c) ) {
#else  /* _MT */
            if ( !iswupper(c) ) {
#endif  /* _MT */
                return c;
            }
        }

        /* convert wide char to lowercase */
#ifdef _MT
        if ( 0 == __crtLCMapStringW( ptloci->lc_handle[LC_CTYPE],
#else  /* _MT */
        if ( 0 == __crtLCMapStringW( __lc_handle[LC_CTYPE],
#endif  /* _MT */
                                     LCMAP_LOWERCASE,
                                     (LPCWSTR)&c,
                                     1,
                                     (LPWSTR)&widechar,
                                     1,
#ifdef _MT
                                     ptloci->lc_codepage ) )
#else  /* _MT */
                                     __lc_codepage ) )
#endif  /* _MT */
        {
            return c;
        }

        return widechar;

}
