/***
*mbtowc.c - Convert multibyte char to wide char.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
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
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

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


#ifdef _MT
        if ( ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
#else  /* _MT */
        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
#endif  /* _MT */
        {
            if (pwc)
                *pwc = (wchar_t)(unsigned char)*s;
            return sizeof(char);
        }

#ifdef _MT
        if ( __isleadbyte_mt(ptloci, (unsigned char)*s) )
        {
            /* multi-byte char */

            if ( (ptloci->mb_cur_max <= 1) || ((int)n < ptloci->mb_cur_max) ||
                 (MultiByteToWideChar( ptloci->lc_codepage,
                                       MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                       s,
                                       ptloci->mb_cur_max,
                                       pwc,
                                       (pwc) ? 1 : 0 ) == 0) )
            {
                /* validate high byte of mbcs char */
                if ( (n < (size_t)ptloci->mb_cur_max) || (!*(s + 1)) )
                {
                    errno = EILSEQ;
                    return -1;
                }
            }
            return ptloci->mb_cur_max;
        }
#else  /* _MT */
        if ( isleadbyte((unsigned char)*s) )
        {
            /* multi-byte char */

            if ( (MB_CUR_MAX <= 1) || ((int)n < MB_CUR_MAX) ||
                 (MultiByteToWideChar( __lc_codepage,
                                      MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                      s,
                                      MB_CUR_MAX,
                                      pwc,
                                      (pwc) ? 1 : 0 ) == 0) )
            {
                /* validate high byte of mbcs char */
                if ( (n < (size_t)MB_CUR_MAX) || (!*(s + 1)) )
                {
                    errno = EILSEQ;
                    return -1;
                }
            }
            return MB_CUR_MAX;
        }
#endif  /* _MT */
        else {
            /* single byte char */

#ifdef _MT
            if ( MultiByteToWideChar( ptloci->lc_codepage,
#else  /* _MT */
            if ( MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
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
