/***
*wsetlocal.c - Contains the setlocale function (wchar_t version)
*
*       Copyright (c) 1993-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the _wsetlocale() function.
*
*******************************************************************************/


#include <wchar.h>
#include <stdlib.h>
#include <setlocal.h>
#include <locale.h>
#include <dbgint.h>
#include <mtdll.h>

#define MAXSIZE ((MAX_LC_LEN+1) * (LC_MAX-LC_MIN+1) + CATNAMES_LEN)

wchar_t * __cdecl _wsetlocale (
        int _category,
        const wchar_t *_wlocale
        )
{
        size_t size;
        char *inlocale = NULL;
        char *outlocale;
        pthreadlocinfo ptloci;
        int *refcount = NULL;
        wchar_t *outwlocale = NULL;

        /* convert WCS string into ASCII string */

        if (_wlocale)
        {
            size = wcslen(_wlocale) + 1;
            if (NULL == (inlocale = (char *)_malloc_crt(size * sizeof(char))))
                return NULL;
            if (-1 == wcstombs(inlocale, _wlocale, size))
            {
                _free_crt (inlocale);
                return NULL;
            }
        }

        /* set the locale and get ASCII return string */

        outlocale = setlocale(_category, inlocale);
        _free_crt (inlocale);
        if (NULL == outlocale)
            return NULL;

        /* get space for WCS return value, first call only */

        if (-1 == (size = mbstowcs(NULL, outlocale, 0)))
            return NULL;

        size++;
        refcount = (int *)_malloc_crt(size * sizeof(wchar_t) + sizeof(int));
        if (!refcount)
            return NULL;
        outwlocale = (wchar_t *)&refcount[1];

        /* convert return value to WCS */

        if (-1 == mbstowcs(outwlocale, outlocale, size))
        {
            _free_crt(refcount);
            return NULL;
        }

#ifndef _MT
        ptloci  = __ptlocinfo;
#else  /* _MT */
        _mlock(_SETLOCALE_LOCK);
        ptloci = _getptd()->ptlocinfo;
        __try {
#endif  /* _MT */
            if (ptloci->lc_category[_category].wrefcount != NULL &&
                InterlockedDecrement(ptloci->lc_category[_category].wrefcount) == 0) {
                _free_crt(ptloci->lc_category[_category].wrefcount);
            }
            // note here that we are doning some trick here. The reason for this trick is
            // we are adding this wlocale to whole structure and thus the starting refcount
            // will be the refcount of the pthreadlocinfo struct or else we might end up
            // in leaking this memory block.
            *refcount = ptloci->refcount;
            ptloci->lc_category[_category].wrefcount = refcount;
            ptloci->lc_category[_category].wlocale = outwlocale;
#ifdef _MT
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }
#endif  /* _MT */

        return outwlocale;
}

