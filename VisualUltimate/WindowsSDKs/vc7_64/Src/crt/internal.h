/***
*internal.h - contains declarations of internal routines and variables
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Declares routines and variables used internally by the C run-time.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_INTERNAL
#define _INC_INTERNAL

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <cruntime.h>

/*
 * Conditionally include windows.h to pick up the definition of
 * CRITICAL_SECTION.
 */
#ifdef _MT
#include <windows.h>
#endif  /* _MT */

#if !defined (_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#define _W64
#endif  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#endif  /* !defined (_W64) */

/* Define _CRTIMP */

#ifndef _CRTIMP
#ifdef CRTDLL
#define _CRTIMP __declspec(dllexport)
#else  /* CRTDLL */
#ifdef _DLL
#define _CRTIMP __declspec(dllimport)
#else  /* _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* CRTDLL */
#endif  /* _CRTIMP */


/* Define _CRTIMP1 */

#ifndef _CRTIMP1
#ifdef CRTDLL1
#define _CRTIMP1 __declspec(dllexport)
#else  /* CRTDLL1 */
#define _CRTIMP1 _CRTIMP
#endif  /* CRTDLL1 */
#endif  /* _CRTIMP1 */


/* Define _CRTIMP2 */
#ifndef _CRTIMP2
#if defined (CRTDLL2)
#define _CRTIMP2 __declspec(dllexport)
#else  /* defined (CRTDLL2) */
#if defined (_DLL) && !defined (_STATIC_CPPLIB)
#define _CRTIMP2 __declspec(dllimport)
#else  /* defined (_DLL) && !defined (_STATIC_CPPLIB) */
#define _CRTIMP2
#endif  /* defined (_DLL) && !defined (_STATIC_CPPLIB) */
#endif  /* defined (CRTDLL2) */
#endif  /* _CRTIMP2 */


/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */

#ifndef _INTPTR_T_DEFINED
#ifdef _WIN64
typedef __int64             intptr_t;
#else  /* _WIN64 */
typedef _W64 int            intptr_t;
#endif  /* _WIN64 */
#define _INTPTR_T_DEFINED
#endif  /* _INTPTR_T_DEFINED */

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */

/* Define function types used in several startup sources */

typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);


#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP int * __p__commode(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#if defined (SPECIAL_CRTEXE) && defined (_DLL)
        extern int _commode;
#else  /* defined (SPECIAL_CRTEXE) && defined (_DLL) */
_CRTIMP extern int _commode;
#endif  /* defined (SPECIAL_CRTEXE) && defined (_DLL) */


/*
 * Control structure for lowio file handles
 */
typedef struct {
        intptr_t osfhnd;    /* underlying OS file HANDLE */
        char osfile;        /* attributes of file (e.g., open in text mode?) */
        char pipech;        /* one char buffer for handles opened on pipes */
#ifdef _MT
        int lockinitflag;
        CRITICAL_SECTION lock;
#endif  /* _MT */
    }   ioinfo;

/*
 * Definition of IOINFO_L2E, the log base 2 of the number of elements in each
 * array of ioinfo structs.
 */
#define IOINFO_L2E          5

/*
 * Definition of IOINFO_ARRAY_ELTS, the number of elements in ioinfo array
 */
#define IOINFO_ARRAY_ELTS   (1 << IOINFO_L2E)

/*
 * Definition of IOINFO_ARRAYS, maximum number of supported ioinfo arrays.
 */
#define IOINFO_ARRAYS       64

#define _NHANDLE_           (IOINFO_ARRAYS * IOINFO_ARRAY_ELTS)

/*
 * Access macros for getting at an ioinfo struct and its fields from a
 * file handle
 */
#define _pioinfo(i) ( __pioinfo[(i) >> IOINFO_L2E] + ((i) & (IOINFO_ARRAY_ELTS - \
                              1)) )
#define _osfhnd(i)  ( _pioinfo(i)->osfhnd )

