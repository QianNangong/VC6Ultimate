/***
*initctyp.c - contains __init_ctype
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
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
#include <mtdll.h>

#define _CTABSIZE   257     /* size of ctype tables */

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
        pthreadlocinfo ploci
        )
{
    int *refcount;
    /* non-C locale table for char's    */
    unsigned short *newctype1;          /* temp new table */

    /* non-C locale table for wchar_t's */

    unsigned char *cbuffer = NULL;      /* char working buffer */

    int i;                              /* general purpose counter */
    unsigned char *cp;                  /* char pointer */
    CPINFO lpCPInfo;                    /* struct for use with GetCPInfo */
    int mb_cur_max;

    /* allocate and set up buffers before destroying old ones */
    /* codepage will be restored by setlocale if error */

    if (ploci->lc_handle[LC_CTYPE] != _CLOCALEHANDLE)
    {
        if (ploci->lc_codepage == 0)
        { /* code page was not specified */
            if ( __getlocaleinfo( LC_INT_TYPE,
                                  MAKELCID(ploci->lc_id[LC_CTYPE].wLanguage, SORT_DEFAULT),
                                  LOCALE_IDEFAULTANSICODEPAGE,
                                  (char **)&ploci->lc_codepage ) )
                goto error_cleanup;
        }

        /* allocate a new (thread) reference counter */
        refcount = (int *)_malloc_crt(sizeof(int));

        /* allocate new buffers for tables */
        newctype1 = (unsigned short *)
            _malloc_crt(_CTABSIZE * sizeof(unsigned short));
        cbuffer = (unsigned char *)
            _malloc_crt (_CTABSIZE * sizeof(char));

        if (!refcount || !newctype1 || !cbuffer )
            goto error_cleanup;

        *refcount = 0;

        /* construct string composed of first 256 chars in sequence */
        for (cp=cbuffer, i=0; i<_CTABSIZE-1; i++)
            *cp++ = (unsigned char)i;

        if (GetCPInfo( ploci->lc_codepage, &lpCPInfo) == FALSE)
            goto error_cleanup;

        if (lpCPInfo.MaxCharSize > MB_LEN_MAX)
            goto error_cleanup;

        mb_cur_max = (unsigned short) lpCPInfo.MaxCharSize;

        /* zero out leadbytes so GetStringType doesn't interpret as multi-byte chars */
        if (mb_cur_max > 1)
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
                                  newctype1+1,
                                  ploci->lc_codepage,
                                  0,
                                  FALSE ) == FALSE )
            goto error_cleanup;
        *newctype1 = 0; /* entry for EOF */

        /* ignore DefaultChar */

        /* mark lead-byte entries in newctype1 table */
        if (mb_cur_max > 1)
        {
            for (cp = (unsigned char *)lpCPInfo.LeadByte; cp[0] && cp[1]; cp += 2)
            {
                for (i = cp[0]; i <= cp[1]; i++)
                    newctype1[i+1] = _LEADBYTE;
            }
        }

        /* free old tables */
        if ((ploci->ctype1_refcount) &&
            (InterlockedDecrement(ploci->ctype1_refcount) == 0))
        {
            _ASSERT(0);
            _free_crt(ploci->ctype1);
            _free_crt(ploci->ctype1_refcount);
        }
        (*refcount) = 1;
        ploci->ctype1_refcount = refcount;
        /* set pointers to point to entry 0 of tables */
        ploci->pctype = newctype1 + 1;
        ploci->ctype1 = newctype1;
        ploci->mb_cur_max = mb_cur_max;

        /* cleanup and return success */
        _free_crt (cbuffer);
#ifdef _MT
    _ASSERT(ploci->lc_time_curr == _getptd()->ptlocinfo->lc_time_curr);
#endif  /* _MT */
        return 0;

error_cleanup:
        _free_crt (refcount);
        _free_crt (newctype1);
        _free_crt (cbuffer);
        return 1;

    } else {

        if ( (ploci->ctype1_refcount != NULL)&&
             (InterlockedDecrement(ploci->ctype1_refcount) == 0))
        {
            _ASSERTE(ploci->ctype1_refcount > 0);
        }
        ploci->ctype1_refcount = NULL;
        ploci->ctype1 = NULL;
        ploci->pctype = _ctype + 1;


        return 0;
    }
}

/* Define a number of functions which exist so, under _STATIC_CPPLIB, the
 * static multithread C++ Library libcpmt.lib can access data found in the
 * main CRT DLL without using __declspec(dllimport).
 */

_CRTIMP int __cdecl ___mb_cur_max_func(void)
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return ptloci->mb_cur_max;
#else  /* _MT */
    return __ptlocinfo->mb_cur_max;
#endif  /* _MT */
}


_CRTIMP UINT __cdecl ___lc_codepage_func(void)
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return ptloci->lc_codepage;
#else  /* _MT */
    return __ptlocinfo->lc_codepage;
#endif  /* _MT */
}


_CRTIMP UINT __cdecl ___lc_collate_cp_func(void)
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return ptloci->lc_collate_cp;
#else  /* _MT */
    return __lc_collate_cp;
#endif  /* _MT */
}


_CRTIMP LCID* __cdecl ___lc_handle_func(void)
{
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

    return ptloci->lc_handle;
#else  /* _MT */
    return __lc_handle;
#endif  /* _MT */
}
