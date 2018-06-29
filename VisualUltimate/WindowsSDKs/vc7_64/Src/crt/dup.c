/***
*dup.c - duplicate file handles
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _dup() - duplicate file handles
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <errno.h>
#include <mtdll.h>
#include <io.h>
#include <msdos.h>
#include <internal.h>
#include <stdlib.h>

#ifdef _MT
static int __cdecl _dup_lk(int);
#endif  /* _MT */

/***
*int _dup(fh) - duplicate a file handle
*
*Purpose:
*       Assigns another file handle to the file associated with the
*       handle fh.  The next available file handle is assigned.
*
*       Multi-thread: Be sure not to hold two file handle locks
*       at the same time!
*
*Entry:
*       int fh - file handle to duplicate
*
*Exit:
*       returns new file handle if successful
*       returns -1 (and sets errno) if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _dup (
        int fh
        )
{
        int newfh;                      /* variable for new file handle */
#ifndef _MT
        ULONG dosretval;                /* o.s. return value */
        char fileinfo;                  /* _osfile info for file */
        intptr_t new_osfhandle;
#endif  /* _MT */

        /* validate file handle */
        if ( ((unsigned)fh >= (unsigned)_nhandle) ||
             !(_osfile(fh) & FOPEN) )
        {
                errno = EBADF;
                _doserrno = 0;  /* no o.s. error */
                return -1;
        }

#ifndef _MT
        fileinfo = _osfile(fh);         /* get file info for file */
#endif  /* _MT */

#ifdef _MT

        _lock_fh(fh);                   /* lock file handle */

        __try {
                if ( _osfile(fh) & FOPEN )
                        newfh = _dup_lk(fh);
                else {
                        errno = EBADF;
                        _doserrno = 0;
                        newfh = -1;
                }
        }
        __finally {
                if ( newfh != -1 )
                        _unlock_fh(newfh);
                _unlock_fh(fh);
        }

        return newfh;
}

static int __cdecl _dup_lk(
        int fh
        )
{
        int newfh;                      /* variable for new file handle */
        ULONG dosretval;                /* o.s. return value */
        char fileinfo;                  /* _osfile info for file */
        intptr_t new_osfhandle;
        fileinfo = _osfile(fh);         /* get file info for file */

        if ( !(_osfile(fh) & FOPEN) )
                return -1;

#endif  /* _MT */

        /* create duplicate handle */

        if ( (newfh = _alloc_osfhnd()) == -1 )
        {
                errno = EMFILE;         /* too many files error */
                _doserrno = 0L;         /* not an OS error */
                _unlock_fh(fh);
                return -1;              /* return error to caller */
        }

        /*
         * duplicate the file handle
         */
        if ( !(DuplicateHandle(GetCurrentProcess(),
                               (HANDLE)_get_osfhandle(fh),
                               GetCurrentProcess(),
                               (PHANDLE)&new_osfhandle,
                               0L,
                               TRUE,
                               DUPLICATE_SAME_ACCESS)) )
        {
                dosretval = GetLastError();
        }
        else {
                _set_osfhnd(newfh, new_osfhandle);
                dosretval = 0;
        }

        if (dosretval) {
                /* o.s. error -- map and return */
                _dosmaperr(dosretval);
                return -1;
        }

        /*
         * copy the _osfile value, with the FNOINHERIT bit cleared
         */
        _osfile(newfh) = fileinfo & ~FNOINHERIT;

        return newfh;
}
