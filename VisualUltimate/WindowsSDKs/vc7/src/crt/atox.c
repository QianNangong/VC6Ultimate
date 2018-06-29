/***
*atox.c - atoi and atol conversion
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts a character string into an int or long.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <ctype.h>
#include <mtdll.h>
#include <tchar.h>
#include <setlocal.h>
#ifdef _MBCS
#undef _MBCS
#endif  /* _MBCS */

#ifndef _UNICODE
#define _tchartodigit(c)    ((c) >= '0' && (c) <= '9' ? (c) - '0' : -1)
#else  /* _UNICODE */
int _wchartodigit(wchar_t);
#define _tchartodigit(c)    _wchartodigit((wchar_t)(c))
#endif  /* _UNICODE */

/***
*long atol(char *nptr) - Convert string to long
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       Overflow is not detected.
*
*Entry:
*       nptr = ptr to string to convert
*
*Exit:
*       return long int value of the string
*
*Exceptions:
*       None - overflow is not detected.
*
*******************************************************************************/

long __cdecl _tstol(
        const _TCHAR *nptr
        )
{
        int c;              /* current char */
        long total;         /* current total */
        int sign;           /* if '-', then negative, otherwise positive */
#if defined (_MT) && !defined (_UNICODE)
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);


    /* skip whitespace */
    while ( __isspace_mt(ptloci, (int)(_TUCHAR)*nptr) )
#else  /* defined (_MT) && !defined (_UNICODE) */
    while ( _istspace((int)(_TUCHAR)*nptr) )
#endif  /* defined (_MT) && !defined (_UNICODE) */
        ++nptr;

    c = (int)(_TUCHAR)*nptr++;
    sign = c;           /* save sign indication */
    if (c == _T('-') || c == _T('+'))
        c = (int)(_TUCHAR)*nptr++;    /* skip sign */

    total = 0;

    while ( (c = _tchartodigit(c)) != -1 ) {
        total = 10 * total + c;     /* accumulate digit */
        c = (_TUCHAR)*nptr++;    /* get next char */
    }

    if (sign == '-')
        return -total;
    else
        return total;   /* return result, negated if necessary */
}


/***
*int atoi(char *nptr) - Convert string to long
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       Overflow is not detected.  Because of this, we can just use
*       atol().
*
*Entry:
*       nptr = ptr to string to convert
*
*Exit:
*       return int value of the string
*
*Exceptions:
*       None - overflow is not detected.
*
*******************************************************************************/

int __cdecl _tstoi(
        const _TCHAR *nptr
        )
{
    return (int)_tstol(nptr);
}

#ifndef _NO_INT64

__int64 __cdecl _tstoi64(
        const _TCHAR *nptr
        )
{
    int c;              /* current char */
    __int64 total;      /* current total */
    int sign;           /* if '-', then negative, otherwise positive */
#if defined (_MT) && !defined (_UNICODE)
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    /* skip whitespace */
    while ( __isspace_mt(ptloci, (int)(_TUCHAR)*nptr) )
#else  /* defined (_MT) && !defined (_UNICODE) */
    while ( _istspace((int)(_TUCHAR)*nptr) )
#endif  /* defined (_MT) && !defined (_UNICODE) */
        ++nptr;

    c = (int)(_TUCHAR)*nptr++;
    sign = c;           /* save sign indication */
    if (c == _T('-') || c == _T('+'))
        c = (int)(_TUCHAR)*nptr++;    /* skip sign */

    total = 0;

    while ( (c = _tchartodigit(c)) != -1 ) {
        total = 10 * total + c;     /* accumulate digit */
        c = (_TUCHAR)*nptr++;    /* get next char */
    }

    if (sign == _T('-'))
        return -total;
    else
        return total;   /* return result, negated if necessary */
}

#endif  /* _NO_INT64 */
