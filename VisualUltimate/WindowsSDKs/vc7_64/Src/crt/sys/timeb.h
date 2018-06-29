/***
*sys/timeb.h - definition/declarations for _ftime()
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file define the _ftime() function and the types it uses.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_TIMEB
#define _INC_TIMEB

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


#ifndef _TIME_T_DEFINED
#ifdef _WIN64
typedef __int64 time_t;         /* time value */
#else  /* _WIN64 */
typedef long    time_t;         /* time value */
#endif  /* _WIN64 */
#if _INTEGRAL_MAX_BITS >= 64   
typedef __int64 __time64_t;
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#define _TIME_T_DEFINED         /* avoid multiple def's of time_t */
#endif  /* _TIME_T_DEFINED */


/* Structure returned by _ftime system call */

#ifndef _TIMEB_DEFINED
struct _timeb {
        time_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };

#if !__STDC__

/* Non-ANSI name for compatibility */

struct timeb {
        time_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };

#endif  /* !__STDC__ */

#if _INTEGRAL_MAX_BITS >= 64   
struct __timeb64 {
        __time64_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _TIMEB_DEFINED
#endif  /* _TIMEB_DEFINED */


/* Function prototypes */

_CRTIMP void __cdecl _ftime(struct _timeb *);

#if !__STDC__

/* Non-ANSI name for compatibility */

_CRTIMP void __cdecl ftime(struct timeb *);

#endif  /* !__STDC__ */

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP void __cdecl _ftime64(struct __timeb64 *);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_TIMEB */
