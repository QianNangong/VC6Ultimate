/***
*sys/stat.h - defines structure used by stat() and fstat()
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the structure used by the _stat() and _fstat()
*       routines.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STAT
#define _INC_STAT

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */


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


/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */


#include <sys/types.h>


#ifndef _TIME_T_DEFINED
#ifdef _WIN64
typedef __int64 time_t;         /* time value */
#else  /* _WIN64 */
typedef long    time_t;         /* time value */
#endif  /* _WIN64 */
#if _INTEGRAL_MAX_BITS >= 64   
typedef __int64 __time64_t;     /* 64-bit time value */
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#define _TIME_T_DEFINED         /* avoid multiple def's of time_t */
#endif  /* _TIME_T_DEFINED */


#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */


/* define structure for returning status information */

#ifndef _STAT_DEFINED

struct _stat {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        _off_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#if !__STDC__

/* Non-ANSI names for compatibility */

struct stat {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        _off_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#endif  /* !__STDC__ */

#if _INTEGRAL_MAX_BITS >= 64   

struct _stati64 {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        __int64 st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

struct __stat64 {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        __int64 st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
        };

#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _STAT_DEFINED
#endif  /* _STAT_DEFINED */


#define _S_IFMT         0170000         /* file type mask */
#define _S_IFDIR        0040000         /* directory */
#define _S_IFCHR        0020000         /* character special */
#define _S_IFIFO        0010000         /* pipe */
#define _S_IFREG        0100000         /* regular */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define _S_IEXEC        0000100         /* execute/search permission, owner */


/* Function prototypes */

_CRTIMP int __cdecl _fstat(int, struct _stat *);
_CRTIMP int __cdecl _stat(const char *, struct _stat *);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP int __cdecl _fstati64(int, struct _stati64 *);
_CRTIMP int __cdecl _fstat64(int, struct __stat64 *);
_CRTIMP int __cdecl _stati64(const char *, struct _stati64 *);
_CRTIMP int __cdecl _stat64(const char *, struct __stat64 *);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifndef _WSTAT_DEFINED

/* wide function prototypes, also declared in wchar.h  */

_CRTIMP int __cdecl _wstat(const wchar_t *, struct _stat *);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP int __cdecl _wstati64(const wchar_t *, struct _stati64 *);
_CRTIMP int __cdecl _wstat64(const wchar_t *, struct __stat64 *);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _WSTAT_DEFINED
#endif  /* _WSTAT_DEFINED */


#if !__STDC__

/* Non-ANSI names for compatibility */

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

_CRTIMP int __cdecl fstat(int, struct stat *);
_CRTIMP int __cdecl stat(const char *, struct stat *);

#endif  /* !__STDC__ */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_STAT */
