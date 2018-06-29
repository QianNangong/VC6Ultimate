/***
*crt0dat.c - 32-bit C run-time initialization/termination routines
*
*       Copyright (c) 1986-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the routines _cinit, exit, and _exit
*       for C run-time startup and termination.  _cinit and exit
*       are called from the _astart code in crt0.asm.
*       This module also defines several data variables used by the
*       runtime.
*
*       [NOTE: Lock segment definitions are at end of module.]
*
*******************************************************************************/

#include <cruntime.h>
#include <msdos.h>
#include <rtcapi.h>
#include <dos.h>
#include <oscalls.h>
#include <mtdll.h>
#include <internal.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <dbgint.h>
#include <sect_attribs.h>
#include <mbctype.h>

/* define errno */
#ifndef _MT
int errno = 0;            /* libc error value */
unsigned long _doserrno = 0;  /* OS system error value */
#endif  /* _MT */


/* define umask */
int _umaskval = 0;


/* define version info variables */

_CRTIMP unsigned int _osplatform = 0;
_CRTIMP unsigned int _osver = 0;
_CRTIMP unsigned int _winver = 0;
_CRTIMP unsigned int _winmajor = 0;
_CRTIMP unsigned int _winminor = 0;


/* argument vector and environment */

_CRTIMP int __argc = 0;
_CRTIMP char **__argv = NULL;
_CRTIMP wchar_t **__wargv = NULL;
_CRTIMP char **_environ = NULL;
_CRTIMP char **__initenv = NULL;
_CRTIMP wchar_t **_wenviron = NULL;
_CRTIMP wchar_t **__winitenv = NULL;
_CRTIMP char *_pgmptr = NULL;           /* ptr to program name */
_CRTIMP wchar_t *_wpgmptr = NULL;       /* ptr to wide program name */


/* callable exit flag */
char _exitflag = 0;

/*
 * flag indicating if C runtime termination has been done. set if exit,
 * _exit, _cexit or _c_exit has been called. checked when _CRTDLL_INIT
 * is called with DLL_PROCESS_DETACH.
 */
int _C_Termination_Done = FALSE;
int _C_Exit_Done = FALSE;

#ifndef CRTDLL
/*
 * Flag checked by getenv() and _putenv() to determine if the environment has
 * been initialized.
 */
int __env_initialized;

#endif  /* CRTDLL */

#ifdef _MBCS
/*
 * Flag to ensure multibyte ctype table is only initialized once
 */
int __mbctype_initialized;
#endif  /* _MBCS */


/*
 * NOTE: THE USE OF THE POINTERS DECLARED BELOW DEPENDS ON THE PROPERTIES
 * OF C COMMUNAL VARIABLES. SPECIFICALLY, THEY ARE NON-NULL IFF THERE EXISTS
 * A DEFINITION ELSEWHERE INITIALIZING THEM TO NON-NULL VALUES.
 */

/*
 * pointers to initialization functions
 */

_PVFV _FPinit;          /* floating point init. */

/*
 * pointers to initialization sections
 */

extern _CRTALLOC(".CRT$XIA") _PIFV __xi_a[];
extern _CRTALLOC(".CRT$XIZ") _PIFV __xi_z[];    /* C initializers */
extern _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */
extern _CRTALLOC(".CRT$XPA") _PVFV __xp_a[];
extern _CRTALLOC(".CRT$XPZ") _PVFV __xp_z[];    /* C pre-terminators */
extern _CRTALLOC(".CRT$XTA") _PVFV __xt_a[];
extern _CRTALLOC(".CRT$XTZ") _PVFV __xt_z[];    /* C terminators */

/*
 * pointers to the start and finish of the _onexit/atexit table
 */
_PVFV *__onexitbegin;
_PVFV *__onexitend;


/*
 * static (internal) functions that walk a table of function pointers,
 * calling each entry between the two pointers, skipping NULL entries
 *
 * _initterm needs to be exported for CRT DLL so that C++ initializers in the
 * client EXE / DLLs can be initialized.
 *
 * _initterm_e calls function pointers that return a nonzero error code to
 * indicate an initialization failed fatally.
 */
#ifdef CRTDLL
void __cdecl _initterm(_PVFV *, _PVFV *);
#else  /* CRTDLL */
static void __cdecl _initterm(_PVFV *, _PVFV *);
#endif  /* CRTDLL */
static int  __cdecl _initterm_e(_PIFV *, _PIFV *);


