/***
*mbccpy.c - Copy one character  to another (MBCS)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Copy one MBCS character to another (1 or 2 bytes)
*
*******************************************************************************/

#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <stddef.h>

/***
* _mbccpy - Copy one character to another (MBCS)
*
*Purpose:
*       Copies exactly one MBCS character from src to dst.  Copies _mbclen(src)
*       bytes from src to dst.
*
*Entry:
*       unsigned char *dst = destination for copy
*       unsigned char *src = source for copy
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _mbccpy(
        unsigned char *dst,
        const unsigned char *src
        )
{
        *dst = *src;
        /*  Don't copy second byte if second byte is EOS,
            since this is not a valid MBCS pair
        */
        if ( _ismbblead(*src) && src[1]!='\0')
        {
            *++dst = *++src;
        }
}
