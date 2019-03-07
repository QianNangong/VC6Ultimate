/***
*fdopen.c - open a file descriptor as stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fdopen() - opens a file descriptor as a stream, thus allowing
*       buffering, etc.
*
*******************************************************************************/

#include <cruntime.h>
#include <msdos.h>
#include <stdio.h>
#include <file2.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>
#include <tchar.h>
#include <errno.h>

/***
*FILE *_fdopen(filedes, mode) - open a file descriptor as a stream
*
*Purpose:
*       associates a stream with a file handle, thus allowing buffering, etc.
*       The mode must be specified and must be compatible with the mode
*       the file was opened with in the low level open.
*
*Entry:
*       int filedes - handle referring to open file
*       _TSCHAR *mode - file mode to use ("r", "w", "a", etc.)
*
*Exit:
*       returns stream pointer and sets FILE struct fields if successful
*       returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _tfdopen (
        int filedes,
        REG2 const _TSCHAR *mode
        )
{
        REG1 FILE *stream;
        int whileflag, tbflag, cnflag;
        int fileflag;

        _ASSERTE(mode != NULL);


        _ASSERTE((unsigned)filedes < (unsigned)_nhandle);
        _ASSERTE(_osfile(filedes) & FOPEN);

        if ( ((unsigned)filedes >= (unsigned)_nhandle) ||
             !(_osfile(filedes) & FOPEN) )
            return(NULL);


        /* First character must be 'r', 'w', or 'a'. */

        switch (*mode) {
            case _T('r'):
                fileflag = _IOREAD;
                break;
            case _T('w'):
            case _T('a'):
                fileflag = _IOWRT;
                break;
            default:
                return(NULL);  /* error */
        }

        /* There can be up to three more optional characters:
           (1) A single '+' character,
           (2) One of 'b' and 't' and
           (3) One of 'c' and 'n'.

           Note that currently, the 't' and 'b' flags are syntax checked
           but ignored.  'c' and 'n', however, are correctly supported.
        */

        whileflag=1;
        tbflag=cnflag=0;
        fileflag |= _commode;

        while(*++mode && whileflag)
            switch(*mode) {

                case _T('+'):
                    if (fileflag & _IORW)
                        whileflag=0;
                    else {
                        fileflag |= _IORW;
                        fileflag &= ~(_IOREAD | _IOWRT);
                    }
                    break;

                case _T('b'):
                case _T('t'):
                    if (tbflag)
                        whileflag=0;
                    else
                        tbflag=1;
                    break;

                case _T('c'):
                    if (cnflag)
                        whileflag = 0;
                    else {
                        cnflag = 1;
                        fileflag |= _IOCOMMIT;
                    }
                    break;

                case _T('n'):
                    if (cnflag)
                        whileflag = 0;
                    else {
                        cnflag = 1;
                        fileflag &= ~_IOCOMMIT;
                    }
                    break;

                default:
                    whileflag=0;
                    break;
            }

        /* Find a free stream; stream is returned 'locked'. */

        if ((stream = _getstream()) == NULL) {
            errno = EMFILE;
            return(NULL);
        }

#ifdef _MT
        __try {
#endif  /* _MT */

#ifndef CRTDLL
        _cflush++;  /* force library pre-termination procedure */
#endif  /* CRTDLL */

        stream->_flag = fileflag;
        stream->_file = filedes;

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

        return(stream);
}
