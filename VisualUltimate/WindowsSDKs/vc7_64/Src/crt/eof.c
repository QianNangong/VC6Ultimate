/***
*eof.c - test a handle for end of file
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _eof() - determine if a file is at eof
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <internal.h>
#include <msdos.h>
#include <mtdll.h>

/***
*int _eof(filedes) - test a file for eof
*
*Purpose:
*       see if the file length is the same as the present position. if so, return
*       1. if not, return 0. if an error occurs, return -1
*
*Entry:
*       int filedes - handle of file to test
*
*Exit:
*       returns 1 if at eof
*       returns 0 if not at eof
*       returns -1 and sets errno if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _eof (
        int filedes
        )
{
        __int64 here;
        __int64 end;
        int retval;

        if ( ((unsigned)filedes >= (unsigned)_nhandle) ||
             !(_osfile(filedes) & FOPEN) )
        {
                errno = EBADF;
                _doserrno = 0;
                return(-1);
        }

#ifdef _MT

        /* Lock the file */
        _lock_fh(filedes);
        __try {
                if ( _osfile(filedes) & FOPEN ) {

#endif  /* _MT */

        /* See if the current position equals the end of the file. */

        if ( ((here = _lseeki64_lk(filedes, 0i64, SEEK_CUR)) == -1i64) ||
             ((end = _lseeki64_lk(filedes, 0i64, SEEK_END)) == -1i64) )
                retval = -1;
        else if ( here == end )
                retval = 1;
        else {
                _lseeki64_lk(filedes, here, SEEK_SET);
                retval = 0;
        }

#ifdef _MT

                }
                else {
                        errno = EBADF;
                        _doserrno = 0;
                        retval = -1;
                }
        }
        __finally {
                /* Unlock the file */
                _unlock_fh(filedes);
        }

#endif  /* _MT */

        /* Done */
        return(retval);
}