/***
*_cinit - C initialization
*
*Purpose:
*       This routine performs the shared DOS and Windows initialization.
*       The following order of initialization must be preserved -
*
*       1.  Check for devices for file handles 0 - 2
*       2.  Integer divide interrupt vector setup
*       3.  General C initializer routines
*
*Entry:
*       No parameters: Called from __crtstart and assumes data
*       set up correctly there.
*
*Exit:
*       Initializes C runtime data.
*       Returns 0 if all .CRT$XI internal initializations succeeded, else
*       the _RT_* fatal error code encountered.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _cinit (
        void
        )
{
        int initret;

        /*
         * initialize floating point package, if present
         */
        if ( _FPinit != NULL )
            (*_FPinit)();

        /*
         * do initializations
         */
        initret = _initterm_e( __xi_a, __xi_z );
        if ( initret != 0 )
            return initret;

#ifdef _RTC
        atexit(_RTC_Terminate);
#endif  /* _RTC */
        /*
         * do C++ initializations
         */
        _initterm( __xc_a, __xc_z );

        return 0;
}


/***
*exit(status), _exit(status), _cexit(void), _c_exit(void) - C termination
*
*Purpose:
*
*       Entry points:
*
*           exit(code):  Performs all the C termination functions
*               and terminates the process with the return code
*               supplied by the user.
*
*           _exit(code):  Performs a quick exit routine that does not
*               do certain 'high-level' exit processing.  The _exit
*               routine terminates the process with the return code
*               supplied by the user.
*
*           _cexit():  Performs the same C lib termination processing
*               as exit(code) but returns control to the caller
*               when done (i.e., does NOT terminate the process).
*
*           _c_exit():  Performs the same C lib termination processing
*               as _exit(code) but returns control to the caller
*               when done (i.e., does NOT terminate the process).
*
*       Termination actions:
*
*           exit(), _cexit():
*
*           1.  call user's terminator routines
*           2.  call C runtime preterminators
*
*           _exit(), _c_exit():
*
*           3.  call C runtime terminators
*           4.  return to DOS or caller
*
*       Notes:
*
*       The termination sequence is complicated due to the multiple entry
*       points sharing the common code body while having different entry/exit
*       sequences.
*
*       Multi-thread notes:
*
*       1. exit() should NEVER be called when mthread locks are held.
*          The exit() routine can make calls that try to get mthread locks.
*
*       2. _exit()/_c_exit() can be called from anywhere, with or without locks held.
*          Thus, _exit() can NEVER try to get locks (otherwise, deadlock
*          may occur).  _exit() should always 'work' (i.e., the process
*          should always terminate successfully).
*
*       3. Only one thread is allowed into the exit code (see _lockexit()
*          and _unlockexit() routines).
*
*Entry:
*       exit(), _exit()
*           int status - exit status (0-255)
*
*       _cexit(), _c_exit()
*           <no input>
*
*Exit:
*       exit(), _exit()
*           <EXIT to DOS>
*
*       _cexit(), _c_exit()
*           Return to caller
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

/* worker routine prototype */
static void __cdecl doexit (int code, int quick, int retcaller);

void __cdecl exit (
        int code
        )
{
        doexit(code, 0, 0); /* full term, kill process */
}


void __cdecl _exit (
        int code
        )
{
        doexit(code, 1, 0); /* quick term, kill process */
}

void __cdecl _cexit (
        void
        )
{
        doexit(0, 0, 1);    /* full term, return to caller */
}

void __cdecl _c_exit (
        void
        )
{
        doexit(0, 1, 1);    /* quick term, return to caller */
}


#ifdef _DEBUG
/***
* __freeCrtMemory()
*
* Purpose:
*       To free as much as CRT memory as possible. This helps in keeping CRT leaks as
*       minimum.
*
*******************************************************************************/
static void __freeCrtMemory()
{
    void **pptr;
    for (pptr = _wenviron; pptr != NULL && *pptr != NULL; ++pptr) {
        _free_crt(*pptr);
    }
    _free_crt(_wenviron);
    pptr = _environ;
    for (; pptr != NULL && *pptr != NULL; ++pptr) {
        _free_crt(*pptr);
    }
    _free_crt(_environ);
    _free_crt(__wargv);
    _free_crt(__argv);
    _free_crt(__onexitbegin);
    _ioterm();
#ifdef _MT
    _free_crt(__ptmbcinfo);
#endif  /* _MT */
}
#endif  /* _DEBUG */

