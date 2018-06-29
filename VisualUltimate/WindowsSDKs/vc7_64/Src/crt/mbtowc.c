/***
*mbtowc.c - Convert multibyte char to wide char.
*
*       Copyright (c) 1990-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a multibyte character into the equivalent wide character.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <mtdll.h>
#include <errno.h>
#include <dbgint.h>
#include <ctype.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>

/***
*int mbtowc() - Convert multibyte char to wide character.
*
*Purpose:
*       Convert a multi-byte character into the equivalent wide character,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       wchar_t  *pwc = pointer to destination wide character
*       const char *s = pointer to multibyte character
*       size_t      n = maximum length of multibyte character to consider
*
*Exit:
*       If s = NULL, returns 0, indicating we only use state-independent
*       character encodings.
*       If s != NULL, returns:  0 (if *s = null char)
*                               -1 (if the next n or fewer bytes not valid mbc)
*                               number of bytes comprising converted mbc
*
*Exceptions:
*
*******************************************************************************/

int __cdecl mbtowc(
        wchar_t  *pwc,
        const char *s,
        size_t n
        )
{
#ifdef _MT

    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __mbtowc_mt(ptloci, pwc, s, n);
}

int __cdecl __mbtowc_mt (
        pthreadlocinfo ptloci,
        wchar_t  *pwc,
        const char *s,
        size_t n
        )
{
    _ASSERTE (ptloci->mb_cur_max == 1 || ptloci->mb_cur_max == 2);
#else  /* _MT */
    _ASSERTE (MB_CUR_MAX == 1 || MB_CUR_MAX == 2);
#endif  /* _MT */
    if ( !s || n == 0 )
        /* indicate do not have state-dependent encodings,
           handle zero length string */
        return 0;

    if ( !*s )
    {
        /* handle NULL char */
        if (pwc)
            *pwc = 0;
        return 0;
    }


    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE )
    {
        if (pwc)
            *pwc = (wchar_t)(unsigned char)*s;
        return sizeof(char);
    }

#ifdef _MT
    if ( __isleadbyte_mt(ptloci, (unsigned char)*s) )
#else  /* _MT */
    if ( isleadbyte((unsigned char)*s) )
#endif  /* _MT */
    {
        /* multi-byte char */

        if ( (__MB_CUR_MAX(ptloci) <= 1) || ((int)n < __MB_CUR_MAX(ptloci)) ||
             (MultiByteToWideChar( __LC_CODEPAGE(ptloci),
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  __MB_CUR_MAX(ptloci),
                                  pwc,
                                  (pwc) ? 1 : 0 ) == 0) )
        {
            /* validate high byte of mbcs char */
            if ( (n < (size_t)__MB_CUR_MAX(ptloci)) || (!*(s + 1)) )
            {
                errno = EILSEQ;
                return -1;
            }
        }
        return __MB_CUR_MAX(ptloci);
    }
    else {
        /* single byte char */
         if ( MultiByteToWideChar( __LC_CODEPAGE(ptloci),
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  1,
                                  pwc,
                                  (pwc) ? 1 : 0 ) == 0 )
        {
            errno = EILSEQ;
            return -1;
        }
        return sizeof(char);
    }

}
