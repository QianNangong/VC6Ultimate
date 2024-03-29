/***
*wcsspn.c - find length of initial substring of chars from a control string
*       (wide-character strings)
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcsspn() - finds the length of the initial substring of
*       a string consisting entirely of characters from a control string
*       (wide-character strings).
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>

/***
*int wcsspn(string, control) - find init substring of control chars
*
*Purpose:
*       Finds the index of the first character in string that does belong
*       to the set of characters specified by control.  This is
*       equivalent to the length of the initial substring of string that
*       consists entirely of characters from control.  The L'\0' character
*       that terminates control is not considered in the matching process
*       (wide-character strings).
*
*Entry:
*       wchar_t *string - string to search
*       wchar_t *control - string containing characters not to search for
*
*Exit:
*       returns index of first wchar_t in string not in control
*
*Exceptions:
*
*******************************************************************************/

size_t __cdecl wcsspn (
        const wchar_t * string,
        const wchar_t * control
        )
{
        wchar_t *str = (wchar_t *) string;
        wchar_t *ctl;

        /* 1st char not in control string stops search */
        while (*str) {
            for (ctl = (wchar_t *)control; *ctl != *str; ctl++) {
                if (*ctl == (wchar_t)0) {
                    /*
                     * reached end of control string without finding a match
                     */
                    return (size_t)(str - string);
                }
            }
            str++;
        }
        /*
         * The whole string consisted of characters from control
         */
        return (size_t)(str - string);
}

