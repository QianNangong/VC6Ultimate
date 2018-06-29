/***
*mbtolwr.c - Convert character to lower case (MBCS).
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Convert character to lower case (MBCS).
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>

#include <cruntime.h>
#include <ctype.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _mbctolower - Convert character to lower case (MBCS)
*
*Purpose:
*       If the given character is upper case, convert it to lower case.
*       Handles MBCS characters correctly.
*
*       Note:  Use test against 0x00FF instead of _ISLEADBYTE
*       to ensure that we don't call SBCS routine with a two-byte
*       value.
*
*Entry:
*       unsigned int c = character to convert
*
*Exit:
*       Returns converted character
*
*Exceptions:
*
*******************************************************************************/

unsigned int __cdecl _mbctolower (unsigned int c)
{
        unsigned char val[2];
        unsigned char ret[4];
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();
#endif  /* _MT */

        if (c > 0x00FF)
        {
            val[0] = (c >> 8) & 0xFF;
            val[1] = c & 0xFF;

#ifdef _MT
            if ( !__ismbblead_mt(ptmbci, val[0]) )
#else  /* _MT */
            if ( !_ismbblead(val[0]) )
#endif  /* _MT */
                return c;


#ifdef _MT
            if ( __crtLCMapStringA( ptmbci->mblcid,
#else  /* _MT */
            if ( __crtLCMapStringA( __mblcid,
#endif  /* _MT */
                                    LCMAP_LOWERCASE,
                                    val,
                                    2,
                                    ret,
                                    2,
#ifdef _MT
                                    ptmbci->mbcodepage,
#else  /* _MT */
                                    __mbcodepage,
#endif  /* _MT */
                                    TRUE ) == 0 )
                return c;

            c = ret[1];
            c += ret[0] << 8;

            return c;


        }
        else
#ifdef _MT
            return (unsigned int)__mbbtolower_mt(ptmbci, (int)c);
#else  /* _MT */
            return (unsigned int)_mbbtolower((int)c);
#endif  /* _MT */
}

#endif  /* _MBCS */
