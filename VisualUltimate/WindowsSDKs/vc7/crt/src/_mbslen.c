/***
*_mbslen.c - Return number of multibyte characters in a multibyte string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Return number of multibyte characters in a multibyte string
*       excluding the terminal null.  Locale-dependent.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <ctype.h>
#include <mtdll.h>
#include <locale.h>
#include <setlocal.h>
#include <dbgint.h>

/***
*_mbstrlen - Return number of multibyte characters in a multibyte string
*
*Purpose:
*       Return number of multibyte characters in a multibyte string
*       excluding the terminal null.  Locale-dependent.
*
*Entry:
*       char *s = string
*
*Exit:
*       Returns the number of multibyte characters in the string, or
*       (size_t)-1 if the string contains an invalid multibyte character.
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl _mbstrlen(
        const char *s
        )
{
        size_t n;
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();
#endif  /* _MT */

        _ASSERTE (MB_CUR_MAX == 1 || MB_CUR_MAX == 2);

#ifdef _MT
        if ( ptloci->mb_cur_max == 1 )
#else  /* _MT */
        if ( MB_CUR_MAX == 1 )
#endif  /* _MT */
            /* handle single byte character sets */
            return (int)strlen(s);


        /* verify all valid MB chars */
#ifdef _MT
        if ( MultiByteToWideChar( ptloci->lc_codepage,
#else  /* _MT */
        if ( MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  -1,
                                  NULL,
                                  0 ) == 0 )
        {
            /* bad MB char */
            return (size_t)-1;
        }

        /* count MB chars */
        for (n = 0; *s; n++, s++) {
#ifdef _MT
            if ( __isleadbyte_mt(ptloci, (unsigned char)*s) ) {
#else  /* _MT */
            if ( isleadbyte((unsigned char)*s) ) {
#endif  /* _MT */
                if (*++s == '\0')
                    break;
            }
        }


        return(n);
}
