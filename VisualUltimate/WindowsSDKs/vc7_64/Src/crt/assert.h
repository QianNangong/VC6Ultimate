/***
*assert.h - define the assert macro
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the assert(exp) macro.
*       [ANSI/System V]
*
*       [Public]
*
****/

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

#ifndef _INTERNAL_IFSTRIP_
#ifndef _ASSERT_OK
#error assert.h not for CRT internal use, use dbgint.h
#endif  /* _ASSERT_OK */
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

#undef  assert

#ifdef NDEBUG

#define assert(exp)     ((void)0)

#else  /* NDEBUG */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

_CRTIMP void __cdecl _assert(void *, void *, unsigned);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )

#endif  /* NDEBUG */
