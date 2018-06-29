/***
*assert.c - Display a message and abort
*
*       Copyright (c) 1988-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <file2.h>
#include <internal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <awint.h>

#ifdef NDEBUG
#undef NDEBUG
#endif  /* NDEBUG */
#define _ASSERT_OK
#include <assert.h>


/*
 * assertion format string for use with output to stderr
 */
static char _assertstring[] = "Assertion failed: %s, file %s, line %d\n";

/*      Format of MessageBox for assertions:
*
*       ================= Microsft Visual C++ Debug Library ================
*
*       Assertion Failed!
*
*       Program: c:\test\mytest\foo.exe
*       File: c:\test\mytest\bar.c
*       Line: 69
*
*       Expression: <expression>
*
*       For information on how your program can cause an assertion
*       failure, see the Visual C++ documentation on asserts
*
*       (Press Retry to debug the application - JIT must be enabled)
*
*       ===================================================================
*/

/*
 * assertion string components for message box
 */
#define BOXINTRO    "Assertion failed!"
#define PROGINTRO   "Program: "
#define FILEINTRO   "File: "
#define LINEINTRO   "Line: "
#define EXPRINTRO   "Expression: "
#define INFOINTRO   "For information on how your program can cause an assertion\n" \
                    "failure, see the Visual C++ documentation on asserts"
#define HELPINTRO   "(Press Retry to debug the application - JIT must be enabled)"

static char * dotdotdot = "...";
static char * newline = "\n";
static char * dblnewline = "\n\n";

#define DOTDOTDOTSZ 3
#define NEWLINESZ   1
#define DBLNEWLINESZ   2

#define MAXLINELEN  60 /* max length for line in message box */
#define ASSERTBUFSZ (MAXLINELEN * 9) /* 9 lines in message box */

#if defined (_M_IX86)
#define _DbgBreak() __asm { int 3 }
#elif defined (_M_IA64)
void __break(int);
#pragma intrinsic (__break)
#define _DbgBreak() __break(0x80016)
#else  /* defined (_M_IA64) */
#define _DbgBreak() DebugBreak()
#endif  /* defined (_M_IA64) */

