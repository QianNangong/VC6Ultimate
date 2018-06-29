/***
*commit.c - flush buffer to disk
*
*       Copyright (c) 1990-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _commit() - flush buffer to disk
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <errno.h>
#include <io.h>
#include <internal.h>
#include <msdos.h>      /* for FOPEN */
#include <mtdll.h>
#include <stdlib.h>     /* for _doserrno */

/***
*int _commit(filedes) - flush buffer to disk
*
*Purpose:
*       Flushes cache buffers for the specified file handle to disk
*
*Entry:
*       int filedes - file handle of file
/*
*Exit:
*       returns success code
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _commit (
        int filedes
        )
{
        int retval;

        /* if filedes out of range, complain */
        if ( ((unsigned)filedes >= (unsigned)_nhandle) ||
             !(_osfile(filedes) & FOPEN) )
        {
                errno = EBADF;
                return (-1);
        }

#ifdef _MT
        _lock_fh(filedes);
        __try {
                if (_osfile(filedes) & FOPEN) {
#endif  /* _MT */

        if ( !FlushFileBuffers((HANDLE)_get_osfhandle(filedes)) ) {
                retval = GetLastError();
        }
        else {
                retval = 0;     /* return success */
        }

        /* map the OS return code to C errno value and return code */
        if (retval == 0)
                goto good;

        _doserrno = retval;

#ifdef _MT
                }
#endif  /* _MT */

        errno = EBADF;
        retval = -1;

good :
#ifdef _MT
        ; }
        __finally {
                _unlock_fh(filedes);
        }
#endif  /* _MT */
        return (retval);
}
