/***
*mbstowcs.c - Convert multibyte char string to wide char string.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a multibyte char string into the equivalent wide char string.
*
*******************************************************************************/


#include <internal.h>
#include <locale.h>
#include <setlocal.h>
#include <errno.h>
#include <cruntime.h>
#include <stdlib.h>
#include <string.h>
#include <mtdll.h>
#include <dbgint.h>
#include <stdio.h>

/***
*size_t mbstowcs() - Convert multibyte char string to wide char string.
*
*Purpose:
*       Convert a multi-byte char string into the equivalent wide char string,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*Entry:
*       wchar_t *pwcs = pointer to destination wide character string buffer
*       const char *s = pointer to source multibyte character string
*       size_t      n = maximum number of wide characters to store
*
*Exit:
*       If s != NULL, returns:  number of words modified (<=n)
*               (size_t)-1 (if invalid mbcs)
*
*Exceptions:
*       Returns (size_t)-1 if s is NULL or invalid mbcs character encountered
*
*******************************************************************************/

size_t __cdecl mbstowcs
(
        wchar_t  *pwcs,
        const char *s,
        size_t n
        )
{
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        return __mbstowcs_mt(ptloci, pwcs, s, n);
}

size_t __cdecl __mbstowcs_mt (
        pthreadlocinfo ptloci,
        wchar_t  *pwcs,
        const char *s,
        size_t n
        )
{
#endif  /* _MT */
        size_t count = 0;

        if (pwcs && n == 0)
            /* dest string exists, but 0 bytes converted */
            return (size_t) 0;

        _ASSERTE(s != NULL);

#ifdef _WIN64
        /* n must fit into an int for MultiByteToWideChar */
        if ( n > INT_MAX )
            return (size_t)-1;
#endif  /* _WIN64 */


        /* if destination string exists, fill it in */
        if (pwcs)
        {
#ifdef _MT
            if (ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE)
#else  /* _MT */
            if (__lc_handle[LC_CTYPE] == _CLOCALEHANDLE)
#endif  /* _MT */
            {
                /* C locale: easy and fast */
                while (count < n)
                {
                    *pwcs = (wchar_t) ((unsigned char)s[count]);
                    if (!s[count])
                        return count;
                    count++;
                    pwcs++;
                }
                return count;

            } else {
                int bytecnt, charcnt;
                unsigned char *p;

                /* Assume that the buffer is large enough */
#ifdef _MT
                if ( (count = MultiByteToWideChar( ptloci->lc_codepage,
#else  /* _MT */
                if ( (count = MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
                                                   MB_PRECOMPOSED |
                                                    MB_ERR_INVALID_CHARS,
                                                   s,
                                                   -1,
                                                   pwcs,
                                                   (int)n )) != 0 )
                    return count - 1; /* don't count NUL */

                if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                {
                    errno = EILSEQ;
                    return (size_t)-1;
                }

                /* User-supplied buffer not large enough. */

                /* How many bytes are in n characters of the string? */
                charcnt = (int)n;
                for (p = (unsigned char *)s; (charcnt-- && *p); p++)
                {
#ifdef _MT
                    if (__isleadbyte_mt(ptloci, *p))
#else  /* _MT */
                    if (isleadbyte(*p))
#endif  /* _MT */
                    {
                        if(p[1]=='\0')
                        {
                            /*  this is a leadbyte followed by EOS -- a dud MBCS string
                                We choose not to assert here because this
                                function is defined to deal with dud strings on
                                input and return a known value
                            */
                            errno = EILSEQ;
                            return (size_t)-1;
                        }
                        else
                        {
                            p++;
                        }
                    }
                }
                bytecnt = ((int) ((char *)p - (char *)s));

#ifdef _MT
                if ( (count = MultiByteToWideChar( ptloci->lc_codepage,
#else  /* _MT */
                if ( (count = MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
                                                   MB_PRECOMPOSED,
                                                   s,
                                                   bytecnt,
                                                   pwcs,
                                                   (int)n )) == 0 )
                {
                    errno = EILSEQ;
                    return (size_t)-1;
                }

                return count; /* no NUL in string */
            }
        }
        else { /* pwcs == NULL, get size only, s must be NUL-terminated */
#ifdef _MT
            if (ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE)
#else  /* _MT */
            if (__lc_handle[LC_CTYPE] == _CLOCALEHANDLE)
#endif  /* _MT */
                return strlen(s);

            else {
#ifdef _MT
                if ( (count = MultiByteToWideChar( ptloci->lc_codepage,
#else  /* _MT */
                if ( (count = MultiByteToWideChar( __lc_codepage,
#endif  /* _MT */
                                                   MB_PRECOMPOSED |
                                                    MB_ERR_INVALID_CHARS,
                                                   s,
                                                   -1,
                                                   NULL,
                                                   0 )) == 0 )
                {
                    errno = EILSEQ;
                    return (size_t)-1;
                }

                return count - 1;
            }
        }

}
