/***
*vsprintf.c - print formatted data into a string from var arg list
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vsprintf() and _vsnprintf() - print formatted output to
*       a string, get the data from an argument ptr instead of explicit
*       arguments.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <limits.h>
#include <mtdll.h>

#define MAXSTR INT_MAX


/***
*ifndef _COUNT_
*int vsprintf(string, format, ap) - print formatted data to string from arg ptr
*else
*int _vsnprintf(string, cnt, format, ap) - print formatted data to string from arg ptr
*endif
*
*Purpose:
*       Prints formatted data, but to a string and gets data from an argument
*       pointer.
*       Sets up a FILE so file i/o operations can be used, make string look
*       like a huge buffer to it, but _flsbuf will refuse to flush it if it
*       fills up. Appends '\0' to make it a true string.
*
*       Allocate the 'fake' _iob[] entryit statically instead of on
*       the stack so that other routines can assume that _iob[] entries are in
*       are in DGROUP and, thus, are near.
*
*ifdef _COUNT_
*       The _vsnprintf() flavor takes a count argument that is
*       the max number of bytes that should be written to the
*       user's buffer.
*endif
*
*       Multi-thread: (1) Since there is no stream, this routine must never try
*       to get the stream lock (i.e., there is no stream lock either).  (2)
*       Also, since there is only one staticly allocated 'fake' iob, we must
*       lock/unlock to prevent collisions.
*
*Entry:
*       char *string - place to put destination string
*ifdef _COUNT_
*       size_t count - max number of bytes to put in buffer
*endif
*       char *format - format string, describes format of data
*       va_list ap - varargs argument pointer
*
*Exit:
*       returns number of characters in string
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_

int __cdecl vsprintf (
        char *string,
        const char *format,
        va_list ap
        )
#else  /* _COUNT_ */

int __cdecl _vsnprintf (
        char *string,
        size_t count,
        const char *format,
        va_list ap
        )
#endif  /* _COUNT_ */

{
        FILE str;
        REG1 FILE *outfile = &str;
        REG2 int retval;

        _ASSERTE(format != NULL);

#ifndef _COUNT_
        _ASSERTE(string != NULL);
        outfile->_cnt = MAXSTR;
#else  /* _COUNT_ */
        outfile->_cnt = (int)count;
#endif  /* _COUNT_ */
        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = string;

        retval = _output(outfile,format,ap );
        if ( string!=NULL)
            _putc_lk('\0',outfile);

        return(retval);
}

/***
* _vscprintf() - counts the number of character needed to print the formatted
* data
*
*Purpose:
*       Counts the number of characters in the fotmatted data.
*
*Entry:
*       char *format - format string, describes format of data
*       va_list ap - varargs argument pointer
*
*Exit:
*       returns number of characters needed to print formatted data.
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_
int __cdecl _vscprintf (
        const char *format,
        va_list ap
        )
{
        FILE str;
        REG1 FILE *outfile = &str;
        REG2 int retval;

        _ASSERTE(format != NULL);

        outfile->_cnt = MAXSTR;
        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = NULL;

        retval = _output(outfile,format,ap);
        return(retval);
}
#endif  /* _COUNT_ */
