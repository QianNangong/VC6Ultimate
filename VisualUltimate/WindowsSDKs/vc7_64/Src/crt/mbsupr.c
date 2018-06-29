/***
*mbsupr.c - Convert string upper case (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Convert string upper case (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <ctype.h>
#include <mbdata.h>
#include <mbstring.h>
#include <mbctype.h>


/***
* _mbsupr - Convert string upper case (MBCS)
*
*Purpose:
*       Converts all the lower case characters in a string
*       to upper case in place.   Handles MBCS chars correctly.
*
*Entry:
*       unsigned char *string = pointer to string
*
*Exit:
*       Returns a pointer to the input string; no error return.
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbsupr(
        unsigned char *string
        )
{
        unsigned char *cp;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();
#endif  /* _MT */

        for (cp=string; *cp; cp++)
        {
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *cp) )
#else  /* _MT */
            if ( _ismbblead(*cp) )
#endif  /* _MT */
            {


                int retval;
                unsigned char ret[4];

#ifdef _MT
                if ( (retval = __crtLCMapStringA( ptmbci->mblcid,
#else  /* _MT */
                if ( (retval = __crtLCMapStringA( __mblcid,
#endif  /* _MT */
                                                  LCMAP_UPPERCASE,
                                                  cp,
                                                  2,
                                                  ret,
                                                  2,
#ifdef _MT
                                                  ptmbci->mbcodepage,
#else  /* _MT */
                                                  __mbcodepage,
#endif  /* _MT */
                                                  TRUE )) == 0 )
                    return NULL;

                *cp = ret[0];

                if (retval > 1)
                    *(++cp) = ret[1];


            }
            else
                /* single byte, macro version */
#ifdef _MT
                *cp = (unsigned char) __mbbtoupper_mt(ptmbci, *cp);
#else  /* _MT */
                *cp = (unsigned char) _mbbtoupper(*cp);
#endif  /* _MT */
        }

        return string ;
}

#endif  /* _MBCS */
