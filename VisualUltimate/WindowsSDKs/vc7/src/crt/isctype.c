/***
*isctype.c - support is* ctype functions/macros for two-byte multibyte chars
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _isctype.c - support is* ctype functions/macros for
*       two-byte multibyte chars.
*
*******************************************************************************/

#include <stdio.h>
#include <cruntime.h>
#include <ctype.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <awint.h>
#include <dbgint.h>


/*
 *  Use GetCharType() API so check that character type masks agree between
 *  ctype.h and winnls.h
 */
#if _UPPER   != C1_UPPER || \
        _LOWER   != C1_LOWER || \
        _DIGIT   != C1_DIGIT || \
        _SPACE   != C1_SPACE || \
        _PUNCT   != C1_PUNCT || \
        _CONTROL != C1_CNTRL
#error Character type masks do not agree in ctype and winnls
#endif  /* _UPPER   != C1_UPPER || \ */



/***
* __chvalidator
*
* Purpose:
*       This function is called by character testing functions in debug
*       versions. This function test for validation of c as character.
*       For improvement in performance, it is not used in non-debug
*       version.  It is available in the static single-thread non-debug
*       build, though, just in case C code that includes ctype.h is compiled
*       /D_DEBUG /ML.
*
*******************************************************************************/

#if !defined (_MT) || defined (_DEBUG)
int __cdecl _chvalidator(
        int c,
        int mask
        )
{
        _ASSERTE((unsigned)(c + 1) <= 256);
        return ( _pctype[c] & mask);
}
#endif  /* !defined (_MT) || defined (_DEBUG) */

#if defined (_MT) && defined (_DEBUG)
int __cdecl _chvalidator_mt(
        pthreadlocinfo p,
        int c,
        int mask
        )
{
        _ASSERTE((unsigned)(c + 1) <= 256);
        return (p->pctype[c] & mask);
}

#endif  /* defined (_MT) && defined (_DEBUG) */

/***
*_isctype - support is* ctype functions/macros for two-byte multibyte chars
*
*Purpose:
*       This function is called by the is* ctype functions/macros
*       (e.g. isalpha()) when their argument is a two-byte multibyte char.
*       Returns true or false depending on whether the argument satisfies
*       the character class property encoded by the mask.
*
*Entry:
*       int c - the multibyte character whose type is to be tested
*       unsigned int mask - the mask used by the is* functions/macros
*                  corresponding to each character class property
*
*       The leadbyte and the trailbyte should be packed into the int c as:
*
*       H.......|.......|.......|.......L
*           0       0   leadbyte trailbyte
*
*Exit:
*       Returns non-zero if c is of the character class.
*       Returns 0 if c is not of the character class.
*
*Exceptions:
*       Returns 0 on any error.
*
*******************************************************************************/

int __cdecl _isctype (
        int c,
        int mask
        )
{
#ifdef _MT

    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __isctype_mt(ptloci, c, mask);
}

int __cdecl __isctype_mt (
        pthreadlocinfo ptloci,
        int c,
        int mask
        )
{
#endif  /* _MT */
        int size;
        unsigned short chartype;
        char buffer[3];

        /* c valid between -1 and 255 */
        if ( ((unsigned)(c + 1)) <= 256 )
#ifdef _MT
            return ptloci->pctype[c] & mask;
#else  /* _MT */
            return _pctype[c] & mask;
#endif  /* _MT */

#ifdef _MT
        if ( __isleadbyte_mt(ptloci, c >> 8 & 0xff) )
#else  /* _MT */
        if ( isleadbyte(c >> 8 & 0xff) )
#endif  /* _MT */
        {
            buffer[0] = (c >> 8 & 0xff); /* put lead-byte at start of str */
            buffer[1] = (char)c;
            buffer[2] = 0;
            size = 2;
        } else {
            buffer[0] = (char)c;
            buffer[1] = 0;
            size = 1;
        }

        if ( 0 == __crtGetStringTypeA( CT_CTYPE1,
                                       buffer,
                                       size,
                                       &chartype,
                                       __LC_CODEPAGE(ptloci),
                                       __LC_HANDLE(ptloci)[LC_CTYPE],
                                       TRUE) )
        {
            return 0;
        }

        return (int)(chartype & mask);
}

