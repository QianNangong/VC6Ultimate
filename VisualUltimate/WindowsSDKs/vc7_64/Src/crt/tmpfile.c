/***
*tmpfile.c - create unique file name or file
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines tmpnam() and tmpfile().
*
*******************************************************************************/

#include <cruntime.h>
#include <errno.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <mtdll.h>
#include <share.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <file2.h>
#include <internal.h>
#include <tchar.h>
#include <dbgint.h>

/*
 * Buffers used by tmpnam() and tmpfile() to build filenames.
 */
static _TSCHAR namebuf0[L_tmpnam] = { 0 };      /* used by tmpnam()  */
static _TSCHAR namebuf1[L_tmpnam] = { 0 };      /* used by tmpfile() */

/*
 * Initializing function for namebuf0 and namebuf1.
 */
#ifdef _UNICODE
static void __cdecl winit_namebuf(int);
#else  /* _UNICODE */
static void __cdecl init_namebuf(int);
#endif  /* _UNICODE */

/*
 * Generator function that produces temporary filenames
 */
#ifdef _UNICODE
static int __cdecl wgenfname(wchar_t *);
#else  /* _UNICODE */
static int __cdecl genfname(char *);
#endif  /* _UNICODE */


/***
*_TSCHAR *tmpnam(_TSCHAR *s) - generate temp file name
*
*Purpose:
*       Creates a file name that is unique in the directory specified by
*       _P_tmpdir in stdio.h.  Places file name in string passed by user or
*       in static mem if pass NULL.
*
*Entry:
*       _TSCHAR *s - ptr to place to put temp name
*
*Exit:
*       returns pointer to constructed file name (s or address of static mem)
*       returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _ttmpnam (
        _TSCHAR *s
        )
{
        _TSCHAR *pfnam = NULL;
#ifdef _MT
        _ptiddata ptd;

        if ( !_mtinitlocknum( _TMPNAM_LOCK ))
                return NULL;

        _mlock(_TMPNAM_LOCK);

        __try {
#endif  /* _MT */

        /*
         * Initialize namebuf0, if needed. Otherwise, call genfname() to
         * generate the next filename.
         */
        if ( *namebuf0 == 0 ) {
#ifdef _UNICODE
                winit_namebuf(0);
#else  /* _UNICODE */
                init_namebuf(0);
#endif  /* _UNICODE */
        }
#ifdef _UNICODE
        else if ( wgenfname(namebuf0) )
#else  /* _UNICODE */
        else if ( genfname(namebuf0) )
#endif  /* _UNICODE */
                goto tmpnam_err;

        /*
         * Generate a filename that doesn't already exist.
         */
        while ( _taccess(namebuf0, 0) == 0 )
#ifdef _UNICODE
                if ( wgenfname(namebuf0) )
#else  /* _UNICODE */
                if ( genfname(namebuf0) )
#endif  /* _UNICODE */
                        goto tmpnam_err;

        /*
         * Filename has successfully been generated.
         */
        if ( s == NULL )
#ifdef _MT
        {
                /*
                 * Use a per-thread buffer to hold the generated file name.
                 * If there isn't one, and one cannot be created, just use
                 * namebuf0.
                 */
                ptd = _getptd();
#ifdef _UNICODE
                if ( (ptd->_wnamebuf0 != NULL) || ((ptd->_wnamebuf0 =
                      _malloc_crt(L_tmpnam * sizeof(wchar_t))) != NULL) )
                {
                        s = ptd->_wnamebuf0;
                        wcscpy(s, namebuf0);
                }
#else  /* _UNICODE */
                if ( (ptd->_namebuf0 != NULL) || ((ptd->_namebuf0 =
                      _malloc_crt(L_tmpnam)) != NULL) )
                {
                        s = ptd->_namebuf0;
                        strcpy(s, namebuf0);
                }
#endif  /* _UNICODE */
                else
                        s = namebuf0;
        }
#else  /* _MT */
                s = namebuf0;
#endif  /* _MT */
        else
                _tcscpy(s, namebuf0);

        pfnam = s;

        /*
         * All errors come here.
         */
tmpnam_err:

#ifdef _MT
        ; }
        __finally {
                _munlock(_TMPNAM_LOCK);
        }
#endif  /* _MT */

        return pfnam;
}

#ifndef _UNICODE

/***
*FILE *tmpfile() - create a temporary file
*
*Purpose:
*       Creates a temporary file with the file mode "w+b".  The file
*       will be automatically deleted when closed or the program terminates
*       normally.
*
*Entry:
*       None.
*
*Exit:
*       Returns stream pointer to opened file.
*       Returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl tmpfile (
        void
        )
{
        FILE *stream;
        FILE *return_stream = NULL;
        int fh;

#ifdef _MT
        int stream_lock_held = 0;

        if ( !_mtinitlocknum( _TMPNAM_LOCK ))
                return NULL;

        _mlock(_TMPNAM_LOCK);

        __try {
#endif  /* _MT */

        /*
         * Initialize namebuf1, if needed. Otherwise, call genfname() to
         * generate the next filename.
         */
        if ( *namebuf1 == 0 ) {
                init_namebuf(1);
        }
        else if ( genfname(namebuf1) )
                goto tmpfile_err;

        /*
         * Get a free stream.
         *
         * Note: In multi-thread models, the stream obtained below is locked!
         */
        if ( (stream = _getstream()) == NULL ) {
                errno = EMFILE;
                goto tmpfile_err;
        }

