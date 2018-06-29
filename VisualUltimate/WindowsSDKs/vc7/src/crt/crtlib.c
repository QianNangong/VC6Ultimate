/***
*crtlib.c - CRT DLL initialization and termination routine (Win32, Dosx32)
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains initialization entry point for the CRT DLL
*       in the Win32 environment. It also contains some of the supporting
*       initialization and termination code.
*
*******************************************************************************/

#ifdef CRTDLL

#include <cruntime.h>
#include <oscalls.h>
#include <dos.h>
#include <internal.h>
#include <malloc.h>
#include <mbctype.h>
#include <mtdll.h>
#include <process.h>
#include <rterr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <awint.h>
#include <tchar.h>
#include <time.h>
#include <dbgint.h>
#include <setlocal.h>

/*
 * flag set iff _CRTDLL_INIT was called with DLL_PROCESS_ATTACH
 */
static int proc_attached = 0;

/*
 * command line, environment, and a few other globals
 */
wchar_t *_wcmdln = NULL;           /* points to wide command line */
char *_acmdln = NULL;              /* points to command line */

char *_aenvptr = NULL;      /* points to environment block */
wchar_t *_wenvptr = NULL;   /* points to wide environment block */

void (__cdecl * _aexit_rtn)(int) = _exit;   /* RT message return procedure */

extern int _newmode;    /* declared in <internal.h> */

int __error_mode = _OUT_TO_DEFAULT;

int __app_type = _UNKNOWN_APP;

static void __cdecl inherit(void);  /* local function */

/***
*int __[w]getmainargs - get values for args to main()
*
*Purpose:
*       This function invokes the command line parsing and copies the args
*       to main back through the passsed pointers. The reason for doing
*       this here, rather than having _CRTDLL_INIT do the work and exporting
*       the __argc and __argv, is to support the linked-in option to have
*       wildcard characters in filename arguments expanded.
*
*Entry:
*       int *pargc              - pointer to argc
*       _TCHAR ***pargv         - pointer to argv
*       _TCHAR ***penvp         - pointer to envp
*       int dowildcard          - flag (true means expand wildcards in cmd line)
*       _startupinfo * startinfo- other info to be passed to CRT DLL
*
*Exit:
*       Returns 0 on success, negative if _*setargv returns an error. Values
*       for the arguments to main() are copied through the passed pointers.
*
*******************************************************************************/


_CRTIMP int __cdecl __wgetmainargs (
        int *pargc,
        wchar_t ***pargv,
        wchar_t ***penvp,
        int dowildcard,
        _startupinfo * startinfo)
{
        int ret;


        /* set global new mode flag */
        _newmode = startinfo->newmode;

        if ( dowildcard )
            ret = __wsetargv(); /* do wildcard expansion after parsing args */
        else
            ret = _wsetargv();  /* NO wildcard expansion; just parse args */
        if (ret < 0)
            return ret;

        *pargc = __argc;
        *pargv = __wargv;

        /*
         * if wide environment does not already exist,
         * create it from multibyte environment
         */
        if (!_wenviron)
            __mbtow_environ();

        *penvp = _wenviron;

        return ret;
}



_CRTIMP int __cdecl __getmainargs (
        int *pargc,
        char ***pargv,
        char ***penvp,
        int dowildcard
        ,
        _startupinfo * startinfo
        )
{
        int ret;


        /* set global new mode flag */
        _newmode = startinfo->newmode;

        if ( dowildcard )
            ret = __setargv();  /* do wildcard expansion after parsing args */
        else
            ret = _setargv();   /* NO wildcard expansion; just parse args */
        if (ret < 0)
            return ret;

        *pargc = __argc;
        *pargv = __argv;
        *penvp = _environ;

        return ret;
}


/***
*BOOL _CRTDLL_INIT(hDllHandle, dwReason, lpreserved) - C DLL initialization.
*
*Purpose:
*       This routine does the C runtime initialization.
*
*Entry:
*
*Exit:
*
*******************************************************************************/

