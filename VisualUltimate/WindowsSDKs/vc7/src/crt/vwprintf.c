/***
*vwprintf.c - wprintf from a var args pointer
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vwprintf() - print formatted data from an argument list pointer
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <wchar.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <file2.h>
#include <mtdll.h>

/***
*int vwprintf(format, ap) - print formatted data from an argument list pointer
*
*Purpose:
*       Prints formatted data items to stdout.  Uses a pointer to a
*       variable length list of arguments instead of an argument list.
*
*Entry:
*       wchar_t *format - format string, describes data format to write
*       va_list ap - pointer to variable length arg list
*
*Exit:
*       returns number of wide characters written
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vwprintf (
        const wchar_t *format,
        va_list ap
        )
/*
 * stdout 'V'ariable, 'W'char_t 'PRINT', 'F'ormatted
 */
{
        REG1 FILE *stream = stdout;
        REG2 int buffing;
        REG3 int retval;

        _ASSERTE(format != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        buffing = _stbuf(stream);
        retval = _woutput(stream, format, ap );
        _ftbuf(buffing, stream);

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

        return(retval);
}

