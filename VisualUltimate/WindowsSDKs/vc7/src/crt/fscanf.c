/***
*fscanf.c - read formatted data from stream
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fscanf() - reads formatted data from stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int fscanf(stream, format, ...) - read formatted data from stream
*
*Purpose:
*       Reads formatted data from stream into arguments.  _input does the real
*       work here.
*
*Entry:
*       FILE *stream - stream to read data from
*       char *format - format string
*       followed by list of pointers to storage for the data read.  The number
*       and type are controlled by the format string.
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fscanf (
        FILE *stream,
        const char *format,
        ...
        )
/*
 * 'F'ile (stream) 'SCAN', 'F'ormatted
 */
{
        int retval;

        va_list arglist;

        va_start(arglist, format);

        _ASSERTE(stream != NULL);
        _ASSERTE(format != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        retval = (_input(stream,format,arglist));

#ifdef _MT
        }
        __finally {
                _unlock_str(stream);
        }
#endif  /* _MT */

        return(retval);
}
