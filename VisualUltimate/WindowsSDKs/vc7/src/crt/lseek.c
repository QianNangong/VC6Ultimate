/***
*lseek.c - change file position
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _lseek() - move the file pointer
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <mtdll.h>
#include <io.h>
#include <internal.h>
#include <stdlib.h>
#include <errno.h>
#include <msdos.h>
#include <stdio.h>

/***
*long _lseek(fh,pos,mthd) - move the file pointer
*
*Purpose:
*       Moves the file pointer associated with fh to a new position.
*       The new position is pos bytes (pos may be negative) away
*       from the origin specified by mthd.
*
*       If mthd == SEEK_SET, the origin in the beginning of file
*       If mthd == SEEK_CUR, the origin is the current file pointer position
*       If mthd == SEEK_END, the origin is the end of the file
*
*       Multi-thread:
*       _lseek()    = locks/unlocks the file
*       _lseek_lk() = does NOT lock/unlock the file (it is assumed that
*                     the caller has the aquired the file lock,if needed).
*
*Entry:
*       int fh - file handle to move file pointer on
*       long pos - position to move to, relative to origin
*       int mthd - specifies the origin pos is relative to (see above)
*
*Exit:
*       returns the offset, in bytes, of the new position from the beginning
*       of the file.
*       returns -1L (and sets errno) if fails.
*       Note that seeking beyond the end of the file is not an error.
*       (although seeking before the beginning is.)
*
*Exceptions:
*
*******************************************************************************/

#ifdef _MT

/* define locking/validating lseek */
long __cdecl _lseek (
        int fh,
        long pos,
        int mthd
        )
{
        int r;

        /* validate fh */
        if ( ((unsigned)fh >= (unsigned)_nhandle) ||
             !(_osfile(fh) & FOPEN) )
        {
                /* bad file handle */
                errno = EBADF;
                _doserrno = 0;  /* not o.s. error */
                return -1;
        }

        _lock_fh(fh);                   /* lock file handle */

        __try {
                if ( _osfile(fh) & FOPEN )
                        r = _lseek_lk(fh, pos, mthd);   /* seek */
                else {
                        errno = EBADF;
                        _doserrno = 0;
                        r = -1;
                }
        }
        __finally {
                _unlock_fh(fh);         /* unlock file handle */
        }

        return r;
}

/* define core _lseek -- doesn't lock or validate fh */
long __cdecl _lseek_lk (
        int fh,
        long pos,
        int mthd
        )
{
        ULONG newpos;                   /* new file position */
        ULONG dosretval;                /* o.s. return value */
        HANDLE osHandle;        /* o.s. handle value */

#else  /* _MT */

/* define normal _lseek */
long __cdecl _lseek (
        int fh,
        long pos,
        int mthd
        )
{
        ULONG newpos;                   /* new file position */
        ULONG dosretval;                /* o.s. return value */
        HANDLE osHandle;        /* o.s. handle value */

        /* validate fh */
        if ( ((unsigned)fh >= (unsigned)_nhandle) ||
             !(_osfile(fh) & FOPEN) )
        {
                /* bad file handle */
                errno = EBADF;
                _doserrno = 0;  /* not o.s. error */
                return -1;
        }

#endif  /* _MT */

        /* tell o.s. to seek */

#if SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END
    #error Xenix and Win32 seek constants not compatible
#endif  /* SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END */
        if ((osHandle = (HANDLE)_get_osfhandle(fh)) == (HANDLE)-1)
        {
            errno = EBADF;
            return -1;
        }

        if ((newpos = SetFilePointer(osHandle, pos, NULL, mthd)) == -1)
                dosretval = GetLastError();
        else
                dosretval = 0;

        if (dosretval) {
                /* o.s. error */
                _dosmaperr(dosretval);
                return -1;
        }

        _osfile(fh) &= ~FEOFLAG;        /* clear the ctrl-z flag on the file */
        return newpos;                  /* return */
}
