/***
*crtexe.c - Initialization for console EXE using CRT DLL
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for apps linking to the CRT DLL.
*       It calls the user's main routine [w]main() or [w]WinMain after
*       performing C Run-Time Library initialization.
*
*       With ifdefs, this source file also provides the source code for:
*       wcrtexe.c   the startup routine for console apps with wide chars
*       crtexew.c   the startup routine for Windows apps
*       wcrtexew.c  the startup routine for Windows apps with wide chars
*
*******************************************************************************/

#ifdef CRTDLL

/*
 * SPECIAL BUILD MACROS! Note that crtexe.c (and crtexew.c) is linked in with
 * the client's code. It does not go into crtdll.dll! Therefore, it must be
 * built under the _DLL switch (like user code) and CRTDLL must be undefined.
 * The symbol SPECIAL_CRTEXE is turned on to suppress the normal CRT DLL
 * definition of _fmode and _commode using __declspec(dllexport).  Otherwise
 * this module would not be able to refer to both the local and DLL versions
 * of these two variables.
 */

#undef  CRTDLL
#define _DLL

#define SPECIAL_CRTEXE

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <process.h>
#include <math.h>
#include <rterr.h>
#include <stdlib.h>
#include <tchar.h>
#include <rtcapi.h>
#include <sect_attribs.h>


/*
 * wWinMain is not yet defined in winbase.h. When it is, this should be
 * removed.
 */

int
WINAPI
wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nShowCmd
    );

#define SPACECHAR   _T(' ')
#define DQUOTECHAR  _T('\"')

#ifdef _M_IX86

/*
 * The local copy of the Pentium FDIV adjustment flag
 * and the address of the flag in MSVCRT*.DLL.
 */

extern int _adjust_fdiv;

extern int * _imp___adjust_fdiv;

#endif  /* _M_IX86 */


/* default floating point precision - X86 only! */

#ifdef _M_IX86
extern void _setdefaultprecision();
#endif  /* _M_IX86 */


/*
 * Declare function used to install a user-supplied _matherr routine.
 */
_CRTIMP void __setusermatherr( int (__cdecl *)(struct _exception *) );


/*
 * Declare the names of the exports corresponding to _fmode and _commode
 */

#ifdef _M_IX86

#define _IMP___FMODE    (__p__fmode())
#define _IMP___COMMODE  (__p__commode())

#else  /* _M_IX86 */

/* assumed to be MIPS or Alpha */

#define _IMP___FMODE    __imp__fmode
#define _IMP___COMMODE  __imp__commode

#endif  /* _M_IX86 */


#if !defined (_M_IX86)
extern int * _IMP___FMODE;      /* exported from the CRT DLL */
extern int * _IMP___COMMODE;    /* these names are implementation-specific */
#endif  /* !defined (_M_IX86) */

extern int _fmode;          /* must match the definition in <stdlib.h> */
extern int _commode;        /* must match the definition in <internal.h> */

extern int _dowildcard;     /* passed to __getmainargs() */

/*
 * Declare/define communal that serves as indicator the default matherr
 * routine is being used.
 */
int __defaultmatherr;

/*
 * routine in DLL to do initialization (in this case, C++ constructors)
 */
extern void __cdecl _initterm(_PVFV *, _PVFV *);

/*
 * routine to check if this is a managed application
 */
static int __cdecl check_managed_app(void);

/*
 * pointers to initialization sections
 */
extern _CRTALLOC(".CRT$XIA") _PVFV __xi_a[];
extern _CRTALLOC(".CRT$XIZ") _PVFV __xi_z[];    /* C initializers */
extern _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */

/*
 * Pointers to beginning and end of the table of function pointers manipulated
 * by _onexit()/atexit().  The atexit/_onexit code is shared for both EXE's and
 * DLL's but different behavior is required.  These values are set to -1 to
 * mark this module as an EXE.
 */

extern _PVFV *__onexitbegin;
extern _PVFV *__onexitend;


/***
*mainCRTStartup(void)
*wmainCRTStartup(void)
*WinMainCRTStartup(void)
*wWinMainCRTStartup(void)
*
*Purpose:
*       These routines do the C runtime initialization, call the appropriate
*       user entry function, and handle termination cleanup.  For a managed
*       app, they then return the exit code back to the calling routine, which
*       is the managed startup code.  For an unmanaged app, they call exit and
*       never return.
*
*       Function:               User entry called:
*       mainCRTStartup          main
*       wmainCRTStartup         wmain
*       WinMainCRTStartup       WinMain
*       wWinMainCRTStartup      wWinMain
*
*Entry:
*
*Exit:
*       Managed app: return value from main() et al, or the exception code if
*                 execution was terminated by the __except guarding the call
*                 to main().
*       Unmanaged app: never return.
*
*******************************************************************************/