#define _osfile(i)  ( _pioinfo(i)->osfile )

#define _pipech(i)  ( _pioinfo(i)->pipech )

/*
 * Safer versions of the above macros. Currently, only _osfile_safe is
 * used.
 */
#define _pioinfo_safe(i)    ( ((i) != -1) ? _pioinfo(i) : &__badioinfo )

#define _osfhnd_safe(i)     ( _pioinfo_safe(i)->osfhnd )

#define _osfile_safe(i)     ( _pioinfo_safe(i)->osfile )

#define _pipech_safe(i)     ( _pioinfo_safe(i)->pipech )

/*
 * Special, static ioinfo structure used only for more graceful handling
 * of a C file handle value of -1 (results from common errors at the stdio
 * level).
 */
extern _CRTIMP ioinfo __badioinfo;

/*
 * Array of arrays of control structures for lowio files.
 */
extern _CRTIMP ioinfo * __pioinfo[];

/*
 * Current number of allocated ioinfo structures (_NHANDLE_ is the upper
 * limit).
 */
extern int _nhandle;

int __cdecl _alloc_osfhnd(void);
int __cdecl _free_osfhnd(int);
int __cdecl _set_osfhnd(int, intptr_t);


extern const char __dnames[];
extern const char __mnames[];

extern int _days[];
extern int _lpdays[];

#ifndef _TIME_T_DEFINED
#ifdef _WIN64
typedef __int64   time_t;       /* time value */
#else  /* _WIN64 */
typedef _W64 long time_t;       /* time value */
#endif  /* _WIN64 */
#define _TIME_T_DEFINED         /* avoid multiple def's of time_t */
#endif  /* _TIME_T_DEFINED */

#ifndef _TIME64_T_DEFINED
typedef __int64 __time64_t;     /* 64-bit time value */
#define _TIME64_T_DEFINED
#endif  /* _TIME64_T_DEFINED */

extern time_t __cdecl __loctotime_t(int, int, int, int, int, int, int);

extern __time64_t __cdecl __loctotime64_t(int, int, int, int, int, int, int);

#ifdef _TM_DEFINED
extern int __cdecl _isindst(struct tm *);
#endif  /* _TM_DEFINED */

extern void __cdecl __tzset(void);

extern int __cdecl _validdrive(unsigned);


/*
 * This variable is in the C start-up; the length must be kept synchronized
 * It is used by the *cenvarg.c modules
 */

extern char _acfinfo[]; /* "_C_FILE_INFO=" */

#define CFI_LENGTH  12  /* "_C_FILE_INFO" is 12 bytes long */


/* typedefs needed for subsequent prototypes */

#ifndef _SIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

#ifndef _VA_LIST_DEFINED
typedef char *  va_list;
#define _VA_LIST_DEFINED
#endif  /* _VA_LIST_DEFINED */

/*
 * stdio internals
 */
#ifndef _FILE_DEFINED
struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif  /* _FILE_DEFINED */

#if !defined (_FILEX_DEFINED) && defined (_WINDOWS_)

/*
 * Variation of FILE type used for the dynamically allocated portion of
 * __piob[]. For single thread, _FILEX is the same as FILE. For multithread
 * models, _FILEX has two fields: the FILE struct and the CRITICAL_SECTION
 * struct used to serialize access to the FILE.
 */
#ifdef _MT

typedef struct {
        FILE f;
        CRITICAL_SECTION lock;
        }   _FILEX;

#else  /* _MT */

typedef FILE    _FILEX;

#endif  /* _MT */

#define _FILEX_DEFINED
#endif  /* !defined (_FILEX_DEFINED) && defined (_WINDOWS_) */

/*
 * Number of entries supported in the array pointed to by __piob[]. That is,
 * the number of stdio-level files which may be open simultaneously. This
 * is normally set to _NSTREAM_ by the stdio initialization code.
 */
extern int _nstream;

/*
 * Pointer to the array of pointers to FILE/_FILEX structures that are used
 * to manage stdio-level files.
 */