static void __cdecl doexit (
        int code,
        int quick,
        int retcaller
        )
{
#ifdef _DEBUG
        static int fExit = 0;
#endif  /* _DEBUG */

#ifdef _MT
        _lockexit();        /* assure only 1 thread in exit path */
#endif  /* _MT */

        if (_C_Exit_Done == TRUE)                               /* if doexit() is being called recursively */
                TerminateProcess(GetCurrentProcess(),code);     /* terminate with extreme prejudice */
        _C_Termination_Done = TRUE;

        /* save callable exit flag (for use by terminators) */
        _exitflag = (char) retcaller;  /* 0 = term, !0 = callable exit */

        if (!quick) {

            /*
             * do _onexit/atexit() terminators
             * (if there are any)
             *
             * These terminators MUST be executed in reverse order (LIFO)!
             *
             * NOTE:
             *  This code assumes that __onexitbegin points
             *  to the first valid onexit() entry and that
             *  __onexitend points past the last valid entry.
             *  If __onexitbegin == __onexitend, the table
             *  is empty and there are no routines to call.
             */

            if (__onexitbegin) {
                while ( --__onexitend >= __onexitbegin )
                /*
                 * if current table entry is non-NULL,
                 * call thru it.
                 */
                if ( *__onexitend != NULL )
                    (**__onexitend)();
            }

            /*
             * do pre-terminators
             */
            _initterm(__xp_a, __xp_z);
        }

        /*
         * do terminators
         */
        _initterm(__xt_a, __xt_z);

#ifdef _DEBUG
        /* Dump all memory leaks */
        if (!fExit && _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_LEAK_CHECK_DF)
        {
            fExit = 1;
            __freeCrtMemory();
#ifndef CRTDLL
            _CrtDumpMemoryLeaks();
#endif  /* CRTDLL */
        }
#endif  /* _DEBUG */

        /* return to OS or to caller */

        if (retcaller) {
#ifdef _MT
            _unlockexit();      /* unlock the exit code path */
#endif  /* _MT */
            return;
        }


        _C_Exit_Done = TRUE;

        __crtExitProcess(code);
}

/***
* __crtExitProcess - CRT wrapper for ExitProcess
*
*Purpose:
*       If we're part of a managed app, then call the CorExitProcess,
*       otherwise call ExitProcess.  For managed apps, calling ExitProcess can
*       be problematic, because it doesn't give the managed FinalizerThread a
*       chance to clean up.
*
*       To determine if we're a managed app, we check if mscoree.dll is loaded.
*       Then, if CorExitProcess is available, we call it.
*
*Entry:
*       int status - exit code
*
*Exit:
*       Does not return
*
*Exceptions:
*
*******************************************************************************/

typedef void (WINAPI * PFN_EXIT_PROCESS)(UINT uExitCode);

void __cdecl __crtExitProcess (
        int status
        )
{
        HMODULE hmod;
        PFN_EXIT_PROCESS pfn;

        hmod = GetModuleHandle("mscoree.dll");
        if (hmod != NULL) {
            pfn = (PFN_EXIT_PROCESS)GetProcAddress(hmod, "CorExitProcess");
            if (pfn != NULL) {
                pfn(status);
            }
        }

        /*
         * Either mscoree.dll isn't loaded,
         * or CorExitProcess isn't exported from mscoree.dll,
         * or CorExitProcess returned (should never happen).
         * Just call ExitProcess.
         */

        ExitProcess(status);
}


