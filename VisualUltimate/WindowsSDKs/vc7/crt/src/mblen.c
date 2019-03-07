/***
*mblen.c - length of multibyte character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Return the number of bytes contained in a multibyte character.
*
*******************************************************************************/


#include <internal.h>
#include <locale.h>
#include <setlocal.h>
#include <cruntime.h>
#include <stdlib.h>
#include <ctype.h>
#include <mtdll.h>
#include <dbgint.h>

/***
*int mblen() - length of multibyte character
*
*Purpose:
*       Return the number of bytes contained in a multibyte character.
*       [ANSI].
*
*Entry:
*       const char *s = pointer to multibyte character
*       size_t      n = maximum length of multibyte character to consider
*
*Exit:
*       If s = NULL, returns 0, indicating we use (only) state-independent
*       character encodings.
*
*       If s != NULL, returns:   0  (if *s = null char),
*                               -1  (if the next n or fewer bytes not valid
*                                   mbc),
*                               number of bytes contained in multibyte char
*
*Exceptions:
*
*******************************************************************************/

int __cdecl mblen
        (
        const char * s,
        size_t n
        )
{
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        _ASSERTE (ptloci->mb_cur_max == 1 || ptloci->mb_cur_max == 2);
#else  /* _MT */
        _ASSERTE (MB_CUR_MAX == 1 || MB_CUR_MAX == 2);
#endif  /* _MT */

        if ( !s || !(*s) || (n == 0) )
            /* indicate do not have state-dependent encodings,
               empty string length is 0 */
            return 0;


#ifdef _MT
        if ( __isleadbyte_mt(ptloci, (unsigned char)*s) )
#else  /* _MT */
        if ( isleadbyte((unsigned char)*s) )
#endif  /* _MT */
        {
            /* multi-byte char */

            /* verify valid MB char */
#ifdef _MT
            if ( ptloci->mb_cur_max <= 1 ||
                 (int)n < ptloci->mb_cur_max ||
                 MultiByteToWideChar( ptloci->lc_codepage,
                                      MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                      s,
                                      ptloci->mb_cur_max,
                                      NULL,
                                      0 ) == 0 )
#else  /* _MT */
            if ( MB_CUR_MAX <= 1 ||
                 (int)n < MB_CUR_MAX ||
                 MultiByteToWideChar( __lc_codepage,
                                      MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                      s,
                                      MB_CUR_MAX,
                                      NULL,
                                      0 ) == 0 )
#endif  /* _MT */
                /* bad MB char */
                return -1;
            else
#ifdef _MT
                return ptloci->mb_cur_max;
#else  /* _MT */
                return MB_CUR_MAX;
#endif  /* _MT */
        }
        else {
            /* single byte char */

            /* verify valid SB char */
#ifdef _MT
            if ( MultiByteToWideChar( __lc_codepage,
#else  /* _MT */
            if ( MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
                                      MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                      s,
                                      1,
                                      NULL,
                                      0 ) == 0 )
                return -1;
            return sizeof(char);
        }

}
