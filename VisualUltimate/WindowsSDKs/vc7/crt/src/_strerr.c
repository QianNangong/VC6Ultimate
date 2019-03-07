/***
*_strerr.c - routine for indexing into system error list
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   Returns system error message index by errno; conforms to the
*   XENIX standard, much compatibility with 1983 uniforum draft standard.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <errmsg.h>
#include <syserr.h>
#include <string.h>
#include <tchar.h>
#include <malloc.h>
#include <mtdll.h>
#include <dbgint.h>

/* Max length of message = user_string(94)+system_string+2 */
/* [NOTE: The mthread error message buffer is shared by both strerror
   and _strerror so must be the max length of both. */
#define _ERRMSGLEN_ (94+_SYS_MSGMAX+2)

#ifdef _UNICODE
#define _terrmsg    _werrmsg
#else  /* _UNICODE */
#define _terrmsg    _errmsg
#endif  /* _UNICODE */


/***
*char *_strerror(message) - get system error message
*
*Purpose:
*   builds an error message consisting of the users error message
*   (the message parameter), followed by ": ", followed by the system
*   error message (index through errno), followed by a newline.  If
*   message is NULL or a null string, returns a pointer to just
*   the system error message.
*
*Entry:
*   char *message - user's message to prefix system error message
*
*Exit:
*   returns pointer to static memory containing error message.
*   returns NULL if malloc() fails in multi-thread versions.
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
wchar_t * __cdecl __wcserror(
#else  /* _UNICODE */
char * __cdecl _strerror (
#endif  /* _UNICODE */
    REG1 const _TCHAR *message
    )
{
#ifdef _MT

    _ptiddata ptd = _getptd();
    _TCHAR *bldmsg;

#else  /* _MT */

    static _TCHAR bldmsg[_ERRMSGLEN_];

#endif  /* _MT */


#ifdef _MT

    /* Use per thread buffer area (malloc space, if necessary) */
    /* [NOTE: This buffer is shared between _strerror and streror.] */

    if ( (ptd->_terrmsg == NULL) && ((ptd->_terrmsg =
            _malloc_crt(_ERRMSGLEN_ * sizeof(_TCHAR))) == NULL) )
            return(NULL);
    bldmsg = ptd->_terrmsg;

#endif  /* _MT */

    /* Build the error message */

    bldmsg[0] = '\0';

    if (message && *message) {
        // should leave space for ": \n\0"
        _tcsncat( bldmsg, message, _ERRMSGLEN_-4 );
        _tcscat( bldmsg, _T(": "));
    }

    //  We should have extra space for "\n\0"
#ifdef _UNICODE
    mbstowcs(bldmsg + wcslen(bldmsg), _sys_err_msg( errno ), _ERRMSGLEN_ - wcslen(bldmsg)-2);
#else  /* _UNICODE */
    strncat( bldmsg, _sys_err_msg( errno ),_ERRMSGLEN_-strlen(bldmsg)-2);
#endif  /* _UNICODE */
    return( _tcscat( bldmsg, _T("\n")) );
}