#ifdef _WINMAIN_

#ifdef WPRFLAG
int wWinMainCRTStartup(
#else  /* WPRFLAG */
int WinMainCRTStartup(
#endif  /* WPRFLAG */

#else  /* _WINMAIN_ */

#ifdef WPRFLAG
int wmainCRTStartup(
#else  /* WPRFLAG */
int mainCRTStartup(
#endif  /* WPRFLAG */

#endif  /* _WINMAIN_ */
        void
        )
{
        int argc;   /* three standard arguments to main */
        _TSCHAR **argv;
        _TSCHAR **envp;

        int argret;
        int mainret;
        int managedapp;

#ifdef _WINMAIN_
        _TUCHAR *lpszCommandLine;
        STARTUPINFO StartupInfo;
#endif  /* _WINMAIN_ */

        _startupinfo    startinfo;

        /*
         * Determine if this is a managed application
         */
        managedapp = check_managed_app();

        /*
         * Guard the initialization code and the call to user's main, or
         * WinMain, function in a __try/__except statement.
         */

        __try {
            /*
             * Set __app_type properly
             */
#ifdef _WINMAIN_
            __set_app_type(_GUI_APP);
#else  /* _WINMAIN_ */
            __set_app_type(_CONSOLE_APP);
#endif  /* _WINMAIN_ */

            /*
             * Mark this module as an EXE file so that atexit/_onexit
             * will do the right thing when called, including for C++
             * d-tors.
             */
            __onexitbegin = __onexitend = (_PVFV *)(-1);

            /*
             * Propogate the _fmode and _commode variables to the DLL
             */
            *_IMP___FMODE = _fmode;
            *_IMP___COMMODE = _commode;

#ifdef _M_IX86
            /*
             * Set the local copy of the Pentium FDIV adjustment flag
             */

            _adjust_fdiv = * _imp___adjust_fdiv;
#endif  /* _M_IX86 */

            /*
             * Run the RTC initialization code for this DLL
             */
#ifdef _RTC
            _RTC_Initialize();
#endif  /* _RTC */

            /*
             * Call _setargv(), which will trigger a call to __setargv() if
             * SETARGV.OBJ is linked with the EXE.  If SETARGV.OBJ is not
             * linked with the EXE, a dummy _setargv() will be called.
             */
#ifdef WPRFLAG
            _wsetargv();
#else  /* WPRFLAG */
            _setargv();
#endif  /* WPRFLAG */

            /*
             * If the user has supplied a _matherr routine then set
             * __pusermatherr to point to it.
             */
            if ( !__defaultmatherr )
                __setusermatherr(_matherr);

#ifdef _M_IX86
            _setdefaultprecision();
#endif  /* _M_IX86 */

            /*
             * Do runtime startup initializers.
             *
             * Note: the only possible entry we'll be executing here is for
             * __lconv_init, pulled in from charmax.obj only if the EXE was
             * compiled with -J.  All other .CRT$XI* initializers are only
             * run as part of the CRT itself, and so for the CRT DLL model
             * are not found in the EXE.  For that reason, we call _initterm,
             * not _initterm_e, because __lconv_init will never return failure,
             * and _initterm_e is not exported from the CRT DLL.
             *
             * Note further that, when using the CRT DLL, executing the
             * .CRT$XI* initializers is only done for an EXE, not for a DLL
             * using the CRT DLL.  That is to make sure the -J setting for
             * the EXE is not overriden by that of any DLL.
             */
            _initterm( __xi_a, __xi_z );

#ifdef _RTC
            atexit(_RTC_Terminate);
#endif  /* _RTC */

            /*
             * Get the arguments for the call to main. Note this must be
             * done explicitly, rather than as part of the dll's
             * initialization, to implement optional expansion of wild
             * card chars in filename args
             */

#ifdef WPRFLAG
            startinfo.newmode = _newmode;

            argret = __wgetmainargs(&argc, &argv, &envp,
                                    _dowildcard, &startinfo);
#else  /* WPRFLAG */
            startinfo.newmode = _newmode;

            argret = __getmainargs(&argc, &argv, &envp,
                                   _dowildcard, &startinfo);
#endif  /* WPRFLAG */
            if (argret < 0)
                _amsg_exit(_RT_SPACEARG);


            /*
             * do C++ constructors (initializers) specific to this EXE
             */
            _initterm( __xc_a, __xc_z );

#ifdef _WINMAIN_
            /*
             * Skip past program name (first token in command line).
             * Check for and handle quoted program name.
             */
#ifdef WPRFLAG
            /* OS may not support "W" flavors */
            if (_wcmdln == NULL)
                return 255;
            lpszCommandLine = (wchar_t *)_wcmdln;
#else  /* WPRFLAG */
            lpszCommandLine = (unsigned char *)_acmdln;
#endif  /* WPRFLAG */

            if ( *lpszCommandLine == DQUOTECHAR ) {
                /*
                 * Scan, and skip over, subsequent characters until
                 * another double-quote or a null is encountered.
                 */
                while ( *++lpszCommandLine && (*lpszCommandLine
                        != DQUOTECHAR) );
                /*
                 * If we stopped on a double-quote (usual case), skip
                 * over it.
                 */
                if ( *lpszCommandLine == DQUOTECHAR )
                    lpszCommandLine++;
            }
            else {
                while (*lpszCommandLine > SPACECHAR)
                    lpszCommandLine++;
            }

            /*
             * Skip past any white space preceeding the second token.
             */
            while (*lpszCommandLine && (*lpszCommandLine <= SPACECHAR)) {
                lpszCommandLine++;
            }

            StartupInfo.dwFlags = 0;
            GetStartupInfo( &StartupInfo );

#ifdef WPRFLAG
            mainret = wWinMain(
#else  /* WPRFLAG */
            mainret = WinMain(
#endif  /* WPRFLAG */
                       GetModuleHandleA(NULL),
                       NULL,
                       lpszCommandLine,
                       StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                        ? StartupInfo.wShowWindow
                        : SW_SHOWDEFAULT
                      );
#else  /* _WINMAIN_ */

#ifdef WPRFLAG
            __winitenv = envp;
            mainret = wmain(argc, argv, envp);
#else  /* WPRFLAG */
            __initenv = envp;
            mainret = main(argc, argv, envp);
#endif  /* WPRFLAG */

#endif  /* _WINMAIN_ */

            if ( !managedapp )
                exit(mainret);

            _cexit();

        }
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
        {
            /*
             * Should never reach here
             */

            mainret = GetExceptionCode();

            if ( !managedapp )
                _exit(mainret);

            _c_exit();

        } /* end of try - except */

        return mainret;
}

/***
*check_managed_app() - Check for a managed executable
*
*Purpose:
*       Determine if the EXE the startup code is linked into is a managed app
*       by looking for the COM Runtime Descriptor in the Image Data Directory
*       of the PE or PE+ header.
*
*Entry:
*       None
*
*Exit:
*       1 if managed app, 0 if not.
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl check_managed_app (
        void
        )
{
        PIMAGE_DOS_HEADER pDOSHeader;
        PIMAGE_NT_HEADERS pPEHeader;
        PIMAGE_OPTIONAL_HEADER32 pNTHeader32;
        PIMAGE_OPTIONAL_HEADER64 pNTHeader64;

        pDOSHeader = (PIMAGE_DOS_HEADER)GetModuleHandleA(NULL);
        if ( pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE )
            return 0;

        pPEHeader = (PIMAGE_NT_HEADERS)((char *)pDOSHeader +
                                        pDOSHeader->e_lfanew);
        if ( pPEHeader->Signature != IMAGE_NT_SIGNATURE )
            return 0;

        pNTHeader32 = (PIMAGE_OPTIONAL_HEADER32)&pPEHeader->OptionalHeader;
        switch ( pNTHeader32->Magic ) {
        case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
            /* PE header */
            if ( pNTHeader32->NumberOfRvaAndSizes <=
                    IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR )
                return 0;
            return !! pNTHeader32 ->
                      DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR] .
                      VirtualAddress;
        case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
            /* PE+ header */
            pNTHeader64 = (PIMAGE_OPTIONAL_HEADER64)pNTHeader32;
            if ( pNTHeader64->NumberOfRvaAndSizes <=
                    IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR )
                return 0;
            return !! pNTHeader64 ->
                      DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR] .
                      VirtualAddress;
        }

        /* Not PE or PE+, so not managed */
        return 0;
}


#endif  /* CRTDLL */
