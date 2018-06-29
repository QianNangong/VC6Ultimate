/***
*fgetc.c - get a character from a stream
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fgetc() and getc() - read  a character from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int fgetc(stream), getc(stream) - read a character from a stream
*
*Purpose:
*       reads a character from the given stream
*
*Entry:
*       FILE *stream - stream to read character from
*
*Exit:
*       returns the character read
*       returns EOF if at end of file or error occurred
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fgetc (
        REG1 FILE *stream
        )
{
        int retval;

        _ASSERTE(stream != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        retval = _getc_lk(stream);

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

        return(retval);
}

#undef getc

int __cdecl getc (
        FILE *stream
        )
{
        int retval;

        _ASSERTE(stream != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        retval = _getc_lk(stream);

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

        return(retval);
}
