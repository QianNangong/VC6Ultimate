/***
*_ctype.c - function versions of ctype macros
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This files provides function versions of the character
*       classification and conversion macros in ctype.h.
*
*******************************************************************************/

/***
*ctype - Function versions of ctype macros
*
*Purpose:
*       Function versions of the macros in ctype.h.  In order to define
*       these, we use a trick -- we undefine the macro so we can use the
*       name in the function declaration, then re-include the file so
*       we can use the macro in the definition part.
*
*       Functions defined:
*           isalpha     isupper     islower
*           isdigit     isxdigit    isspace
*           ispunct     isalnum     isprint
*           isgraph     isctrl      __isascii
*           __toascii   __iscsym    __iscsymf
*
*Entry:
*       int c = character to be tested
*Exit:
*       returns non-zero = character is of the requested type
*                  0 = character is NOT of the requested type
*
*Exceptions:
*       None.
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <mtdll.h>
#include <setlocal.h>

int (__cdecl isalpha) (
        int c
        )
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __isalpha_mt(ptloci, c);
#else  /* _MT */
    return isalpha(c);
#endif  /* _MT */
}

int (__cdecl isupper) (
        int c
        )
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __isupper_mt(ptloci, c);
#else  /* _MT */
    return isupper(c);
#endif  /* _MT */
}

int (__cdecl islower) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __islower_mt(ptloci, c);
#else  /* _MT */
    return islower(c);
#endif  /* _MT */
}

int (__cdecl isdigit) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isdigit_mt(ptloci, c);
#else  /* _MT */
    return isdigit(c);
#endif  /* _MT */
}

int (__cdecl isxdigit) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isxdigit_mt(ptloci, c);
#else  /* _MT */
    return isxdigit(c);
#endif  /* _MT */
}

int (__cdecl isspace) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isspace_mt(ptloci, c);
#else  /* _MT */
    return isspace(c);
#endif  /* _MT */
}

int (__cdecl ispunct) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __ispunct_mt(ptloci, c);
#else  /* _MT */
    return ispunct(c);
#endif  /* _MT */
}

int (__cdecl isalnum) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isalnum_mt(ptloci, c);
#else  /* _MT */
    return isalnum(c);
#endif  /* _MT */
}

int (__cdecl isprint) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isprint_mt(ptloci, c);
#else  /* _MT */
    return isprint(c);
#endif  /* _MT */
}

int (__cdecl isgraph) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __isgraph_mt(ptloci, c);
#else  /* _MT */
    return isgraph(c);
#endif  /* _MT */
}

int (__cdecl iscntrl) (
        int c
        )
{
#ifdef _MT
    pthreadlocinfo ptloci = _getptd()->ptlocinfo;

    if ( ptloci != __ptlocinfo )
        ptloci = __updatetlocinfo();

    return __iscntrl_mt(ptloci, c);
#else  /* _MT */
    return iscntrl(c);
#endif  /* _MT */
}

int (__cdecl __isascii) (
        int c
        )
{
    return __isascii(c);
}

int (__cdecl __toascii) (
        int c
        )
{
    return __toascii(c);
}

int (__cdecl __iscsymf) (
        int c
        )
{
    return __iscsymf(c);
}

int (__cdecl __iscsym) (
        int c
        )
{
    return __iscsym(c);
}