extern void **__piob;

FILE * __cdecl _getstream(void);
FILE * __cdecl _openfile(const char *, const char *, int, FILE *);
FILE * __cdecl _wopenfile(const wchar_t *, const wchar_t *, int, FILE *);
void __cdecl _getbuf(FILE *);
int __cdecl _filwbuf (FILE *);
int __cdecl _flswbuf(int, FILE *);
void __cdecl _freebuf(FILE *);
int __cdecl _stbuf(FILE *);
void __cdecl _ftbuf(int, FILE *);
int __cdecl _output(FILE *, const char *, va_list);
int __cdecl _woutput(FILE *, const wchar_t *, va_list);
int __cdecl _input(FILE *, const unsigned char *, va_list);
int __cdecl _winput(FILE *, const wchar_t *, va_list);
int __cdecl _flush(FILE *);
void __cdecl _endstdio(void);

int __cdecl _fseeki64(FILE *, __int64, int);
int __cdecl _fseeki64_lk(FILE *, __int64, int);
__int64 __cdecl _ftelli64(FILE *);
#ifdef _MT
__int64 __cdecl _ftelli64_lk(FILE *);
#else  /* _MT */
#define _ftelli64_lk    _ftelli64
#endif  /* _MT */

#ifndef CRTDLL
extern int _cflush;
#endif  /* CRTDLL */

extern unsigned int _tempoff;

extern unsigned int _old_pfxlen;

extern int _umaskval;       /* the umask value */

extern char _pipech[];      /* pipe lookahead */

extern char _exitflag;      /* callable termination flag */

extern int _C_Termination_Done; /* termination done flag */

char * __cdecl _getpath(const char *, char *, unsigned);
wchar_t * __cdecl _wgetpath(const wchar_t *, wchar_t *, unsigned);

extern int _dowildcard;     /* flag to enable argv[] wildcard expansion */

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif  /* _PNH_DEFINED */


/* calls the currently installed new handler */
int __cdecl _callnewh(size_t);

extern int _newmode;    /* malloc new() handler mode */

/* pointer to initial environment block that is passed to [w]main */
#if defined (_DLL) && defined (_M_IX86)
#define __winitenv  (*__p___winitenv())
_CRTIMP wchar_t *** __cdecl __p___winitenv(void);
#define __initenv  (*__p___initenv())
_CRTIMP char *** __cdecl __p___initenv(void);
#else  /* defined (_DLL) && defined (_M_IX86) */
extern _CRTIMP wchar_t **__winitenv;
extern _CRTIMP char **__initenv;
#endif  /* defined (_DLL) && defined (_M_IX86) */

/* startup set values */
extern char *_aenvptr;      /* environment ptr */
extern wchar_t *_wenvptr;   /* wide environment ptr */

/* command line */

