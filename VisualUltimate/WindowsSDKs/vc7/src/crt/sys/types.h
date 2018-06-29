/***
*sys/types.h - types returned by system level calls for file and time info
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines types used in defining values returned by system
*       level calls for file status and time information.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_TYPES
#define _INC_TYPES

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


#ifndef _INO_T_DEFINED

typedef unsigned short _ino_t;          /* i-node number (not used on DOS) */

#if !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned short ino_t;
#endif  /* !__STDC__ */

#define _INO_T_DEFINED
#endif  /* _INO_T_DEFINED */


#ifndef _DEV_T_DEFINED

typedef unsigned int _dev_t;            /* device code */

#if !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned int dev_t;
#endif  /* !__STDC__ */

#define _DEV_T_DEFINED
#endif  /* _DEV_T_DEFINED */


#ifndef _OFF_T_DEFINED

typedef long _off_t;                    /* file offset value */

#if !__STDC__
/* Non-ANSI name for compatibility */
typedef long off_t;
#endif  /* !__STDC__ */

#define _OFF_T_DEFINED
#endif  /* _OFF_T_DEFINED */

#endif  /* _INC_TYPES */
