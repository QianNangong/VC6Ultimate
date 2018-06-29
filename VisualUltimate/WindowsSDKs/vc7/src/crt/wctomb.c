/***
*wctomb.c - Convert wide character to multibyte character.
*
*       Copyright (c) 1990-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a wide character into the equivalent multibyte character.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <mtdll.h>
#include <errno.h>
#include <locale.h>
#include <setlocal.h>

/***
*int wctomb() - Convert wide character to multibyte character.
*
*Purpose:
*       Convert a wide character into the equivalent multi-byte character,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       char *s          = pointer to multibyte character
*       wchar_t wchar        = source wide character
*
*Exit:
*       If s = NULL, returns 0, indicating we only use state-independent
*       character encodings.
*       If s != NULL, returns:
*                   -1 (if error) or number of bytes comprising
*                   converted mbc
*
*Exceptions:
*
*******************************************************************************/

int __cdecl wctomb (
        char *s,
        wchar_t wchar
        )
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __wctomb_mt(ptloci, s, wchar);
}

int __cdecl __wctomb_mt (
        pthreadlocinfo ptloci,
        char *s,
        wchar_t wchar
        )
{
#endif  /* _MT */
    if ( !s )
        /* indicate do not have state-dependent encodings */
        return 0;


    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE )
    {
        if ( wchar > 255 )  /* validate high byte */
        {
            errno = EILSEQ;
            return -1;
        }

        *s = (char) wchar;
        return sizeof(char);
    }
    else
    {
        int size;
        BOOL defused = 0;

        if ( ((size = WideCharToMultiByte( __LC_CODEPAGE(ptloci),
                                           0,
                                           &wchar,
                                           1,
                                           s,
#ifdef _MT
                                           ptloci->mb_cur_max,
#else  /* _MT */
                                           MB_CUR_MAX,
#endif  /* _MT */
                                           NULL,
                                           &defused) ) == 0) ||
             (defused) )
        {
            errno = EILSEQ;
            return -1;
        }

        return size;
    }

}
