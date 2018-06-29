/***
*_mbslen.c - Return number of multibyte characters in a multibyte string
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
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
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    _ASSERTE (__MB_CUR_MAX(ptloci) == 1 || __MB_CUR_MAX(ptloci) == 2);

    if ( __MB_CUR_MAX(ptloci) == 1 )
        /* handle single byte character sets */
        return (int)strlen(s);


    /* verify all valid MB chars */
    if ( MultiByteToWideChar( __LC_CODEPAGE(ptloci),
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
        if ( __isleadbyte_mt(ptloci, (unsigned char)*s) )
#else  /* _MT */
        if ( isleadbyte((unsigned char)*s) )
#endif  /* _MT */
        {
            if (*++s == '\0')
                break;
        }
    }


    return(n);
}
