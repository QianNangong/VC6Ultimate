/***
*wcslwr.c - routine to map upper-case characters in a wchar_t string
*       to lower-case
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts all the upper case characters in a wchar_t string
*       to lower case, in place.
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <ctype.h>
#include <setlocal.h>
#include <mtdll.h>
#include <awint.h>
#include <dbgint.h>

/***
*wchar_t *_wcslwr(string) - map upper-case characters in a string to lower-case
*
*Purpose:
*       wcslwr converts upper-case characters in a null-terminated wchar_t
*       string to their lower-case equivalents.  The result may be longer or
*       shorter than the original string.  Assumes enough space in string
*       to hold the result.
*
*Entry:
*       wchar_t *wsrc - wchar_t string to change to lower case
*
*Exit:
*       input string address
*
*Exceptions:
*       on an error, the original string is unaltered
*
*******************************************************************************/

wchar_t * __cdecl _wcslwr (
        wchar_t * wsrc
        )
{

    wchar_t *p;             /* traverses string for C locale conversion */
    wchar_t *wdst;          /* wide version of string in alternate case */
    int dstlen;             /* len of wdst string, wide chars, with null */
    int malloc_flag = 0;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */
    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE )
    {
        for ( p = wsrc ; *p ; p++ )
            if ( (*p >= (wchar_t)L'A') && (*p <= (wchar_t)L'Z') )
                *p -= L'A' - L'a';

        return(wsrc);
    }   /* C locale */

    /* Inquire size of wdst string */
    if ( (dstlen = __crtLCMapStringW( __LC_HANDLE(ptloci)[LC_CTYPE],
                                      LCMAP_LOWERCASE,
                                      wsrc,
                                      -1,
                                      NULL,
                                      0,
                                      __LC_CODEPAGE(ptloci)
                                      )) == 0 )
        return(wsrc);

    /* Allocate space for wdst */
    __try {
        wdst = (wchar_t *)_alloca(dstlen * sizeof(wchar_t));
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
        _resetstkoflw();
        wdst = NULL;
    }

    if ( wdst == NULL ) {
        wdst = (wchar_t *)_malloc_crt(dstlen * sizeof(wchar_t));
        malloc_flag++;
    }

    /* Map wrc string to wide-character wdst string in alternate case */
    if ( (wdst != NULL) &&
         (__crtLCMapStringW(__LC_HANDLE(ptloci)[LC_CTYPE],
                            LCMAP_LOWERCASE,
                            wsrc,
                            -1,
                            wdst,
                            dstlen,
                            __LC_CODEPAGE(ptloci)
                            ) != 0) )
        /* Copy wdst string to user string */
        wcscpy(wsrc, wdst);

    if ( malloc_flag )
        _free_crt(wdst);
    return(wsrc);
}

