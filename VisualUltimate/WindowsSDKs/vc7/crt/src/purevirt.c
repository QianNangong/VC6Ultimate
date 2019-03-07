/***
*purevirt.c - stub to trap pure virtual function calls
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _purecall() -
*
*******************************************************************************/


#include <cruntime.h>
#include <internal.h>
#include <rterr.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
//
// The global variable:
//

_purecall_handler __pPurecall= NULL;

/***
*void _purecall(void) -
*
*Purpose:
*       The compiler calls this if a pure virtual happens
*
*Entry:
*       No arguments
*
*Exit:
*       Never returns
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _purecall(
        void
        )
{
        if(__pPurecall)
        {
            __pPurecall();

            /*  shouldn't return, but if it does, we drop back to
                default behaviour
            */
        }

        _amsg_exit(_RT_PUREVIRT);
}

/***
*void _set_purecall_handler(void) -
*
*Purpose:
*       Establish a handler to be called when a pure virtual is called
*       Note that if you link to the crt statically, and replace
*       _purecall, then none of this will happen.
*
*       This function is not thread-safe
*
*Entry:
*       New handler
*
*Exit:
*       Old handler
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP _purecall_handler __cdecl
_set_purecall_handler( _purecall_handler pNew )
{
    _purecall_handler pOld = NULL;

    pOld = __pPurecall;
    __pPurecall = pNew;

    return pOld;
}

