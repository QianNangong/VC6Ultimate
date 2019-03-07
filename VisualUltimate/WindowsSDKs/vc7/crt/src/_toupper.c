/***
*_toupper.c - convert character to uppercase
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _Toupper()
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <stddef.h>
#include <xlocinfo.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <awint.h>

/* remove macro definitions of _toupper() and toupper()
 */
#undef  _toupper
#undef  toupper

/***
*int _toupper(c) - convert character to uppercase
*
*Purpose:
*       _toupper() is a version of toupper with a locale argument.
*
*Entry:
*       c - int value of character to be converted
*       const _Ctypevec * = pointer to locale info
*
*Exit:
*       returns int value of uppercase representation of c
*
*Exceptions:
*
*******************************************************************************/

#ifdef _MT
int __cdecl _Toupper_lk (
        int c,
        const _Ctypevec *ploc
        );
#endif  /* _MT */

_CRTIMP2 int __cdecl _Toupper (
        int c,
        const _Ctypevec *ploc
        )
{
#ifdef _MT

        LCID handle;
        int local_lock_flag;

        if (ploc == 0)
            handle = ___lc_handle_func()[LC_CTYPE];
        else
            handle = ploc->_Hand;

        if (handle == _CLOCALEHANDLE)
        {
            if ( (c >= 'a') && (c <= 'z') )
                c = c - ('a' - 'A');
            return c;
        }

        _lock_locale( local_lock_flag )
        __TRY

            c = _Toupper_lk(c, ploc);

        __FINALLY
            _unlock_locale( local_lock_flag )
        __END_TRY_FINALLY

        return c;
}

/***
*int _toupper_lk(c) - convert character to uppercase
*
*Purpose:
*       Multi-thread function only! Non-locking version of toupper.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _Toupper_lk (
        int c,
        const _Ctypevec *ploc
        )
{

#endif  /* _MT */

        int size;
        unsigned char inbuffer[3];
        unsigned char outbuffer[3];

        LCID handle;
        UINT codepage;

        if (ploc == 0)
        {
            handle = ___lc_handle_func()[LC_CTYPE];
            codepage = ___lc_codepage_func();
        }
        else
        {
            handle = ploc->_Hand;
            codepage = ploc->_Page;
        }

        if (handle == _CLOCALEHANDLE)
        {
            if ( (c >= 'a') && (c <= 'z') )
                c = c - ('a' - 'A');
            return c;
        }

        /* if checking case of c does not require API call, do it */
        if ((unsigned)c < 256)
        {
            if (ploc == 0)
            {
                if (!islower(c))
                {
                    return c;
                }
            }
            else
            {
                if (!(ploc->_Table[c] & _LOWER))
                {
                    return c;
                }
            }
        }

        /* convert int c to multibyte string */
        if (_cpp_isleadbyte(c >> 8 & 0xff))
        {
            inbuffer[0] = (c >> 8 & 0xff);
            inbuffer[1] = (unsigned char)c;
            inbuffer[2] = 0;
            size = 2;
        } else {
            inbuffer[0] = (unsigned char)c;
            inbuffer[1] = 0;
            size = 1;
        }

        /* convert wide char to uppercase */
        if (0 == (size = __crtLCMapStringA(handle, LCMAP_UPPERCASE,
            inbuffer, size, outbuffer, 3, codepage, TRUE)))
        {
            return c;
        }

        /* construct integer return value */
        if (size == 1)
            return ((int)outbuffer[0]);
        else
            return ((int)outbuffer[1] | ((int)outbuffer[0] << 8));

}
