/***
*wsetlocal.c - Contains the setlocale function (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
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
        static wchar_t *outwlocale = NULL;

        /* convert WCS string into ASCII string */

        if (_wlocale)
        {
            size = (size_t)(wcstombs(NULL,_wlocale,INT_MAX) + 1);
            if (size==0 || (inlocale = (char *)_malloc_crt(size * sizeof(char))) == NULL)
                return NULL;
            if (NULL == (inlocale = (char *)_malloc_crt(size * sizeof(char))))
                return NULL;
            if (-1 == wcstombs(inlocale, _wlocale, size))
            {
                _free_crt (inlocale);
                return NULL;
            }
        }

#ifdef _MT
        _mlock(_SETLOCALE_LOCK);

        __try {
#endif  /* _MT */

        /* set the locale and get ASCII return string */

        outlocale = setlocale(_category, inlocale);
        _free_crt (inlocale);
        if (NULL == outlocale)
            return NULL;

        /* get space for WCS return value, first call only */

        if (!outwlocale)
        {
            outwlocale = (wchar_t *)_malloc_crt(MAXSIZE * sizeof(wchar_t));
            if (!outwlocale)
                return NULL;
        }

        if (-1 == (size = mbstowcs(NULL, outlocale, 0)))
            return NULL;

        size++;

        if (MAXSIZE < size)
            return NULL;

        /* convert return value to WCS */

        if (-1 == mbstowcs(outwlocale, outlocale, size))
        {
            _free_crt(outwlocale);
            outwlocale = NULL;
            return NULL;
        }

#ifdef _MT
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }
#endif  /* _MT */

        return outwlocale;
}