BOOL WINAPI _CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{

        if ( dwReason == DLL_PROCESS_ATTACH ) {
            /*
             * Dynamically allocate the OSVERSIONINFOA buffer, so we avoid
             * triggering the /GS buffer overrun detection.  That can't be
             * used here, since the guard cookie isn't available until we
             * initialize it from here!
             */
            OSVERSIONINFOA *posvi =
                (OSVERSIONINFOA *)_alloca(sizeof(OSVERSIONINFOA));

            /*
             * Get the full Win32 version.
             */
            posvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
            if ( !GetVersionExA(posvi) )
                return FALSE;

            _osplatform = posvi->dwPlatformId;
            _winmajor = posvi->dwMajorVersion;
            _winminor = posvi->dwMinorVersion;

            /*
             * The somewhat bizarre calculations of _osver and _winver are
             * required for backward compatibility (used to use GetVersion)
             */
            _osver = (posvi->dwBuildNumber) & 0x07fff;
            if ( _osplatform != VER_PLATFORM_WIN32_NT )
                _osver |= 0x08000;
            _winver = (_winmajor << 8) + _winminor;

#if defined (_M_IX86) && defined (_SYSCRT)

            /*
             * Make sure we are NOT running on Win32s
             */
            if ( _osplatform == VER_PLATFORM_WIN32s ) {
                __crtMessageBoxA("MSVCRT.DLL for Win32\n\nError: MSVCRT.DLL is not compatible with Win32s.",
                         "Microsoft Visual C++ Runtime Library",
                         MB_OK|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);
                return FALSE;
            }

#endif  /* defined (_M_IX86) && defined (_SYSCRT) */


#ifdef _MT
            if ( !_heap_init(1) )   /* initialize heap */
#else  /* _MT */
            if ( !_heap_init(0) )   /* initialize heap */
#endif  /* _MT */
                /*
                 * The heap cannot be initialized, return failure to the
                 * loader.
                 */
                return FALSE;

#ifdef _MT
            if(!_mtinit())          /* initialize multi-thread */
            {
                /*
                 * If the DLL load is going to fail, we must clean up
                 * all resources that have already been allocated.
                 */
                _heap_term();       /* heap is now invalid! */

                return FALSE;       /* fail DLL load on failure */
            }
#endif  /* _MT */

            if (_ioinit() < 0) {    /* inherit file info */
                /* Clean up already-allocated resources */
#ifdef _MT
                /* free TLS index, call _mtdeletelocks() */
                _mtterm();
#endif  /* _MT */

                _heap_term();       /* heap is now invalid! */

                return FALSE;       /* fail DLL load on failure */
            }

            _aenvptr = (char *)__crtGetEnvironmentStringsA();

            _acmdln = (char *)GetCommandLineA();
            _wcmdln = (wchar_t *)__crtGetCommandLineW();

#ifdef _MBCS
            /*
             * Initialize multibyte ctype table. Always done since it is
             * needed for processing the environment strings.
             */
            __initmbctable();
#endif  /* _MBCS */

            /*
             * For CRT DLL, since we don't know the type (wide or multibyte)
             * of the program, we create only the multibyte type since that
             * is by far the most likely case. Wide environment will be created
             * on demand as usual.
             */

            if (_setenvp() < 0 ||   /* get environ info */
                _cinit() != 0)      /* do C data initialize */
            {
                _ioterm();          /* shut down lowio */
#ifdef _MT
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
#endif  /* _MT */
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail DLL load on failure */
            }

            /*
             * Increment flag indicating process attach notification
             * has been received.
             */
            proc_attached++;

        }
        else if ( dwReason == DLL_PROCESS_DETACH ) {
            /*
             * if a client process is detaching, make sure minimal
             * runtime termination is performed and clean up our
             * 'locks' (i.e., delete critical sections).
             */
            if ( proc_attached > 0 ) {
                proc_attached--;

                /*
                 * Any basic clean-up done here may also need
                 * to be done below if Process Attach is partly
                 * processed and then a failure is encountered.
                 */

                if ( _C_Termination_Done == FALSE )
                    _cexit();

#ifdef _DEBUG
                /* Dump all memory leaks */
                if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_LEAK_CHECK_DF)
                {
                    _CrtSetDumpClient(NULL);
                    _CrtDumpMemoryLeaks();
                }
#endif  /* _DEBUG */

                /*
                 * What remains is to clean up the system resources we have
                 * used (handles, critical sections, memory,...,etc.). This
                 * needs to be done if the whole process is NOT terminating.
                 */
                if ( lpreserved == NULL )
                {
                    /*
                     * The process is NOT terminating so we must clean up...
                     */
                    _ioterm();
#ifdef _MT
                    /* free TLS index, call _mtdeletelocks() */
                    _mtterm();
#endif  /* _MT */

                    /* This should be the last thing the C run-time does */
                    _heap_term();   /* heap is now invalid! */
                }

            }
            else
                /* no prior process attach, just return */
                return FALSE;

        }
#ifdef _MT
        else if ( dwReason == DLL_THREAD_ATTACH )
        {
            _ptiddata ptd;
            if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) != NULL))
            {
                if (FLS_SETVALUE(__tlsindex, (LPVOID)ptd) ) {
                    /*
                     * Initialize of per-thread data
                     */
                    _initptd(ptd);

                    ptd->_tid = GetCurrentThreadId();
                    ptd->_thandle = (uintptr_t)(-1);
                } else
                {
                    _free_crt(ptd);
                    return FALSE;
                }
            } else
            {
                return FALSE;
            }
        }
        else if ( dwReason == DLL_THREAD_DETACH )
        {
            _freeptd(NULL);     /* free up per-thread CRT data */
        }
#endif  /* _MT */

        return TRUE;
}

/***
*_amsg_exit(rterrnum) - Fast exit fatal errors
*
*Purpose:
*       Exit the program with error code of 255 and appropriate error
*       message.
*
*Entry:
*       int rterrnum - error message number (amsg_exit only).
*
*Exit:
*       Calls exit() (for integer divide-by-0) or _exit() indirectly
*       through _aexit_rtn [amsg_exit].
*       For multi-thread: calls _exit() function
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _amsg_exit (
        int rterrnum
        )
{

        if ( (__error_mode == _OUT_TO_STDERR) || ((__error_mode ==
               _OUT_TO_DEFAULT) && (__app_type == _CONSOLE_APP)) )
            _FF_MSGBANNER();    /* write run-time error banner */

        _NMSG_WRITE(rterrnum);      /* write message */
        _aexit_rtn(255);        /* normally _exit(255) */
}


