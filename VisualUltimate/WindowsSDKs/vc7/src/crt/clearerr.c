/***
*clearerr.c - clear error and eof flags
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines clearerr() - clear error and eof flags from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <mtdll.h>
#include <internal.h>
#include <msdos.h>

/***
*void clearerr(stream) - clear error and eof flags on a stream
*
*Purpose:
*       Resets the error and eof indicators for a stream to 0
*
*Entry:
*       FILE *stream - stream to set indicators on
*
*Exit:
*       No return value.
*       changes the _flag field of the FILE struct.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl clearerr (
        FILE *stream
        )
{
        _ASSERTE(stream != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        /* Clear stdio level flags */
        stream->_flag &= ~(_IOERR|_IOEOF);

        /* Clear lowio level flags */

        _osfile_safe(_fileno(stream)) &= ~(FEOFLAG);

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

}
