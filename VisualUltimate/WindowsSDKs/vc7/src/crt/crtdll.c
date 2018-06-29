/***
*crtdll.c - CRT initialization for a DLL using the MSVCRT* model of C run-time
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the initialization entry point for the C run-time
*       stub in this DLL.  All C run-time code is located in the C Run-Time
*       Library DLL "MSVCRT*.DLL", except for a little bit of start-up code in
*       the EXE, and this code in each DLL.  This code is necessary to invoke
*       the C++ constructors for the C++ code in this DLL.
*
*       This entry point should either be specified as the DLL initialization
*       entry point, or else it must be called by the DLL initialization entry
*       point of the DLL with the same arguments that the entry point receives.
*
*******************************************************************************/

#ifdef CRTDLL

/*
 * SPECIAL BUILD MACRO! Note that crtexe.c (and crtexew.c) is linked in with
 * the client's code. It does not go into crtdll.dll! Therefore, it must be
 * built under the _DLL switch (like user code) and CRTDLL must be undefined.
 */
#undef  CRTDLL
#define _DLL

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <stdlib.h>
#define _DECL_DLLMAIN   /* enable prototypes for DllMain and _CRT_INIT */
#include <process.h>
#include <dbgint.h>
#include <rtcapi.h>
#include <sect_attribs.h>

#ifdef _M_IX86

/*
 * The local copy of the Pentium FDIV adjustment flag
 *      and the address of the flag in MSVCRT*.DLL.
 */

extern int _adjust_fdiv;

extern int * _imp___adjust_fdiv;

#endif  /* _M_IX86 */


/*
 * routine in DLL to do initialization (in this case, C++ constructors)
 */

extern void __cdecl _initterm(_PVFV *, _PVFV *);

/*
 * pointers to initialization sections
 */

extern _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */

/*
 * flag set iff _CRTDLL_INIT was called with DLL_PROCESS_ATTACH
 */
static int __proc_attached = 0;


/*
 * Pointers to beginning and end of the table of function pointers manipulated
 * by _onexit()/atexit().  The atexit/_onexit code is shared for both EXE's and
 * DLL's but different behavior is required.  These values are initialized to
 * 0 by default and will be set to point to a malloc-ed memory block to mark
 * this module as an DLL.
 */

extern _PVFV *__onexitbegin;
extern _PVFV *__onexitend;


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
*BOOL WINAPI _CRT_INIT(hDllHandle, dwReason, lpreserved) - C++ DLL
*       initialization.
*BOOL WINAPI _DllMainCRTStartup(hDllHandle, dwReason, lpreserved) - C++ DLL
*       initialization.
*
*Purpose:
*       This is the entry point for DLL's linked with the C/C++ run-time libs.
*       This routine does the C runtime initialization for a DLL linked with
*       MSVCRT.LIB (whose C run-time code is thus in MSVCRT*.DLL.)
*       It will call the user notification routine DllMain on all 4 types of
*       DLL notifications.  The return code from this routine is the return
*       code from the user notification routine.
*
*       On DLL_PROCESS_ATTACH, the C++ constructors for the DLL will be called.
*
*       On DLL_PROCESS_DETACH, the C++ destructors and _onexit/atexit routines
*       will be called.
*
*Entry:
*
*Exit:
*
*******************************************************************************/

BOOL WINAPI _CRT_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        /*
         * If this is a process detach notification, check that there has
         * been a prior (successful) process attachment.
         */
        if ( dwReason == DLL_PROCESS_DETACH ) {
            if ( __proc_attached > 0 )
                __proc_attached--;
            else
                /*
                 * no prior process attach. just return failure.
                 */
                return FALSE;
        }

#ifdef _M_IX86

        /*
         * Set the local copy of the Pentium FDIV adjustment flag
         */

        _adjust_fdiv = * _imp___adjust_fdiv;

#endif  /* _M_IX86 */

        /*
         * do C++ constructors (initializers) specific to this DLL
         */

        if ( dwReason == DLL_PROCESS_ATTACH ) {

            /*
             * create the onexit table.
             */
            if ( (__onexitbegin = (_PVFV *)_malloc_crt(32 * sizeof(_PVFV)))
                 == NULL )
                /*
                 * cannot allocate minimal required
                 * size. generate failure to load DLL
                 */
                return FALSE;

            *(__onexitbegin) = (_PVFV) NULL;

            __onexitend = __onexitbegin;

            /*
             * Run the RTC initialization code for this DLL
             */
#ifdef _RTC
            _RTC_Initialize();
            atexit(_RTC_Terminate);
#endif  /* _RTC */
            /*
             * Invoke C++ constructors
             */
            _initterm(__xc_a,__xc_z);

            /*
             * Increment the process attached flag.
             */
            __proc_attached++;

        }
        else if ( dwReason == DLL_PROCESS_DETACH ) {

            /*
             * Any basic clean-up code that goes here must be
             * duplicated below in _DllMainCRTStartup for the
             * case where the user's DllMain() routine fails on a
             * Process Attach notification. This does not include
             * calling user C++ destructors, etc.
             */

            /*
             * do _onexit/atexit() terminators
             * (if there are any)
             *
             * These terminators MUST be executed in
             * reverse order (LIFO)!
             *
             * NOTE:
             *  This code assumes that __onexitbegin
             *  points to the first valid onexit()
             *  entry and that __onexitend points
             *  past the last valid entry. If
             *  __onexitbegin == __onexitend, the
             *  table is empty and there are no
             *  routines to call.
             */

            if (__onexitbegin) {
                while ( --__onexitend >= __onexitbegin )
                    /*
                     * if current table entry is not
                     * NULL, call thru it.
                     */
                    if ( *__onexitend != NULL )
                        (**__onexitend)();

                /*
                 * free the block holding onexit table to
                 * avoid memory leaks.  Also zero the ptr
                 * variable so that it is clearly cleaned up.
                 */

                _free_crt ( __onexitbegin ) ;

                __onexitbegin = NULL ;
            }
        }

        return TRUE;
}


BOOL WINAPI _DllMainCRTStartup(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        BOOL retcode = TRUE;

        /*
         * If this is a process detach notification, check that there has
         * been a prior process attach notification.
         */
        if ( (dwReason == DLL_PROCESS_DETACH) && (__proc_attached == 0) )
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

#endif  /* CRTDLL */
