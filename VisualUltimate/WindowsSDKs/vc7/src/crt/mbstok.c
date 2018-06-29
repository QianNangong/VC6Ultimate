/***
*mbstok.c - Break string into tokens (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Break string into tokens (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <stddef.h>

/***
* _mbstok - Break string into tokens (MBCS)
*
*Purpose:
*       strtok considers the string to consist of a sequence of zero or more
*       text tokens separated by spans of one or more control chars. the first
*       call, with string specified, returns a pointer to the first char of the
*       first token, and will write a null char into string immediately
*       following the returned token. subsequent calls with zero for the first
*       argument (string) will work thru the string until no tokens remain. the
*       control string may be different from call to call. when no tokens remain
*       in string a NULL pointer is returned. remember the control chars with a
*       bit map, one bit per ascii char. the null char is always a control char.
*
*       MBCS chars supported correctly.
*
*Entry:
*       char *string = string to break into tokens.
*       char *sepset = set of characters to use as seperators
*
*Exit:
*       returns pointer to token, or NULL if no more tokens
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbstok(
        unsigned char * string,
        const unsigned char * sepset
        )
{
        unsigned char *nextsep;
#ifdef _MT
        _ptiddata ptd = _getptd();
        unsigned char *nextoken;
        pthreadmbcinfo ptmbci;

        if ( (ptmbci = ptd->ptmbcinfo) != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();

        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        static unsigned char *nextoken;

        if ( _ISNOTMBCP )
#endif  /* _MT */
            return strtok(string, sepset);

        /* init start of scan */

        if (string)
            nextoken = string;
        else
        /* If string==NULL, continue with previous string */
        {

#ifdef _MT
            nextoken = ptd->_mtoken;
#endif  /* _MT */

            if (!nextoken)
                return NULL;
        }

        /* skip over lead seperators */

#ifdef _MT
        if ( (string = __mbsspnp_mt(ptmbci, nextoken, sepset)) == NULL )
#else  /* _MT */
        if ( (string = _mbsspnp(nextoken, sepset)) == NULL )
#endif  /* _MT */
            return(NULL);


        /* test for end of string */

        if ( (*string == '\0') ||
#ifdef _MT
             ((__ismbblead_mt(ptmbci, *string)) && (string[1] == '\0')) )
#else  /* _MT */
             ((_ismbblead(*string)) && (string[1] == '\0')) )
#endif  /* _MT */
            return(NULL);


        /* find next seperator */

#ifdef _MT
        nextsep = __mbspbrk_mt(ptmbci, string, sepset);
#else  /* _MT */
        nextsep = _mbspbrk(string, sepset);
#endif  /* _MT */

        if ((nextsep == NULL) || (*nextsep == '\0'))
            nextoken = NULL;
        else {
#ifdef _MT
            if ( __ismbblead_mt(ptmbci, *nextsep) )
#else  /* _MT */
            if ( _ismbblead(*nextsep) )
#endif  /* _MT */
                *nextsep++ = '\0';
            *nextsep = '\0';
            nextoken = ++nextsep;
        }

#ifdef _MT
        /* Update the corresponding field in the per-thread data * structure */

        ptd->_mtoken = nextoken;

#endif  /* _MT */

        return(string);
}

#endif  /* _MBCS */