/***
*_assert() - Display a message and abort
*
*Purpose:
*       The assert macro calls this routine if the assert expression is
*       true.  By placing the assert code in a subroutine instead of within
*       the body of the macro, programs that call assert multiple times will
*       save space.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _assert (
        void *expr,
        void *filename,
        unsigned lineno
        )
{
        /*
         * Build the assertion message, then write it out. The exact form
         * depends on whether it is to be written out via stderr or the
         * MessageBox API.
         */
        if ( (__error_mode == _OUT_TO_STDERR) || ((__error_mode ==
               _OUT_TO_DEFAULT) && (__app_type == _CONSOLE_APP)) )
        {
            /*
             * Build message and write it out to stderr. It will be of the
             * form:
             *        Assertion failed: <expr>, file <filename>, line <lineno>
             */
            if ( !anybuf(stderr) )
            /*
             * stderr is unused, hence unbuffered, as yet. set it to
             * single character buffering (to avoid a malloc() of a
             * stream buffer).
             */
             (void) setvbuf(stderr, NULL, _IONBF, 0);

            fprintf(stderr, _assertstring, expr, filename, lineno);
            fflush(stderr);
        }
        else {
            int nCode;
            char * pch;
            char assertbuf[ASSERTBUFSZ];
            char progname[MAX_PATH + 1];

            /*
             * Line 1: box intro line
             */
            strcpy( assertbuf, BOXINTRO );
            strcat( assertbuf, dblnewline );

            /*
             * Line 2: program line
             */
            strcat( assertbuf, PROGINTRO );

            progname[MAX_PATH] = '\0';
            if ( !GetModuleFileName( NULL, progname, MAX_PATH ))
                strcpy( progname, "<program name unknown>");

            pch = (char *)progname;

            /* sizeof(PROGINTRO) includes the NULL terminator */
            if ( sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ > MAXLINELEN )
            {
                pch += (sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ) - MAXLINELEN;
                strncpy( pch, dotdotdot, DOTDOTDOTSZ );
            }

            strcat( assertbuf, pch );
            strcat( assertbuf, newline );

            /*
             * Line 3: file line
             */
            strcat( assertbuf, FILEINTRO );

            /* sizeof(FILEINTRO) includes the NULL terminator */
            if ( sizeof(FILEINTRO) + strlen(filename) + NEWLINESZ > MAXLINELEN )
            {
                size_t p, len, ffn;

                pch = (char *) filename;
                ffn = MAXLINELEN - sizeof(FILEINTRO) - NEWLINESZ;

                for ( len = strlen(filename), p = 1;
                      pch[len - p] != '\\' && pch[len - p] != '/' && p < len;
                      p++ );

                /* keeping pathname almost 2/3rd of full filename and rest
                 * is filename
                 */
                if ( (ffn - ffn/3) < (len - p) && ffn/3 > p )
                {
                    /* too long. using first part of path and the
                       filename string */
                    strncat( assertbuf, pch, ffn - DOTDOTDOTSZ - p );
                    strcat( assertbuf, dotdotdot );
                    strcat( assertbuf, pch + len - p );
                }
                else if ( ffn - ffn/3 > len - p )
                {
                    /* pathname is smaller. keeping full pathname and putting
                     * dotdotdot in the middle of filename
                     */
                    p = p/2;
                    strncat( assertbuf, pch, ffn - DOTDOTDOTSZ - p );
                    strcat( assertbuf, dotdotdot );
                    strcat( assertbuf, pch + len - p );
                }
                else
                {
                    /* both are long. using first part of path. using first and
                     * last part of filename.
                     */
                    strncat( assertbuf, pch, ffn - ffn/3 - DOTDOTDOTSZ );
                    strcat( assertbuf, dotdotdot );
                    strncat( assertbuf, pch + len - p, ffn/6 - 1 );
                    strcat( assertbuf, dotdotdot );
                    strcat( assertbuf, pch + len - (ffn/3 - ffn/6 - 2) );
                }

            }
            else
                /* plenty of room on the line, just append the filename */
                strcat( assertbuf, filename );

            strcat( assertbuf, newline );

            /*
             * Line 4: line line
             */
            strcat( assertbuf, LINEINTRO );
            _itoa( lineno, assertbuf + strlen(assertbuf), 10 );
            strcat( assertbuf, dblnewline );

            /*
             * Line 5: message line
             */
            strcat( assertbuf, EXPRINTRO );

            /* sizeof(HELPINTRO) includes the NULL terminator */

            if (    strlen(assertbuf) +
                    strlen(expr) +
                    2*DBLNEWLINESZ +
                    sizeof(INFOINTRO)-1 +
                    sizeof(HELPINTRO) > ASSERTBUFSZ )
            {
                strncat( assertbuf, expr,
                    ASSERTBUFSZ -
                    (strlen(assertbuf) +
                    DOTDOTDOTSZ +
                    2*DBLNEWLINESZ +
                    sizeof(INFOINTRO)-1 +
                    sizeof(HELPINTRO)) );
                strcat( assertbuf, dotdotdot );
            }
            else
                strcat( assertbuf, expr );

            strcat( assertbuf, dblnewline );

            /*
             * Line 6, 7: info line
             */

            strcat(assertbuf, INFOINTRO);
            strcat( assertbuf, dblnewline );

            /*
             * Line 8: help line
             */
            strcat(assertbuf, HELPINTRO);

            /*
             * Write out via MessageBox
             */

            nCode = __crtMessageBoxA(assertbuf,
                "Microsoft Visual C++ Runtime Library",
                MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

            /* Abort: abort the program */
            if (nCode == IDABORT)
            {
                /* raise abort signal */
                raise(SIGABRT);

                /* We usually won't get here, but it's possible that
                   SIGABRT was ignored.  So exit the program anyway. */

                _exit(3);
            }

            /* Retry: call the debugger */
            if (nCode == IDRETRY)
            {
                _DbgBreak();
                /* return to user code */
                return;
            }

            /* Ignore: continue execution */
            if (nCode == IDIGNORE)
                return;
        }

        abort();
}
