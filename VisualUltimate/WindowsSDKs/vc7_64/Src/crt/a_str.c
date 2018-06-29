/***
*a_str.c - A version of GetStringType.
*
*       Copyright (c) 1993-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Use either GetStringTypeA or GetStringTypeW depending on which is
*       unstubbed.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <setlocal.h>
#include <locale.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>
#include <awint.h>
#include <mtdll.h>

#define USE_W   1
#define USE_A   2

/***
*int __cdecl __crtGetStringTypeA - Get type information about an ANSI string.
*
*Purpose:
*       Internal support function. Assumes info in ANSI string format. Tries
*       to use NLS API call GetStringTypeA if available and uses GetStringTypeW
*       if it must. If neither are available it fails and returns FALSE.
*
*Entry:
*       DWORD    dwInfoType  - see NT\Chicago docs
*       LPCSTR   lpSrcStr    - char (byte) string for which character types
*                              are requested
*       int      cchSrc      - char (byte) count of lpSrcStr (including NULL
*                              if any)
*       LPWORD   lpCharType  - word array to receive character type information
*                              (must be twice the size of lpSrcStr)
*       int      code_page   - for MB/WC conversion. If 0, use __lc_codepage
*       int      lcid        - for A call, specify LCID, If 0, use
*                              __lc_handle[LC_CTYPE].
*       BOOL     bError      - TRUE if MB_ERR_INVALID_CHARS set on call to
*                              MultiByteToWideChar when GetStringTypeW used.
*
*Exit:
*       Success: TRUE
*       Failure: FALSE
*
*Exceptions:
*
*******************************************************************************/

BOOL __cdecl __crtGetStringTypeA(
        DWORD    dwInfoType,
        LPCSTR   lpSrcStr,
        int      cchSrc,
        LPWORD   lpCharType,
        int      code_page,
        int      lcid,
        BOOL     bError
        )
{
        static int f_use = 0;
#ifdef _MT
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
#endif  /* _MT */

    /*
     * Look for unstubbed 'preferred' flavor. Otherwise use available
     * flavor. Must actually call the function to ensure it's not a stub.
     * (Always try wide version first so WinNT can process codepage correctly.)
     */

    if (0 == f_use)
    {
        unsigned short dummy;

        if (0 != GetStringTypeW(CT_CTYPE1, L"\0", 1, &dummy))
            f_use = USE_W;

        else if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            f_use = USE_A;
    }

    /* Use "A" version */

    if (USE_A == f_use || f_use == 0)
    {
        char *cbuffer = NULL;
        int ret;
        int AnsiCP;

        if (0 == lcid)
            lcid = __LC_HANDLE(ptloci)[LC_CTYPE];
        if (0 == code_page)
            code_page = __LC_CODEPAGE(ptloci);

        if ( -1 == (AnsiCP = __ansicp(lcid)))
            return FALSE;
        /* If current code-page is not ansi code page, convert it to ansi code page
         * as GetStringTypeA uses ansi code page to find the strig type.
         */
        if ( AnsiCP != code_page)
        {
            cbuffer = __convertcp(code_page, AnsiCP, lpSrcStr, &cchSrc, NULL, 0);
            if (cbuffer == NULL)
                return FALSE;
            lpSrcStr = cbuffer;
        }

        ret = GetStringTypeA(lcid, dwInfoType, lpSrcStr, cchSrc, lpCharType);
        if ( cbuffer != NULL)
            _free_crt(cbuffer);
        return ret;
    }

    /* Use "W" version */

    if (USE_W == f_use)
    {
        int retval1;
        int buff_size;
        wchar_t *wbuffer;
        BOOL retval2 = FALSE;
        int malloc_flag = 0;

        /*
         * Convert string and return the requested information. Note that
         * we are converting to a wide character string so there is not a
         * one-to-one correspondence between number of multibyte chars in the
         * input string and the number of wide chars in the buffer. However,
         * there had *better be* a one-to-one correspondence between the
         * number of multibyte characters and the number of WORDs in the
         * return buffer.
         */

        /*
         * Use __lc_codepage for conversion if code_page not specified
         */

        if (0 == code_page)
            code_page = __LC_CODEPAGE(ptloci);

        /* find out how big a buffer we need */
        if ( 0 == (buff_size = MultiByteToWideChar( code_page,
                                                    bError ?
                                                        MB_PRECOMPOSED |
                                                        MB_ERR_INVALID_CHARS
                                                        : MB_PRECOMPOSED,
                                                    lpSrcStr,
                                                    cchSrc,
                                                    NULL,
                                                    0 )) )
            return FALSE;

        /* allocate enough space for wide chars */
        __try {
            wbuffer = (wchar_t *)_alloca( sizeof(wchar_t) * buff_size );
            (void)memset( wbuffer, 0, sizeof(wchar_t) * buff_size );
        }
        __except( EXCEPTION_EXECUTE_HANDLER ) {
            _resetstkoflw();
            wbuffer = NULL;
        }

        if ( wbuffer == NULL ) {
            if ( (wbuffer = (wchar_t *)_calloc_crt(sizeof(wchar_t), buff_size))
                 == NULL )
                return FALSE;
            malloc_flag++;
        }

        /* do the conversion */
        if ( 0 != (retval1 = MultiByteToWideChar( code_page,
                                                 MB_PRECOMPOSED,
                                                 lpSrcStr,
                                                 cchSrc,
                                                 wbuffer,
                                                 buff_size )) )
            /* obtain result */
            retval2 = GetStringTypeW( dwInfoType,
                                      wbuffer,
                                      retval1,
                                      lpCharType );

        if ( malloc_flag )
            _free_crt(wbuffer);

        return retval2;
    }
    else   /* f_use is neither USE_A nor USE_W */
        return FALSE;
}
