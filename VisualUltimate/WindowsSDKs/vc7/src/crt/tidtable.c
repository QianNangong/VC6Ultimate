/***
*tidtable.c - Access thread data table
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the following routines for multi-thread
*       data support:
*
*       _mtinit     = Initialize the mthread data
*       _getptd     = get the pointer to the per-thread data structure for
*                       the current thread
*       _freeptd    = free up a per-thread data structure and its
*                       subordinate structures
*       __threadid  = return thread ID for the current thread
*       __threadhandle = return pseudo-handle for the current thread
*
*******************************************************************************/

#if defined (_MT)

# if defined(_NTSUBSET_)

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <ntstatus.h>
#include <ntos.h>
#include <fsrtl.h>

# endif /* _NTSUBSET_ */

#include <sect_attribs.h>
#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <mtdll.h>
#include <memory.h>
#include <msdos.h>
#include <rterr.h>
#include <stdlib.h>
#include <stddef.h>
#include <dbgint.h>
#include <setlocal.h>
#include <mbstring.h>

#ifdef _MT
extern pthreadmbcinfo __ptmbcinfo;

extern threadlocinfo __initiallocinfo;
extern pthreadlocinfo __ptlocinfo;

void * __cdecl __removelocaleref( pthreadlocinfo);
void __cdecl __addlocaleref( pthreadlocinfo);
void __cdecl __freetlocinfo(pthreadlocinfo);

//
// Define Fiber Local Storage function pointers.
//

PFLS_ALLOC_FUNCTION gpFlsAlloc = NULL;
PFLS_GETVALUE_FUNCTION gpFlsGetValue = NULL;
PFLS_SETVALUE_FUNCTION gpFlsSetValue = NULL;
PFLS_FREE_FUNCTION gpFlsFree = NULL;

#endif  /* _MT */

unsigned long __tlsindex = 0xffffffff;


/****
*_mtinit() - Init multi-thread data bases
*
*Purpose:
*       (1) Call _mtinitlocks to create/open all lock semaphores.
*       (2) Allocate a TLS index to hold pointers to per-thread data
*           structure.
*
*       NOTES:
*       (1) Only to be called ONCE at startup
*       (2) Must be called BEFORE any mthread requests are made
*
*Entry:
*       <NONE>
*Exit:
*       returns FALSE on failure
*
*Uses:
*       <any registers may be modified at init time>
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _mtinit (
        void
        )
{
        _ptiddata ptd;
        HINSTANCE hKernel32;


        /*
         * Initialize the mthread lock data base
         */

        if ( !_mtinitlocks() ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        /*
         * Initialize fiber local storage function pointers.
         */

        hKernel32 = GetModuleHandle("kernel32.dll");
        if (hKernel32 != NULL) {
            gpFlsAlloc = (PFLS_ALLOC_FUNCTION)GetProcAddress(hKernel32,
                                                             "FlsAlloc");

            gpFlsGetValue = (PFLS_GETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                   "FlsGetValue");

            gpFlsSetValue = (PFLS_SETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                   "FlsSetValue");

            gpFlsFree = (PFLS_FREE_FUNCTION)GetProcAddress(hKernel32,
                                                           "FlsFree");
            if (gpFlsGetValue == NULL) {
                gpFlsGetValue = (PFLS_GETVALUE_FUNCTION) GetProcAddress(hKernel32,
                                                                        "TlsGetValue");
            }
        }

        /*
         * Allocate a TLS index to maintain pointers to per-thread data
         */
        if ( (__tlsindex = FLS_ALLOC(&_freefls)) == 0xffffffff ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }


        /*
         * Create a per-thread data structure for this (i.e., the startup)
         * thread.
         */
        if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) == NULL) ||
             !FLS_SETVALUE(__tlsindex, (LPVOID)ptd) )
        {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        /*
         * Initialize the per-thread data
         */

        _initptd(ptd);

        ptd->_tid = GetCurrentThreadId();
        ptd->_thandle = (uintptr_t)(-1);


        return TRUE;
}


/****
*_mtterm() - Clean-up multi-thread data bases
*
*Purpose:
*       (1) Call _mtdeletelocks to free up all lock semaphores.
*       (2) Free up the TLS index used to hold pointers to
*           per-thread data structure.
*
*       NOTES:
*       (1) Only to be called ONCE at termination
*       (2) Must be called AFTER all mthread requests are made
*
*Entry:
*       <NONE>
*Exit:
*       returns
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _mtterm (
        void
        )
{


        /*
         * Free up the TLS index
         *
         * (Set the variable __tlsindex back to the unused state (-1L).)
         */

        if ( __tlsindex != 0xffffffff ) {
            FLS_FREE(__tlsindex);
            __tlsindex = 0xffffffff;
        }

        /*
         * Clean up the mthread lock data base
         */

        _mtdeletelocks();

}