#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP char ** __cdecl __p__acmdln(void);
_CRTIMP wchar_t ** __cdecl __p__wcmdln(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
_CRTIMP extern char *_acmdln;
_CRTIMP extern wchar_t *_wcmdln;


/*
 * prototypes for internal startup functions
 */
int __cdecl _cwild(void);           /* wild.c */
int __cdecl _wcwild(void);          /* wwild.c */
#ifdef _MT
int  __cdecl _mtinit(void);         /* tidtable.c */
void __cdecl _mtterm(void);         /* tidtable.c */
int  __cdecl _mtinitlocks(void);    /* mlock.c */
void __cdecl _mtdeletelocks(void);  /* mlock.c */
int  __cdecl _mtinitlocknum(int);   /* mlock.c */
#endif  /* _MT */

#ifdef _MT
/* Wrapper for InitializeCriticalSection API, with default spin count */
int __cdecl __crtInitCritSecAndSpinCount(PCRITICAL_SECTION, DWORD);
#define _CRT_SPINCOUNT  4000
#endif  /* _MT */

/*
 * C source build only!!!!
 *
 * more prototypes for internal startup functions
 */
void __cdecl _amsg_exit(int);           /* crt0.c */
void __cdecl __crtExitProcess(int);     /* crt0dat.c */
int  __cdecl _cinit(void);              /* crt0dat.c */
void __cdecl __doinits(void);           /* astart.asm */
void __cdecl __doterms(void);           /* astart.asm */
void __cdecl __dopreterms(void);        /* astart.asm */
void __cdecl _FF_MSGBANNER(void);
void __cdecl _fptrap(void);             /* crt0fp.c */
int  __cdecl _heap_init(int);
void __cdecl _heap_term(void);
void __cdecl _heap_abort(void);
void __cdecl __initconin(void);         /* initcon.c */
void __cdecl __initconout(void);        /* initcon.c */
int  __cdecl _ioinit(void);             /* crt0.c, crtlib.c */
void __cdecl _ioterm(void);             /* crt0.c, crtlib.c */
char * __cdecl _GET_RTERRMSG(int);
void __cdecl _NMSG_WRITE(int);
int  __cdecl _setargv(void);            /* setargv.c, stdargv.c */
int  __cdecl __setargv(void);           /* stdargv.c */
int  __cdecl _wsetargv(void);           /* wsetargv.c, wstdargv.c */
int  __cdecl __wsetargv(void);          /* wstdargv.c */
int  __cdecl _setenvp(void);            /* stdenvp.c */
int  __cdecl _wsetenvp(void);           /* wstdenvp.c */
void __cdecl __setmbctable(unsigned int);   /* mbctype.c */

#ifdef _MBCS
int  __cdecl __initmbctable(void);      /* mbctype.c */
#endif  /* _MBCS */

int __cdecl main(int, char **, char **);
int __cdecl wmain(int, wchar_t **, wchar_t **);

/* helper functions for wide/multibyte environment conversion */
int __cdecl __mbtow_environ (void);
int __cdecl __wtomb_environ (void);
int __cdecl __crtsetenv (char *, const int);
int __cdecl __crtwsetenv (wchar_t *, const int);

_CRTIMP extern void (__cdecl * _aexit_rtn)(int);

#if defined (_DLL) || defined (CRTDLL)

#ifndef _STARTUP_INFO_DEFINED
typedef struct
{
        int newmode;
} _startupinfo;
#define _STARTUP_INFO_DEFINED
#endif  /* _STARTUP_INFO_DEFINED */

_CRTIMP int __cdecl __getmainargs(int *, char ***, char ***,
                                  int, _startupinfo *);

_CRTIMP int __cdecl __wgetmainargs(int *, wchar_t ***, wchar_t ***,
                                   int, _startupinfo *);

#endif  /* defined (_DLL) || defined (CRTDLL) */

/*
 * Prototype, variables and constants which determine how error messages are
 * written out.
 */
#define _UNKNOWN_APP    0
#define _CONSOLE_APP    1
#define _GUI_APP        2

extern int __app_type;

extern int __error_mode;

_CRTIMP void __cdecl __set_app_type(int);

/*
 * C source build only!!!!
 *
 * map Win32 errors into Xenix errno values -- for modules written in C
 */
extern void __cdecl _dosmaperr(unsigned long);

/*
 * internal routines used by the exec/spawn functions
 */

extern intptr_t __cdecl _dospawn(int, const char *, char *, char *);
extern intptr_t __cdecl _wdospawn(int, const wchar_t *, wchar_t *, wchar_t *);
extern int __cdecl _cenvarg(const char * const *, const char * const *,
        char **, char **, const char *);
extern int __cdecl _wcenvarg(const wchar_t * const *, const wchar_t * const *,
        wchar_t **, wchar_t **, const wchar_t *);
#ifndef _M_IX86
extern char ** _capture_argv(va_list *, const char *, char **, size_t);
extern wchar_t ** _wcapture_argv(va_list *, const wchar_t *, wchar_t **, size_t);
#endif  /* _M_IX86 */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_INTERNAL */
