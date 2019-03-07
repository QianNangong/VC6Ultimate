/***
*mbsrev.c - Reverse a string in place (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Reverse a string in place (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <crtdbg.h>


/***
* _mbsrev - Reverse a string in place (MBCS)
*
*Purpose:
*       Reverses the order of characters in the string.  The terminating
*       null character remains in place.  The order of MBCS characters
*       is not changed.
*
*Entry:
*       unsigned char *string = string to reverse
*
*Exit:
*       returns string - now with reversed characters
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbsrev(
        unsigned char *string
        )
{

        unsigned char *start = string;
        unsigned char *left  = string;
        unsigned char c;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return _strrev(string);


        /* first go through and reverse the bytes in MBCS chars */
        while ( *string ) {
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *string++) ) {
#else  /* _MT */
            if ( _ismbblead(*string++) ) {
#endif  /* _MT */
                if ( *string ) {
                    c = *string;
                    *string = *(string - 1);
                    *(string - 1) = c;
                    string++;
                }
                else
                {
                    /*  second byte is EOS
                        There is nothing really satisfying to do here. We have a string
                        that ends in leadbyte,'\0'. Reversing this would lead to the leadbyte
                        becoming falsely attached to the character before it:
                        (XL0 -> LX0, X has suddenly become a trailbyte)

                        So what we choose to do is assert and purge the dud byte from within the
                        string.
                    */
                    _ASSERTE(("Bad MBCS string passed into _mbsrev",0));

                    /* String has at least moved once already, so this is safe */
                    _ASSERTE(string>start);

                    /* move back one to point at the dud leadbyte */
                    --string;

                    /* now truncate the string one byte earlier */
                    *string='\0';

                    break;
                }
            }
        }

        /* now reverse the whole string */
        string--;
        while ( left < string ) {
            c = *left;
            *left++ = *string;
            *string-- = c;
        }

        return ( start );
}

#endif  /* _MBCS */
