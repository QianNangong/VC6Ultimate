/***
*mblen.c - length of multibyte character
*
*       Copyright (c) 1990-2001, Microsoft Corporation. All rights reserved.
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
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

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
        if ( __MB_CUR_MAX(ptloci) <= 1 ||
             (int)n < __MB_CUR_MAX(ptloci) ||
             MultiByteToWideChar( __LC_CODEPAGE(ptloci),
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  __MB_CUR_MAX(ptloci),
                                  NULL,
                                  0 ) == 0 )
            /* bad MB char */
            return -1;
        else
            return __MB_CUR_MAX(ptloci);
    }
    else {
        /* single byte char */

        /* verify valid SB char */
        if ( MultiByteToWideChar( __LC_CODEPAGE(ptloci),
                                  MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                                  s,
                                  1,
                                  NULL,
                                  0 ) == 0 )
            return -1;
        return sizeof(char);
    }

}
