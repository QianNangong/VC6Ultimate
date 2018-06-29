/***
*dllcrt0.c - C runtime initialization routine for a DLL with linked-in C R-T
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This the startup routine for a DLL which is linked with its own
*       C run-time code.  It is similar to the routine _mainCRTStartup()
*       in the file CRT0.C, except that there is no main() in a DLL.
*
*******************************************************************************/

#ifndef CRTDLL

#include <cruntime.h>
#include <dos.h>
#include <internal.h>
#include <mtdll.h>
#include <stdlib.h>
#include <string.h>
#include <rterr.h>
#include <oscalls.h>
#define _DECL_DLLMAIN   /* enable prototypes for DllMain and _CRT_INIT */
#include <process.h>
#include <awint.h>
#include <tchar.h>
#include <dbgint.h>
#include <rtcapi.h>

/*
 * flag set iff _CRTDLL_INIT was called with DLL_PROCESS_ATTACH
 */
static int __proc_attached = 0;

/*
 * command line, environment, and a few other globals
 */
char *_acmdln;              /* points to command line */

char *_aenvptr = NULL;      /* points to environment block */
wchar_t *_wenvptr = NULL;   /* points to wide environment block */

void (__cdecl * _aexit_rtn)(int) = _exit;   /* RT message return procedure */

/*
 * _error_mode and _apptype, together, determine how error messages are
 * written out.
 */
int __error_mode = _OUT_TO_DEFAULT;
int __app_type = _UNKNOWN_APP;

/*
 * User routine DllMain is called on all notifications
 */
extern BOOL WINAPI DllMain(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        ) ;

/* _pRawDllMain MUST be a common variable, not extern nor initialized! */

BOOL (WINAPI *_pRawDllMain)(HANDLE, DWORD, LPVOID);

/***
*BOOL WINAPI _CRT_INIT(hDllHandle, dwReason, lpreserved) -
*       C Run-Time initialization for a DLL linked with a C run-time library.
*
*Purpose:
*       This routine does the C run-time initialization or termination.
*       For the multi-threaded run-time library, it also cleans up the
*       multi-threading locks on DLL termination.
*
*Entry:
*
*Exit:
*
*NOTES:
*       This routine must be the entry point for the DLL.
*
*******************************************************************************/

BOOL WINAPI _CRT_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        /*
         * Start-up code only gets executed when the process is initialized
         */

        if ( dwReason == DLL_PROCESS_ATTACH )
        {
            /*
             * Dynamically allocate the OSVERSIONINFOA buffer, so we avoid
             * triggering the /GS buffer overrun detection.  That can't be
             * used here, since the guard cookie isn't available until we
             * initialize it from here!
             */
            OSVERSIONINFOA *posvi =
                (OSVERSIONINFOA *)_alloca(sizeof(OSVERSIONINFOA));

            /*
             * Get the full Win32 version
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

#ifdef _MT
            if ( !_heap_init(1) )   /* initialize heap */
#else  /* _MT */
            if ( !_heap_init(0) )   /* initialize heap */
#endif  /* _MT */
                return FALSE;       /* fail to load DLL */

#ifdef _MT
            if(!_mtinit())          /* initialize multi-thread */
            {
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail to load DLL */
            }
#endif  /* _MT */

            /*
             * Initialize the Runtime Checks stuff
             */
#ifdef _RTC
            _RTC_Initialize();
#endif  /* _RTC */
            _acmdln = (char *)GetCommandLineA();
            _aenvptr = (char *)__crtGetEnvironmentStringsA();

            if (_ioinit() < 0) {    /* initialize lowio */
#ifdef _MT
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
#endif  /* _MT */
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail to load DLL */
            }

            if (_setargv() < 0 ||   /* get cmd line info */
                _setenvp() < 0 ||   /* get environ info */
                _cinit() != 0)      /* do C data initialize */
            {
                _ioterm();          /* shut down lowio */
#ifdef _MT
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
#endif  /* _MT */
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail to load DLL */
            }

            /*
             * increment flag to indicate process attach notification
             * has been received
             */
            __proc_attached++;
        }

        else if ( dwReason == DLL_PROCESS_DETACH )
        {
            if ( __proc_attached > 0 )
            {
                __proc_attached--;

                /*
                 * Any basic clean-up code that goes here must be duplicated
                 * below in _DllMainCRTStartup for the case where the user's
                 * DllMain() routine fails on a Process Attach notification.
                 * This does not include calling user C++ destructors, etc.
                 */

                if ( _C_Termination_Done == FALSE )
                    _cexit();

#ifdef _DEBUG
                /* Dump all memory leaks */
                if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_LEAK_CHECK_DF)
                    _CrtDumpMemoryLeaks();
#endif  /* _DEBUG */

                /* Shut down lowio */
                _ioterm();

#ifdef _MT
                _mtterm();
#endif  /* _MT */

                /* This should be the last thing the C run-time does */
                _heap_term();   /* heap is now invalid! */
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
            _freeptd(NULL);         /* free up per-thread CRT data */
        }
#endif  /* _MT */

        return TRUE ;
}

/***
*BOOL WINAPI _DllMainCRTStartup(hDllHandle, dwReason, lpreserved) -
*       C Run-Time initialization for a DLL linked with a C run-time library.
*
*Purpose:
*       This routine does the C run-time initialization or termination
*       and then calls the user code notification handler "DllMain".
*       For the multi-threaded run-time library, it also cleans up the
*       multi-threading locks on DLL termination.
*
*Entry:
*
*Exit:
*
*NOTES:
*       This routine is the preferred entry point. _CRT_INIT may also be
*       used, or the user may supply his/her own entry and call _CRT_INIT
*       from within it, but this is not the preferred method.
*
*******************************************************************************/

BOOL WINAPI _DllMainCRTStartup(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        BOOL retcode = TRUE;

        /*
         * If this is a process detach notification, check that there has
         * has been a prior process attach notification.
         */
        if ( (dwReason == DLL_PROCESS_DETACH) && (__proc_attached == 0) )
            /*
             * no prior process attach notification. just return
             * without doing anything.
             */
            return FALSE;

        if ( dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH )
        {
            if ( _pRawDllMain )
                retcode = (*_pRawDllMain)(hDllHandle, dwReason, lpreserved);

            if ( retcode )
                retcode = _CRT_INIT(hDllHandle, dwReason, lpreserved);

            if ( !retcode )
                return FALSE;
        }

        retcode = DllMain(hDllHandle, dwReason, lpreserved);

        if ( (dwReason == DLL_PROCESS_ATTACH) && !retcode )
            /*
             * The user's DllMain routine returned failure, the C runtime
             * needs to be cleaned up. Do this by calling _CRT_INIT again,
             * this time imitating DLL_PROCESS_DETACH. Note this will also
             * clear the __proc_attached flag so the cleanup will not be
             * repeated upon receiving the real process detach notification.
             */
            _CRT_INIT(hDllHandle, DLL_PROCESS_DETACH, lpreserved);

        if ( (dwReason == DLL_PROCESS_DETACH) ||
             (dwReason == DLL_THREAD_DETACH) )
        {
            if ( _CRT_INIT(hDllHandle, dwReason, lpreserved) == FALSE )
                retcode = FALSE ;

            if ( retcode && _pRawDllMain )
                retcode = (*_pRawDllMain)(hDllHandle, dwReason, lpreserved);
        }

        return retcode ;
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


#endif  /* CRTDLL */
