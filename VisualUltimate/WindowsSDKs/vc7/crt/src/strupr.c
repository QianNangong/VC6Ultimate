/***
*strupr.c - routine to map lower-case characters in a string to upper-case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts all the lower case characters in a string to upper case,
*       in place.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <limits.h>     /* for INT_MAX */
#include <mtdll.h>
#include <awint.h>
#include <dbgint.h>

/***
*char *_strupr(string) - map lower-case characters in a string to upper-case
*
*Purpose:
*       _strupr() converts lower-case characters in a null-terminated string
*       to their upper-case equivalents.  Conversion is done in place and
*       characters other than lower-case letters are not modified.
*
*       In the C locale, this function modifies only 7-bit ASCII characters
*       in the range 0x61 through 0x7A ('a' through 'z').
*
*       If the locale is not the 'C' locale, LCMapString() is used to do
*       the work.  Assumes enough space in the string to hold result.
*
*Entry:
*       char *string - string to change to upper case
*
*Exit:
*       input string address
*
*Exceptions:
*       The original string is returned unchanged on any error.
*
*******************************************************************************/

char * __cdecl _strupr (
        char * string
        )
{

        int dstlen;                 /* len of dst string, with null  */
        unsigned char *dst;         /* destination string */
        int malloc_flag = 0;
#ifdef _MT
        pthreadlocinfo ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        if ( ptloci->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
#else  /* _MT */
        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
#endif  /* _MT */
        {
            char *cp;       /* traverses string for C locale conversion */

            for ( cp = string ; *cp ; ++cp )
                if ( ('a' <= *cp) && (*cp <= 'z') )
                    *cp -= 'a' - 'A';

            return(string);
        }   /* C locale */

        /* Inquire size of dst string */
#ifdef _MT
        if ( 0 == (dstlen = __crtLCMapStringA( ptloci->lc_handle[LC_CTYPE],
#else  /* _MT */
        if ( 0 == (dstlen = __crtLCMapStringA( __lc_handle[LC_CTYPE],
#endif  /* _MT */
                                               LCMAP_UPPERCASE,
                                               string,
                                               -1,
                                               NULL,
                                               0,
#ifdef _MT
                                               ptloci->lc_codepage,
#else  /* _MT */
                                               __lc_codepage,
#endif  /* _MT */
                                               TRUE )) )
            return(string);

        /* Allocate space for dst */
        __try {
            dst = (unsigned char *)_alloca(dstlen * sizeof(unsigned char));
        }
        __except( EXCEPTION_EXECUTE_HANDLER )
        {
            _resetstkoflw();
            dst = NULL;
        }

        if ( dst == NULL ) {
            dst = (unsigned char *)_malloc_crt(dstlen * sizeof(unsigned char));
            malloc_flag++;
        }

        /* Map src string to dst string in alternate case */
        if ( (dst != NULL) &&
#ifdef _MT
             (__crtLCMapStringA( ptloci->lc_handle[LC_CTYPE],
#else  /* _MT */
             (__crtLCMapStringA( __lc_handle[LC_CTYPE],
#endif  /* _MT */
                                 LCMAP_UPPERCASE,
                                 string,
                                 -1,
                                 dst,
                                 dstlen,
#ifdef _MT
                                 ptloci->lc_codepage,
#else  /* _MT */
                                 __lc_codepage,
#endif  /* _MT */
                                 TRUE ) != 0) )
            /* copy dst string to return string */
            strcpy(string, dst);

        if ( malloc_flag )
            _free_crt(dst);

        return(string);

}