/***
*void _initptd(_ptiddata ptd) - initialize a per-thread data structure
*
*Purpose:
*       This routine handles all of the per-thread initialization
*       which is common to _beginthread, _beginthreadex, _mtinit
*       and _getptd.
*
*Entry:
*       pointer to a per-thread data block
*
*Exit:
*       the common fields in that block are initialized
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _initptd (
        _ptiddata ptd
        )
{
        ptd->_pxcptacttab = (void *)_XcptActTab;
        ptd->_holdrand = 1L;
        // if _ENABLE_PER_THREAD_LOCALE_NEW is set, then set _ownlocale flag for
        // this thread.
        if (__globallocalestatus & 0x4)
            ptd->_ownlocale = 0x2;
        // Initialize _setloc_data. These are the only valuse that need to be
        // initialized.
        ptd->_setloc_data._cachein[0]='C';
        ptd->_setloc_data._cacheout[0]='C';
        // We need to make sure that ptd->ptlocinfo in never NULL, this saves us
        // perf counts when UPDATING locale.
        _mlock(_SETLOCALE_LOCK);
        __try {
            ptd->ptlocinfo = __ptlocinfo;
            __addlocaleref(__ptlocinfo);
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }
}



/***
*_ptiddata _getptd(void) - get per-thread data structure for the current thread
*
*Purpose:
*
*Entry:
*       unsigned long tid
*
*Exit:
*       success = pointer to _tiddata structure for the thread
*       failure = fatal runtime exit
*
*Exceptions:
*
*******************************************************************************/

_ptiddata __cdecl _getptd (
        void
        )
{
        _ptiddata ptd;
        DWORD   TL_LastError;


        TL_LastError = GetLastError();
        if ( (ptd = FLS_GETVALUE(__tlsindex)) == NULL ) {
            /*
             * no per-thread data structure for this thread. try to create
             * one.
             */
            if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) != NULL) &&
                FLS_SETVALUE(__tlsindex, (LPVOID)ptd) ) {

                /*
                 * Initialize of per-thread data
                 */

                _initptd(ptd);

                ptd->_tid = GetCurrentThreadId();
                ptd->_thandle = (uintptr_t)(-1);
            }
            else
                _amsg_exit(_RT_THREAD); /* write message and die */
            }

        SetLastError(TL_LastError);


        return(ptd);
}


/***
*void WINAPI _freefls(void *) - free up a per-fiber data structure
*
*Purpose:
*       Called from _freeptd, as a callback from deleting a fiber, and
*       from deleting an FLS index. This routine frees up the per-fiber
*       buffer associated with a fiber that is going away. The tiddata
*       structure itself is freed, but not until its subordinate buffers
*       are freed.
*
*Entry:
*       pointer to a per-fiber data block (malloc-ed memory)
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void
WINAPI
_freefls (
    void *data
    )

{

    _ptiddata ptd;
    pthreadlocinfo ptloci;
    pthreadmbcinfo ptmbci;

    /*
     * Free up the _tiddata structure & its malloc-ed buffers.
     */

    ptd = data;
    if (ptd != NULL) {
        if(ptd->_errmsg)
            _free_crt((void *)ptd->_errmsg);

        if(ptd->_namebuf0)
            _free_crt((void *)ptd->_namebuf0);

        if(ptd->_namebuf1)
            _free_crt((void *)ptd->_namebuf1);

        if(ptd->_asctimebuf)
            _free_crt((void *)ptd->_asctimebuf);

        if(ptd->_gmtimebuf)
            _free_crt((void *)ptd->_gmtimebuf);

        if(ptd->_cvtbuf)
            _free_crt((void *)ptd->_cvtbuf);

        if (ptd->_pxcptacttab != _XcptActTab)
            _free_crt((void *)ptd->_pxcptacttab);

        _mlock(_MB_CP_LOCK);
        __try {
            if ( ((ptmbci = ptd->ptmbcinfo) != NULL) &&
                 (--(ptmbci->refcount) == 0) &&
                 (ptmbci != __ptmbcinfo) )
                _free_crt(ptmbci);
        }
        __finally {
            _munlock(_MB_CP_LOCK);
        }

        _mlock(_SETLOCALE_LOCK);

        __try {
            if ( (ptloci = ptd->ptlocinfo) != NULL )
            {
                __removelocaleref(ptloci);
                if ( (ptloci != __ptlocinfo) &&
                     (ptloci != &__initiallocinfo) &&
                     (ptloci->refcount == 0) )
                    __freetlocinfo(ptloci);
            }
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }

        _free_crt((void *)ptd);
    }
    return;
}

/***
*void _freeptd(_ptiddata) - free up a per-thread data structure
*
*Purpose:
*       Called from _endthread and from a DLL thread detach handler,
*       this routine frees up the per-thread buffer associated with a
*       thread that is going away.  The tiddata structure itself is
*       freed, but not until its subordinate buffers are freed.
*
*Entry:
*       pointer to a per-thread data block (malloc-ed memory)
*       If NULL, the pointer for the current thread is fetched.
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _freeptd (
        _ptiddata ptd
        )
{


        /*
         * Do nothing unless per-thread data has been allocated for this module!
         */

        if ( __tlsindex != 0xFFFFFFFF ) {

            /*
             * if parameter "ptd" is NULL, get the per-thread data pointer
             * Must NOT call _getptd because it will allocate one if none exists!
             */

            if ( ! ptd )
                ptd = FLS_GETVALUE(__tlsindex );

            _freefls(ptd);

            /*
             * Zero out the one pointer to the per-thread data block
             */

            FLS_SETVALUE(__tlsindex, (LPVOID)0);
        }


}



/***
*__threadid()     - Returns current thread ID
*__threadhandle() - Returns "pseudo-handle" for current thread
*
*Purpose:
*       The two function are simply do-nothing wrappers for the corresponding
*       Win32 APIs (GetCurrentThreadId and GetCurrentThread, respectively).
*
*Entry:
*       void
*
*Exit:
*       thread ID value
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP unsigned long __cdecl __threadid (
        void
        )
{
        return( GetCurrentThreadId() );
}

_CRTIMP uintptr_t __cdecl __threadhandle(
        void
        )
{
        return( (uintptr_t)GetCurrentThread() );
}

#endif  /* defined (_MT) */
