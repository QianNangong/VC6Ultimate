/***
*strerror.c - Contains the strerror C runtime.
*
*       Copyright (c) 1987-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The strerror runtime accepts an error number as input
*       and returns the corresponding error string.
*
*       NOTE: The "old" strerror C runtime resides in file _strerr.c
*       and is now called _strerror.  The new strerror runtime
*       conforms to the ANSI standard.
*
*******************************************************************************/

#include <cruntime.h>
#include <errmsg.h>
#include <stdlib.h>
#include <syserr.h>
#include <string.h>
#include <mtdll.h>
#include <tchar.h>
#ifdef _MT
#include <malloc.h>
#include <stddef.h>
#endif  /* _MT */
#include <dbgint.h>

/* [NOTE: The _MT error message buffer is shared by both strerror
   and _strerror so must be the max length of both. */
#ifdef _MT
/* Max length of message = user_string(94)+system_string+2 */
#define _ERRMSGLEN_ 94+_SYS_MSGMAX+2
#else  /* _MT */
/* Max length of message = system_string+2 */
#define _ERRMSGLEN_ _SYS_MSGMAX+2
#endif  /* _MT */

#ifdef _UNICODE
#define _terrmsg    _werrmsg
#else  /* _UNICODE */
#define _terrmsg    _errmsg
#endif  /* _UNICODE */

/***
*char *strerror(errnum) - Map error number to error message string.
*
*Purpose:
*       The strerror runtime takes an error number for input and
*       returns the corresponding error message string.  This routine
*       conforms to the ANSI standard interface.
*
*Entry:
*       int errnum - Integer error number (corresponding to an errno value).
*
*Exit:
*       char * - Strerror returns a pointer to the error message string.
*       This string is internal to the strerror routine (i.e., not supplied
*       by the user).
*
*Exceptions:
*       None.
*
*******************************************************************************/

#ifdef _UNICODE
wchar_t * cdecl _wcserror(
#else  /* _UNICODE */
char * __cdecl strerror (
#endif  /* _UNICODE */
        int errnum
        )
{
#ifdef _MT

        _ptiddata ptd = _getptd();

        _TCHAR *errmsg;
        static _TCHAR errmsg_backup[_SYS_MSGMAX+2];

#else  /* _MT */

        static _TCHAR errmsg[_ERRMSGLEN_];  /* Longest errmsg + \0 */

#endif  /* _MT */

#ifdef _MT

        if ( (ptd->_terrmsg == NULL) && ((ptd->_terrmsg =
            _malloc_crt(_ERRMSGLEN_ * sizeof(_TCHAR)))
            == NULL) )
                errmsg = errmsg_backup; /* error: use backup */
        else
                errmsg = ptd->_terrmsg;

#endif  /* _MT */
#ifdef _UNICODE
    mbstowcs(errmsg, _sys_err_msg(errnum), _ERRMSGLEN_);
#else  /* _UNICODE */
        strcpy(errmsg, _sys_err_msg(errnum));
#endif  /* _UNICODE */
        return(errmsg);
}
