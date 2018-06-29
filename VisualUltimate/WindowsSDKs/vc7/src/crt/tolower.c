/***
*tolower.c - convert character to lower case
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines function versions of _tolower() and tolower().
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <stddef.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <awint.h>

/* remove macro defintions of _tolower() and tolower()
 */
#undef  _tolower
#undef  tolower

/* define function-like macro equivalent to _tolower()
 */
#define mklower(c)  ( (c)-'A'+'a' )

/***
*int _tolower(c) - convert character to lower case
*
*Purpose:
*       _tolower() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       returns int value of lower case representation of c
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tolower (
        int c
        )
{
        return(mklower(c));
}

/***
*int tolower(c) - convert character to lower case
*
*Purpose:
*       tolower() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       if c is an upper case letter, returns int value of lower case
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/


int __cdecl tolower (
        int c
        )
{

#ifdef _MT

    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return __tolower_mt(ptloci, c);
}

/***
*int __tolower_mt(c) - convert character to lower case
*
*Purpose:
*       Multi-thread function only!
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __tolower_mt (
        pthreadlocinfo ptloci,
        int c
        )
{

#endif  /* _MT */

    int size;
    unsigned char inbuffer[3];
    unsigned char outbuffer[3];

    if ( __LC_HANDLE(ptloci)[LC_CTYPE] == _CLOCALEHANDLE ||
         (__LC_CLIKE(ptloci) && (unsigned) c <= 0x7f))
        return __ascii_tolower(c);

    /* if checking case of c does not require API call, do it */
    if ( (unsigned)c < 256 )
    {
#ifdef _MT
        if ( !__isupper_mt(ptloci, c) )
#else  /* _MT */
        if ( !isupper(c) )
#endif  /* _MT */
        {
            return c;
        }
    }

    /* convert int c to multibyte string */
#ifdef _MT
    if ( __isleadbyte_mt(ptloci, c >> 8 & 0xff) )
#else  /* _MT */
    if ( isleadbyte(c >> 8 & 0xff) )
#endif  /* _MT */
    {
        inbuffer[0] = (c >> 8 & 0xff); /* put lead-byte at start of str */
        inbuffer[1] = (unsigned char)c;
        inbuffer[2] = 0;
        size = 2;
    } else {
        inbuffer[0] = (unsigned char)c;
        inbuffer[1] = 0;
        size = 1;
    }

    /* convert to lowercase */
    if ( 0 == (size = __crtLCMapStringA( __LC_HANDLE(ptloci)[LC_CTYPE],
                                         LCMAP_LOWERCASE,
                                         inbuffer,
                                         size,
                                         outbuffer,
                                         3,
                                         __LC_CODEPAGE(ptloci),
                                         TRUE)) )
    {
        return c;
    }

    /* construct integer return value */
    if (size == 1)
        return ((int)outbuffer[0]);
    else
        return ((int)outbuffer[1] | ((int)outbuffer[0] << 8));

}
