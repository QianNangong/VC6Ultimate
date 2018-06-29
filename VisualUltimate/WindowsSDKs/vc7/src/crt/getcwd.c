/***
*getcwd.c - get current working directory
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*       contains functions _getcwd, _getdcwd and _getcdrv for getting the
*       current working directory.  getcwd gets the c.w.d. for the default disk
*       drive, whereas _getdcwd allows one to get the c.w.d. for whatever disk
*       drive is specified. _getcdrv gets the current drive.
*
*******************************************************************************/

#include <cruntime.h>
#include <mtdll.h>
#include <msdos.h>
#include <errno.h>
#include <malloc.h>
#include <oscalls.h>
#include <stdlib.h>
#include <internal.h>
#include <direct.h>
#include <tchar.h>


/***
*_TSCHAR *_getcwd(pnbuf, maxlen) - get current working directory of default drive
*
*Purpose:
*       _getcwd gets the current working directory for the user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored.
*       An entry point "_getdcwd()" is defined with takes the above
*       parameters, plus a drive number.  "_getcwd()" is implemented
*       as a call to "_getcwd()" with the default drive (0).
*
*       If pnbuf = NULL, maxlen is ignored, and a buffer is automatically
*       allocated using malloc() -- a pointer to which is returned by
*       _getcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       _TSCHAR *pnbuf = pointer to a buffer maintained by the user;
*       int maxlen = length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the c.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tgetcwd (
        _TSCHAR *pnbuf,
        int maxlen
        )
{
        _TSCHAR *retval;

#ifdef _MT
        _mlock( _ENV_LOCK );
        __try {
#endif  /* _MT */

#ifdef WPRFLAG
        retval = _wgetdcwd_lk(0, pnbuf, maxlen);
#else  /* WPRFLAG */
        retval = _getdcwd_lk(0, pnbuf, maxlen);
#endif  /* WPRFLAG */

#ifdef _MT
        }
        __finally {
            _munlock( _ENV_LOCK );
        }
#endif  /* _MT */

        return retval;
}


/***
*_TSCHAR *_getdcwd(drive, pnbuf, maxlen) - get c.w.d. for given drive
*
*Purpose:
*       _getdcwd gets the current working directory for the user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored,
*       and a buffer is automatically allocated using malloc() --
*       a pointer to which is returned by _getdcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       int drive   - number of the drive being inquired about
*                     0 = default, 1 = 'a:', 2 = 'b:', etc.
*       _TSCHAR *pnbuf - pointer to a buffer maintained by the user;
*       int maxlen  - length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the c.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/


#ifdef _MT

_TSCHAR * __cdecl _tgetdcwd (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )
{
        _TSCHAR *retval;

#ifdef _MT
        _mlock( _ENV_LOCK );
        __try {
#endif  /* _MT */

#ifdef WPRFLAG
        retval = _wgetdcwd_lk(drive, pnbuf, maxlen);
#else  /* WPRFLAG */
        retval = _getdcwd_lk(drive, pnbuf, maxlen);
#endif  /* WPRFLAG */

#ifdef _MT
        }
        _finally {
            _munlock( _ENV_LOCK );
        }
#endif  /* _MT */

        return retval;
}

#ifdef WPRFLAG
wchar_t * __cdecl _wgetdcwd_lk (
#else  /* WPRFLAG */
char * __cdecl _getdcwd_lk (
#endif  /* WPRFLAG */
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )
#else  /* _MT */

_TSCHAR * __cdecl _tgetdcwd (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )
#endif  /* _MT */

{
        _TSCHAR *p;
        _TSCHAR dirbuf[_MAX_PATH];
        _TSCHAR drvstr[4];
        int len;
        _TSCHAR *pname; /* only used as argument to GetFullPathName */

        /*
         * GetCurrentDirectory only works for the default drive in Win32
         */
        if ( drive != 0 ) {
            /*
             * Not the default drive - make sure it's valid.
             */
            if ( !_validdrive(drive) ) {
                _doserrno = ERROR_INVALID_DRIVE;
                errno = EACCES;
                return NULL;
            }

            /*
             * Get the current directory string on that drive and its length
             */
            drvstr[0] = _T('A') - 1 + drive;
            drvstr[1] = _T(':');
            drvstr[2] = _T('.');
            drvstr[3] = _T('\0');
            len = GetFullPathName( drvstr,
                                   sizeof(dirbuf) / sizeof(_TSCHAR),
                                   dirbuf,
                                   &pname );

        } else {

            /*
             * Get the current directory string and its length
             */
            len = GetCurrentDirectory( sizeof(dirbuf) / sizeof(_TSCHAR),
                                       (LPTSTR)dirbuf );
        }

        /* API call failed, or buffer not large enough */
        if ( len == 0 || ++len > sizeof(dirbuf)/sizeof(_TSCHAR) )
            return NULL;

        /*
         * Set up the buffer.
         */
        if ( (p = pnbuf) == NULL ) {
            /*
             * Allocate a buffer for the user.
             */
            if ( (p = (_TSCHAR *)malloc(__max(len, maxlen) * sizeof(_TSCHAR)))
                 == NULL )
            {
                errno = ENOMEM;
                return NULL;
            }
        }
        else if ( len > maxlen ) {
            /*
             * Won't fit in the user-supplied buffer!
             */
            errno = ERANGE; /* Won't fit in user buffer */
            return NULL;
        }

        /*
         * Place the current directory string into the user buffer
         */

        return _tcscpy(p, dirbuf);
}

#ifndef WPRFLAG

/***
*int _validdrive( unsigned drive ) -
*
*Purpose: returns non zero if drive is a valid drive number.
*
*Entry: drive = 0 => default drive, 1 => a:, 2 => b: ...
*
*Exit:  0 => drive does not exist.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _validdrive (
    unsigned drive
    )
{
        unsigned retcode;
        char drvstr[4];

        if ( drive == 0 )
            return 1;

        drvstr[0] = 'A' + drive - 1;
        drvstr[1] = ':';
        drvstr[2] = '\\';
        drvstr[3] = '\0';

        if ( ((retcode = GetDriveType( drvstr )) == DRIVE_UNKNOWN) ||
             (retcode == DRIVE_NO_ROOT_DIR) )
            return 0;

        return 1;
}

#endif  /* WPRFLAG */
