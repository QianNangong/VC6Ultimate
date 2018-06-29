/***
*gcvt.c - convert floating point number to character string
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts floating point number into character string representation.
*
*******************************************************************************/

#include <cruntime.h>
#include <fltintrn.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>

/***
*double _gcvt(value, ndec, buffer) - convert floating point value to char
*       string
*
*Purpose:
*       _gcvt converts the value to a null terminated ASCII string
*       buf.  It attempts to produce ndigit significant digits
*       in Fortran F format if possible, ortherwise E format,
*       ready for printing.  Trailing zeros may be suppressed.
*       No error checking or overflow protection is provided.
*       NOTE - to avoid the possibility of generating floating
*       point instructions in this code we fool the compiler
*       about the type of the 'value' parameter using a struct.
*       This is OK since all we do is pass it off as a
*       parameter.
*
*Entry:
*       value - double - number to be converted
*       ndec - int - number of significant digits
*       buf - char * - buffer to place result
*
*Exit:
*       result is written into buffer; it will be overwritten if it has
*       not been made big enough.
*
*Exceptions:
*
*******************************************************************************/

char * __cdecl _gcvt (
        double value,
        int ndec,
        char *buf
        )
{

        STRFLT string;
        int    magnitude;
        char   *rc;
    _CRT_DOUBLE *pdvalue = (_CRT_DOUBLE *)&value;

        REG1 char *str;
        REG2 char *stop;

#ifdef _MT
        struct _strflt strfltstruct;    /* temporary buffers */
        char   resultstring[21];
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);

#endif  /* _MT */

        /* get the magnitude of the number */

#ifdef _MT
        string = _fltout2( *pdvalue, &strfltstruct, resultstring );
#else  /* _MT */
        string = _fltout( *pdvalue );
#endif  /* _MT */

        magnitude = string->decpt - 1;

        /* output the result according to the Fortran G format as outlined in
           Fortran language specification */

        if ( magnitude < -1  ||  magnitude > ndec-1 )
                /* then  Ew.d  d = ndec */
                rc = str = _cftoe( &value, buf, ndec-1, 0);
        else
                /* Fw.d  where d = ndec-string->decpt */
                rc = str = _cftof( &value, buf, ndec-string->decpt );

        while (*str && *str != *__LCONV(ptloci)->decimal_point)
                str++;

        if (*str++) {
                while (*str && *str != 'e')
                        str++;

                stop = str--;

                while (*str == '0')
                        str--;

                while (*++str = *stop++)
                        ;
        }

        return(rc);
}
