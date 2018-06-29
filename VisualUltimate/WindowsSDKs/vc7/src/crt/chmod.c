/***
*chmod.c - change file attributes
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _chmod() - change file attributes
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <tchar.h>

/***
*int _chmod(path, mode) - change file mode
*
*Purpose:
*       Changes file mode permission setting to that specified in
*       mode.  The only XENIX mode bit supported is user write.
*
*Entry:
*       _TSCHAR *path - file name
*       int mode - mode to change to
*
*Exit:
*       returns 0 if successful
*       returns -1 and sets errno if not successful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tchmod (
        const _TSCHAR *path,
        int mode
        )
{
        DWORD attr;

        attr = GetFileAttributes((LPTSTR)path);
        if (attr  == 0xffffffff) {
                /* error occured -- map error code and return */
                _dosmaperr(GetLastError());
                return -1;
        }

        if (mode & _S_IWRITE) {
                /* clear read only bit */
                attr &= ~FILE_ATTRIBUTE_READONLY;
        }
        else {
                /* set read only bit */
                attr |= FILE_ATTRIBUTE_READONLY;
        }

        /* set new attribute */
        if (!SetFileAttributes((LPTSTR)path, attr)) {
                /* error occured -- map error code and return */
                _dosmaperr(GetLastError());
                return -1;
        }

        return 0;
}
