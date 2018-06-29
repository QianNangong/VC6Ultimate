/***
*chdir.c - change directory
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file has the _chdir() function - change current directory.
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <mtdll.h>
#include <internal.h>
#include <direct.h>
#include <stdlib.h>
#include <tchar.h>

/***
*int _chdir(path) - change current directory
*
*Purpose:
*       Changes the current working directory to that given in path.
*
*Entry:
*       _TSCHAR *path - directory to change to
*
*Exit:
*       returns 0 if successful,
*       returns -1 and sets errno if unsuccessful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tchdir (
        const _TSCHAR *path
        )
{
        _TSCHAR env_var[4];
        _TSCHAR abspath[MAX_PATH+1];

        if ( SetCurrentDirectory((LPTSTR)path) )
        {
            /*
             * If the new current directory path is NOT a UNC path, we must
             * update the OS environment variable specifying the current
             * directory for what is now current drive. To do this, get the
             * full current directory, build the environment variable string
             * and call SetEnvironmentVariable(). We need to do this because
             * SetCurrentDirectory does not (i.e., does not update the
             * current-directory-on-drive environment variables) and other
             * functions (fullpath, spawn, etc) need them to be set.
             *
             * If associated with a 'drive', the current directory should
             * have the form of the example below:
             *
             *  D:\nt\private\mytests
             *
             * so that the environment variable should be of the form:
             *
             *  =D:=D:\nt\private\mytests
             *
             */
            if ( GetCurrentDirectory(MAX_PATH+1, (LPTSTR)abspath) != 0 )
            {
                /*
                 * check if it is a UNC name, just return if is
                 */
                if ( ((abspath[0] == _T('\\')) || (abspath[0] == _T('/'))) &&
                     (abspath[0] == abspath[1]) )
                    return 0;

                env_var[0] = _T('=');
                env_var[1] = (_TSCHAR) _totupper((_TUCHAR)abspath[0]);
                env_var[2] = _T(':');
                env_var[3] = _T('\0');

                if ( SetEnvironmentVariable(env_var, abspath) )
                    return 0;
            }
        }

        _dosmaperr(GetLastError());
        return -1;
}
