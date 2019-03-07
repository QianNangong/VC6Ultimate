/***
*initctyp.c - contains __init_ctype
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the locale-category initialization function: __init_ctype().
*
*       Each initialization function sets up locale-specific information
*       for their category, for use by functions which are affected by
*       their locale category.
*
*       *** For internal use by setlocale() only ***
*
*******************************************************************************/

#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <setlocal.h>
#include <ctype.h>
#include <malloc.h>
#include <limits.h>
#include <awint.h>
#include <dbgint.h>
#ifdef _MT
#include <mtdll.h>
#endif  /* _MT */

#define _CTABSIZE   257     /* size of ctype tables */

#ifdef _MT
/*
 * Keep track of how many threads are using a instance of the ctype info. Only
 * used for non-'C' locales.
 */
int *__ctype1_refcount;
#endif  /* _MT */

unsigned short  *__ctype1;  /* keep around until next time */

/***
*int __init_ctype() - initialization for LC_CTYPE locale category.
*
*Purpose:
*       In non-C locales, preread ctype tables for chars and wide-chars.
*       Old tables are freed when new tables are fully established, else
*       the old tables remain intact (as if original state unaltered).
*       The leadbyte table is implemented as the high bit in ctype1.
*
*       In the C locale, ctype tables are freed, and pointers point to
*       the static ctype table.
*
*       Tables contain 257 entries: -1 to 256.
*       Table pointers point to entry 0 (to allow index -1).
*
*Entry:
*       None.
*
*Exit:
*       0 success
*       1 fail
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __init_ctype (
        void
        )
{
#ifdef _MT
        int *refcount;
#endif  /* _MT */
        /* non-C locale table for char's    */
        unsigned short *newctype1;          /* temp new table */

        /* non-C locale table for wchar_t's */

        unsigned char *cbuffer = NULL;      /* char working buffer */

        int i;                              /* general purpose counter */
        unsigned char *cp;                  /* char pointer */
        CPINFO lpCPInfo;                    /* struct for use with GetCPInfo */

        /* allocate and set up buffers before destroying old ones */
        /* codepage will be restored by setlocale if error */

        if (__lc_handle[LC_CTYPE] != _CLOCALEHANDLE)
        {
            if (__lc_codepage == 0)
            { /* code page was not specified */
                if ( __getlocaleinfo( LC_INT_TYPE,
                                      MAKELCID(__lc_id[LC_CTYPE].wLanguage, SORT_DEFAULT),
                                      LOCALE_IDEFAULTANSICODEPAGE,
                                      (char **)&__lc_codepage ) )
                    goto error_cleanup;
            }

#ifdef _MT
            /* allocate a new (thread) reference counter */
            refcount = (int *)_malloc_crt(sizeof(int));
#endif  /* _MT */

            /* allocate new buffers for tables */
            newctype1 = (unsigned short *)
                _malloc_crt((_COFFSET+_CTABSIZE) * sizeof(unsigned short));
            cbuffer = (unsigned char *)
                _malloc_crt (_CTABSIZE * sizeof(char));

#ifdef _MT
            if (!refcount || !newctype1 || !cbuffer )
#else  /* _MT */
            if (!newctype1 || !cbuffer )
#endif  /* _MT */
                goto error_cleanup;

#ifdef _MT
            *refcount = 0;
#endif  /* _MT */

            /* construct string composed of first 256 chars in sequence */
            for (cp=cbuffer, i=0; i<_CTABSIZE-1; i++)
                *cp++ = (unsigned char)i;

            if (GetCPInfo( __lc_codepage, &lpCPInfo) == FALSE)
                goto error_cleanup;

            if (lpCPInfo.MaxCharSize > MB_LEN_MAX)
                goto error_cleanup;

            __mb_cur_max = (unsigned short) lpCPInfo.MaxCharSize;

            /* zero out leadbytes so GetStringType doesn't interpret as multi-byte chars */
            if (__mb_cur_max > 1)
            {
                for (cp = (unsigned char *)lpCPInfo.LeadByte; cp[0] && cp[1]; cp += 2)
                {
                    for (i = cp[0]; i <= cp[1]; i++)
                        cbuffer[i] = 0;
                }
            }

            /* convert to newctype1 table - ignore invalid char errors */
            if ( __crtGetStringTypeA( CT_CTYPE1,
                                      cbuffer,
                                      _CTABSIZE-1,
                                      newctype1+_COFFSET+1,
                                      0,
                                      0,
                                      FALSE ) == FALSE )
                goto error_cleanup;
            newctype1[_COFFSET] = 0; /* entry for EOF */

            /* ignore DefaultChar */

            /* mark lead-byte entries in newctype1 table */
            if (__mb_cur_max > 1)
            {
                for (cp = (unsigned char *)lpCPInfo.LeadByte; cp[0] && cp[1]; cp += 2)
                {
                    for (i = cp[0]; i <= cp[1]; i++)
                        newctype1[_COFFSET+i+1] = _LEADBYTE;
                }
            }
            /* copy last-1 _COFFSET unsigned short to front
             * note -1, we don't really want to copy 0xff
             */
            memcpy(newctype1,newctype1+_CTABSIZE-1,_COFFSET*sizeof(unsigned short));

            /* set pointers to point to entry 0 of tables */
            _pctype = newctype1 + 1+_COFFSET;

#ifdef _MT
            __ctype1_refcount = refcount;
#endif  /* _MT */

            /* free old tables */
#ifndef _MT
            if (__ctype1)
                _free_crt (__ctype1-_COFFSET);
#endif  /* _MT */
            __ctype1 = newctype1+_COFFSET;

            /* cleanup and return success */
            _free_crt (cbuffer);
            return 0;

error_cleanup:
#ifdef _MT
            _free_crt (refcount);
#endif  /* _MT */
            _free_crt (newctype1);
            _free_crt (cbuffer);
            return 1;

        } else {

            /* set pointers to static C-locale table */
            _pctype = __newctype + 128;

#ifndef _MT
            /* free dynamic locale-specific tables */
            _free_crt (__ctype1-_COFFSET);
#endif  /* _MT */

#ifdef _MT
            __ctype1_refcount = NULL;
#endif  /* _MT */

            __ctype1 = NULL;

            return 0;
        }
}

/* Define a number of functions which exist so, under _STATIC_CPPLIB, the
 * static multithread C++ Library libcpmt.lib can access data found in the
 * main CRT DLL without using __declspec(dllimport).
 */

_CRTIMP int __cdecl ___mb_cur_max_func(void)
{
        return __mb_cur_max;
}


_CRTIMP UINT __cdecl ___lc_codepage_func(void)
{
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        return ptloci->lc_codepage;
#else  /* _MT */
        return __lc_codepage;
#endif  /* _MT */
}


_CRTIMP UINT __cdecl ___lc_collate_cp_func(void)
{
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        return ptloci->lc_collate_cp;
#else  /* _MT */
        return __lc_collate_cp;
#endif  /* _MT */
}


_CRTIMP LCID* __cdecl ___lc_handle_func(void)
{
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        return ptloci->lc_handle;
#else  /* _MT */
        return __lc_handle;
#endif  /* _MT */
}