#ifdef _MT
/***
* _lockexit - Aquire the exit code lock
*
*Purpose:
*       Makes sure only one thread is in the exit code at a time.
*       If a thread is already in the exit code, it must be allowed
*       to continue.  All other threads must pend.
*
*       Notes:
*
*       (1) It is legal for a thread that already has the lock to
*       try and get it again(!).  That is, consider the following
*       sequence:
*
*           (a) program calls exit()
*           (b) thread locks exit code
*           (c) user onexit() routine calls _exit()
*           (d) same thread tries to lock exit code
*
*       Since _exit() must ALWAYS be able to work (i.e., can be called
*       from anywhere with no regard for locking), we must make sure the
*       program does not deadlock at step (d) above.
*
*       (2) If a thread executing exit() or _exit() aquires the exit lock,
*       other threads trying to get the lock will pend forever.  That is,
*       since exit() and _exit() terminate the process, there is not need
*       for them to unlock the exit code path.
*
*       (3) Note that onexit()/atexit() routines call _lockexit/_unlockexit
*       to protect mthread access to the onexit table.
*
*       (4) The 32-bit OS semaphore calls DO allow a single thread to acquire
*       the same lock multiple times* thus, this version is straight forward.
*
*Entry: <none>
*
*Exit:
*       Calling thread has exit code path locked on return.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _lockexit (
        void
        )
{
        _mlock(_EXIT_LOCK1);
}

/***
* _unlockexit - Release exit code lock
*
*Purpose:
*       [See _lockexit() description above.]
*
*       This routine is called by _cexit(), _c_exit(), and onexit()/atexit().
*       The exit() and _exit() routines never unlock the exit code path since
*       they are terminating the process.
*
*Entry:
*       Exit code path is unlocked.
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _unlockexit (
        void
        )
{
        _munlock(_EXIT_LOCK1);
}

#endif  /* _MT */


/***
* static void _initterm(_PVFV * pfbegin, _PVFV * pfend) - call entries in
*       function pointer table
*
*Purpose:
*       Walk a table of function pointers, calling each entry, as follows:
*
*           1. walk from beginning to end, pfunctbl is assumed to point
*              to the beginning of the table, which is currently a null entry,
*              as is the end entry.
*           2. skip NULL entries
*           3. stop walking when the end of the table is encountered
*
*Entry:
*       _PVFV *pfbegin  - pointer to the beginning of the table (first
*                         valid entry).
*       _PVFV *pfend    - pointer to the end of the table (after last
*                         valid entry).
*
*Exit:
*       No return value
*
*Notes:
*       This routine must be exported in the CRT DLL model so that the client
*       EXE and client DLL(s) can call it to initialize their C++ constructors.
*
*Exceptions:
*       If either pfbegin or pfend is NULL, or invalid, all bets are off!
*
*******************************************************************************/

#ifdef CRTDLL
void __cdecl _initterm (
#else  /* CRTDLL */
static void __cdecl _initterm (
#endif  /* CRTDLL */
        _PVFV * pfbegin,
        _PVFV * pfend
        )
{
        /*
         * walk the table of function pointers from the bottom up, until
         * the end is encountered.  Do not skip the first entry.  The initial
         * value of pfbegin points to the first valid entry.  Do not try to
         * execute what pfend points to.  Only entries before pfend are valid.
         */
        while ( pfbegin < pfend )
        {
            /*
             * if current table entry is non-NULL, call thru it.
             */
            if ( *pfbegin != NULL )
                (**pfbegin)();
            ++pfbegin;
        }
}

/***
* static int  _initterm_e(_PIFV * pfbegin, _PIFV * pfend) - call entries in
*       function pointer table, return error code on any failure
*
*Purpose:
*       Walk a table of function pointers in the same way as _initterm, but
*       here the functions return an error code.  If an error is returned, it
*       will be a nonzero value equal to one of the _RT_* codes.
*
*Entry:
*       _PIFV *pfbegin  - pointer to the beginning of the table (first
*                         valid entry).
*       _PIFV *pfend    - pointer to the end of the table (after last
*                         valid entry).
*
*Exit:
*       No return value
*
*Notes:
*       This routine must be exported in the CRT DLL model so that the client
*       EXE and client DLL(s) can call it.
*
*Exceptions:
*       If either pfbegin or pfend is NULL, or invalid, all bets are off!
*
*******************************************************************************/

static int __cdecl _initterm_e (
        _PIFV * pfbegin,
        _PIFV * pfend
        )
{
        int ret = 0;

        /*
         * walk the table of function pointers from the bottom up, until
         * the end is encountered.  Do not skip the first entry.  The initial
         * value of pfbegin points to the first valid entry.  Do not try to
         * execute what pfend points to.  Only entries before pfend are valid.
         */
        while ( pfbegin < pfend  && ret == 0)
        {
            /*
             * if current table entry is non-NULL, call thru it.
             */
            if ( *pfbegin != NULL )
                ret = (**pfbegin)();
            ++pfbegin;
        }

        return ret;
}
