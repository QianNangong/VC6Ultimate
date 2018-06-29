/***
*flength.c - find length of a file
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _filelength() - find the length of a file
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <internal.h>
#include <msdos.h>
#include <mtdll.h>
#include <stddef.h>
#include <stdlib.h>

/***
*long _filelength(filedes) - find length of a file
*
*Purpose:
*       Returns the length in bytes of the specified file.
*
*Entry:
*       int filedes - handle referring to file to find length of
*
*Exit:
*       returns length of file in bytes
*       returns -1L if fails
*
*Exceptions:
*
*******************************************************************************/

long __cdecl _filelength (
        int filedes
        )
{
        long length;
        long here;

        if ( ((unsigned)filedes >= (unsigned)_nhandle) ||
             !(_osfile(filedes) & FOPEN) )
        {
                errno = EBADF;
                _doserrno = 0L;         /* not an OS error */
                return(-1L);
        }

#ifdef _MT
        _lock_fh(filedes);
        __try {
                if ( _osfile(filedes) & FOPEN ) {
#endif  /* _MT */

        /* Seek to end to get length of file. */
        if ( (here = _lseek_lk(filedes, 0L, SEEK_CUR)) == -1L )
                length = -1L;   /* return error */
        else {
                length = _lseek_lk(filedes, 0L, SEEK_END);
                if ( here != length )
                        _lseek_lk(filedes, here, SEEK_SET);
        }

#ifdef _MT
                }
                else {
                        errno = EBADF;
                        _doserrno = 0L;
                        length = -1L;
                }
        }
        __finally {
                _unlock_fh(filedes);
        }
#endif  /* _MT */

        return(length);
}
