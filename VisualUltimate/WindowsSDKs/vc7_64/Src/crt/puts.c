/***
*puts.c - put a string to stdout
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines puts() and _putws() - put a string to stdout
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <string.h>
#include <internal.h>
#include <mtdll.h>
#include <tchar.h>

/***
*int puts(string) - put a string to stdout with newline
*
*Purpose:
*       Write a string to stdout; don't include '\0' but append '\n'.  Uses
*       temporary buffering for efficiency on stdout if unbuffered.
*
*Entry:
*       char *string - string to output
*
*Exit:
*       Good return = 0
*       Error return = EOF
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _putts (
        const _TCHAR *string
        )
{
        int buffing;
#ifndef _UNICODE
        size_t length;
        size_t ndone;
#endif  /* _UNICODE */
        int retval = _TEOF; /* error */

        _ASSERTE(string != NULL);

#ifdef _MT
        _lock_str2(1, stdout);
        __try {
#endif  /* _MT */

        buffing = _stbuf(stdout);

#ifdef _UNICODE
        while (*string) {
            if (_putwchar_lk(*string++) == WEOF)
                goto done;
        }
        if (_putwchar_lk(L'\n') != WEOF)
            retval = 0;     /* success */
#else  /* _UNICODE */
        length = strlen(string);
        ndone = _fwrite_lk(string,1,length,stdout);

        if (ndone == length) {
            _putc_lk('\n',stdout);
            retval = 0;     /* success */
        }
#endif  /* _UNICODE */

#ifdef _UNICODE
done:
#endif  /* _UNICODE */
        _ftbuf(buffing, stdout);

#ifdef _MT
        }
        __finally {
            _unlock_str2(1, stdout);
        }
#endif  /* _MT */

        return retval;
}