#ifdef _MT
        stream_lock_held = 1;
#endif  /* _MT */
        /*
         * Create a temporary file.
         *
         * Note: The loop below will only create a new file. It will NOT
         * open and truncate an existing file. Either behavior is probably
         * legal under ANSI (4.9.4.3 says tmpfile "creates" the file, but
         * also says it is opened with mode "wb+"). However, the behavior
         * implemented below is compatible with prior versions of MS-C and
         * makes error checking easier.
         */
        while ( ((fh = _sopen(namebuf1,
                              _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY |
                                _O_TEMPORARY,
                              _SH_DENYNO,
                              _S_IREAD | _S_IWRITE
                             ))
            == -1) && (errno == EEXIST) )
                if ( genfname(namebuf1) )
                        break;

        /*
         * Check that the loop above did indeed create a temporary
         * file.
         */
        if ( fh == -1 )
                goto tmpfile_err;

        /*
         * Initialize stream
         */
#ifdef _DEBUG
        if ( (stream->_tmpfname = _malloc_crt( (_tcslen( namebuf1 ) + 1) *
               sizeof(_TSCHAR) )) == NULL )
#else  /* _DEBUG */
        if ( (stream->_tmpfname = _tcsdup( namebuf1 )) == NULL )
#endif  /* _DEBUG */
        {
                /* close the file, then branch to error handling */
                _close(fh);
                goto tmpfile_err;
        }
#ifdef _DEBUG
        _tcscpy( stream->_tmpfname, namebuf1 );
#endif  /* _DEBUG */
        stream->_cnt = 0;
        stream->_base = stream->_ptr = NULL;
        stream->_flag = _commode | _IORW;
        stream->_file = fh;

        return_stream = stream;

        /*
         * All errors branch to the label below.
         */
tmpfile_err:

#ifdef _MT
        ; }
        __finally {
                if ( stream_lock_held )
                        _unlock_str(stream);
                _munlock(_TMPNAM_LOCK);
        }
#endif  /* _MT */

        return return_stream;
}

#endif  /* _UNICODE */

/***
*static void init_namebuf(flag) - initializes the namebuf arrays
*
*Purpose:
*       Called once each for namebuf0 and namebuf1, to initialize
*       them.
*
*Entry:
*       int flag            - flag set to 0 if namebuf0 is to be initialized,
*                             non-zero (1) if namebuf1 is to be initialized.
*Exit:
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
static void __cdecl winit_namebuf(
#else  /* _UNICODE */
static void __cdecl init_namebuf(
#endif  /* _UNICODE */
        int flag
        )
{
        _TSCHAR *p, *q;

        if ( flag == 0 )
            p = namebuf0;
        else
            p = namebuf1;

        /*
         * Put in the path prefix. Make sure it ends with a slash or
         * backslash character.
         */
#ifdef _UNICODE
        wcscpy(p, _wP_tmpdir);
#else  /* _UNICODE */
        strcpy(p, _P_tmpdir);
#endif  /* _UNICODE */
        q = p + sizeof(_P_tmpdir) - 1;      /* same as p + _tcslen(p) */

        if  ( (*(q - 1) != _T('\\')) && (*(q - 1) != _T('/')) )
                *(q++) = _T('\\');

        /*
         * Append the leading character of the filename.
         */
        if ( flag )
                /* for tmpfile() */
                *(q++) = _T('t');
        else
                /* for tmpnam() */
                *(q++) = _T('s');

        /*
         * Append the process id, encoded in base 32. Note this makes
         * p back into a string again (i.e., terminated by a '\0').
         */
        _ultot((unsigned long)_getpid(), q, 32);
        _tcscat(p, _T("."));
}


/***
*static int genfname(_TSCHAR *fname) -
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
static int __cdecl wgenfname (
#else  /* _UNICODE */
static int __cdecl genfname (
#endif  /* _UNICODE */
        _TSCHAR *fname
        )
{
        _TSCHAR *p;
        _TSCHAR pext[4];
        unsigned long extnum;

        p = _tcsrchr(fname, _T('.'));

        p++;

        if ( (extnum = _tcstoul(p, NULL, 32) + 1) >= (unsigned long)TMP_MAX )
                return -1;

        _tcscpy(p, _ultot(extnum, pext, 32));

        return 0;
}

#if !defined (_UNICODE) && !defined (CRTDLL)

/***
*void __inc_tmpoff(void) - force external reference for _tmpoff
*
*Purpose:
*       Forces an external reference to be generate for _tmpoff, which is
*       is defined in cinittmp.obj. This has the forces cinittmp.obj to be
*       pulled in, making a call to rmtmp part of the termination.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/


extern int _tmpoff;

void __inc_tmpoff(
        void
        )
{
        _tmpoff++;
}

#endif  /* !defined (_UNICODE) && !defined (CRTDLL) */
