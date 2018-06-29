/***
*malloc.h - declarations and definitions for memory allocation functions
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the function declarations for memory allocation functions;
*       also defines manifest constants and types used by the heap routines.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MALLOC
#define _INC_MALLOC

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
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,8)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */

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

/* Define _CRTNOALIAS, _CRTRESTRICT */

#if _MSC_FULL_VER >= 13102050

#ifndef _CRTNOALIAS
#ifndef _INTERNAL_IFSTRIP_
#define _CRTNOALIAS
#else  /* _INTERNAL_IFSTRIP_ */
#define _CRTNOALIAS __declspec(noalias)
#endif  /* _INTERNAL_IFSTRIP_ */
#endif  /* _CRTNOALIAS */

#ifndef _CRTRESTRICT
#ifndef _INTERNAL_IFSTRIP_
#define _CRTRESTRICT
#else  /* _INTERNAL_IFSTRIP_ */
#define _CRTRESTRICT __declspec(restrict)
#endif  /* _INTERNAL_IFSTRIP_ */
#endif  /* _CRTRESTRICT */

#else  /* _MSC_FULL_VER >= 13102050 */

#ifndef _CRTNOALIAS
#define _CRTNOALIAS
#endif  /* _CRTNOALIAS */

#ifndef _CRTRESTRICT
#define _CRTRESTRICT
#endif  /* _CRTRESTRICT */

#endif  /* _MSC_FULL_VER >= 13102050 */

/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */


#ifndef _SIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

#ifndef _INTPTR_T_DEFINED
#ifdef _WIN64
typedef __int64             intptr_t;
#else  /* _WIN64 */
typedef _W64 int            intptr_t;
#endif  /* _WIN64 */
#define _INTPTR_T_DEFINED
#endif  /* _INTPTR_T_DEFINED */


/* Maximum heap request the heap manager will attempt */

#ifdef _WIN64
#define _HEAP_MAXREQ    0xFFFFFFFFFFFFFFE0
#else  /* _WIN64 */
#define _HEAP_MAXREQ    0xFFFFFFE0
#endif  /* _WIN64 */

/* Constants for _heapchk/_heapset/_heapwalk routines */

#define _HEAPEMPTY      (-1)
#define _HEAPOK         (-2)
#define _HEAPBADBEGIN   (-3)
#define _HEAPBADNODE    (-4)
#define _HEAPEND        (-5)
#define _HEAPBADPTR     (-6)
#define _FREEENTRY      0
#define _USEDENTRY      1

#ifndef _HEAPINFO_DEFINED
typedef struct _heapinfo {
        int * _pentry;
        size_t _size;
        int _useflag;
        } _HEAPINFO;
#define _HEAPINFO_DEFINED
#endif  /* _HEAPINFO_DEFINED */

/* External variable declarations */
#ifndef _INTERNAL_IFSTRIP_
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP unsigned int * __cdecl __p__amblksiz(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */

extern unsigned int _amblksiz;

#define _mm_free(a)      _aligned_free(a)
#define _mm_malloc(a, b)    _aligned_malloc(a, b)

/* Function prototypes */

_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl calloc(size_t, size_t);
_CRTIMP _CRTNOALIAS              void    __cdecl free(void *);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl malloc(size_t);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl realloc(void *, size_t);
_CRTIMP _CRTNOALIAS              void    __cdecl _aligned_free(void *);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl _aligned_malloc(size_t, size_t);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl _aligned_offset_malloc(size_t, size_t, size_t);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl _aligned_realloc(void *, size_t, size_t);
_CRTIMP _CRTNOALIAS _CRTRESTRICT void *  __cdecl _aligned_offset_realloc(void *, size_t, size_t, size_t);
_CRTIMP int     __cdecl _resetstkoflw (void);


void *          __cdecl _alloca(size_t);
_CRTIMP void *  __cdecl _expand(void *, size_t);
_CRTIMP size_t  __cdecl _get_sbh_threshold(void);
_CRTIMP int     __cdecl _set_sbh_threshold(size_t);
_CRTIMP int     __cdecl _heapadd(void *, size_t);
_CRTIMP int     __cdecl _heapchk(void);
_CRTIMP int     __cdecl _heapmin(void);
_CRTIMP int     __cdecl _heapset(unsigned int);
_CRTIMP int     __cdecl _heapwalk(_HEAPINFO *);
_CRTIMP size_t  __cdecl _heapused(size_t *, size_t *);
_CRTIMP size_t  __cdecl _msize(void *);

_CRTIMP intptr_t __cdecl _get_heap_handle(void);

#if !__STDC__
/* Non-ANSI names for compatibility */
#define alloca  _alloca
#endif  /* !__STDC__ */




#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_MALLOC */