#if defined (_M_IX86)

/*
 * Functions to access user-visible, per-process variables
 */

/*
 * Macro to construct the name of the access function from the variable
 * name.
 */
#define AFNAME(var) __p_ ## var

/*
 * Macro to construct the access function's return value from the variable
 * name.
 */
#define AFRET(var)  &var

/*
 ***
 ***  Template
 ***

_CRTIMP __cdecl
AFNAME() (void)
{
        return AFRET();
}

 ***
 ***
 ***
 */

#ifdef _DEBUG

_CRTIMP long *
AFNAME(_crtAssertBusy) (void)
{
        return AFRET(_crtAssertBusy);
}

_CRTIMP long *
AFNAME(_crtBreakAlloc) (void)
{
        return AFRET(_crtBreakAlloc);
}

_CRTIMP int *
AFNAME(_crtDbgFlag) (void)
{
        return AFRET(_crtDbgFlag);
}

#endif  /* _DEBUG */

_CRTIMP char ** __cdecl
AFNAME(_acmdln) (void)
{
        return AFRET(_acmdln);
}

_CRTIMP wchar_t ** __cdecl
AFNAME(_wcmdln) (void)
{
        return AFRET(_wcmdln);
}

_CRTIMP unsigned int * __cdecl
AFNAME(_amblksiz) (void)
{
        return AFRET(_amblksiz);
}

_CRTIMP int * __cdecl
AFNAME(__argc) (void)
{
        return AFRET(__argc);
}

_CRTIMP char *** __cdecl
AFNAME(__argv) (void)
{
        return AFRET(__argv);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(__wargv) (void)
{
        return AFRET(__wargv);
}

_CRTIMP int * __cdecl
AFNAME(_commode) (void)
{
        return AFRET(_commode);
}

_CRTIMP int * __cdecl
AFNAME(_daylight) (void)
{
        return AFRET(_daylight);
}

_CRTIMP long * __cdecl
AFNAME(_dstbias) (void)
{
        return AFRET(_dstbias);
}

_CRTIMP char *** __cdecl
AFNAME(_environ) (void)
{
        return AFRET(_environ);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(_wenviron) (void)
{
        return AFRET(_wenviron);
}

_CRTIMP int * __cdecl
AFNAME(_fmode) (void)
{
        return AFRET(_fmode);
}

_CRTIMP int * __cdecl
AFNAME(_fileinfo) (void)
{
        return AFRET(_fileinfo);
}

_CRTIMP char *** __cdecl
AFNAME(__initenv) (void)
{
        return AFRET(__initenv);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(__winitenv) (void)
{
        return AFRET(__winitenv);
}

_CRTIMP FILE *
AFNAME(_iob) (void)
{
        return &_iob[0];
}

_CRTIMP unsigned char * __cdecl
AFNAME(_mbctype) (void)
{
        return &_mbctype[0];
}

_CRTIMP unsigned char * __cdecl
AFNAME(_mbcasemap) (void)
{
        return &_mbcasemap[0];
}

_CRTIMP int * __cdecl
AFNAME(__mb_cur_max) (void)
{
#ifdef _MT
        _ptiddata ptd = _getptd();
        pthreadlocinfo ptloci = ptd->ptlocinfo;

        __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */
        return AFRET(__MB_CUR_MAX(ptloci));
}


_CRTIMP unsigned int * __cdecl
AFNAME(_osver) (void)
{
        return AFRET(_osver);
}

_CRTIMP const unsigned short ** __cdecl
AFNAME(_pctype) (void)
{
#ifdef _MT
        _ptiddata ptd = _getptd();
        pthreadlocinfo ptloci = ptd->ptlocinfo;

        __UPDATE_LOCALE(ptd, ptloci);
        return AFRET(ptloci->pctype);
#else  /* _MT */
        return AFRET(_pctype);
#endif  /* _MT */
}

_CRTIMP const unsigned short ** __cdecl
AFNAME(_pwctype) (void)
{
        return AFRET(_pwctype);
}

_CRTIMP char **  __cdecl
AFNAME(_pgmptr) (void)
{
        return AFRET(_pgmptr);
}

_CRTIMP wchar_t ** __cdecl
AFNAME(_wpgmptr) (void)
{
        return AFRET(_wpgmptr);
}

_CRTIMP long * __cdecl
AFNAME(_timezone) (void)
{
        return AFRET(_timezone);
}

_CRTIMP char ** __cdecl
AFNAME(_tzname) (void)
{
        return &_tzname[0];
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winmajor) (void)
{
        return AFRET(_winmajor);
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winminor) (void)
{
        return AFRET(_winminor);
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winver) (void)
{
        return AFRET(_winver);
}

#endif  /* defined (_M_IX86) */

#endif  /* CRTDLL */
