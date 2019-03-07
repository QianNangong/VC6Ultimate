/***
*stdarg.h - defines ANSI-style macros for variable argument functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines ANSI-style macros for accessing arguments
*       of functions which take a variable number of arguments.
*       [ANSI]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STDARG
#define _INC_STDARG

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

#ifndef _UINTPTR_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    uintptr_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   uintptr_t;
#endif  /* _WIN64 */
#define _UINTPTR_T_DEFINED
#endif  /* _UINTPTR_T_DEFINED */

#ifndef _VA_LIST_DEFINED
#ifdef _M_ALPHA
typedef struct {
        char *a0;       /* pointer to first homed integer argument */
        int offset;     /* byte offset of next parameter */
} va_list;
#else  /* _M_ALPHA */
typedef char *  va_list;
#endif  /* _M_ALPHA */
#define _VA_LIST_DEFINED
#endif  /* _VA_LIST_DEFINED */

#ifdef __cplusplus
#define _ADDRESSOF(v)   ( &reinterpret_cast<const char &>(v) )
#else  /* __cplusplus */
#define _ADDRESSOF(v)   ( &(v) )
#endif  /* __cplusplus */

#if defined (_M_IA64)
#define _VA_ALIGN       8
#define _SLOTSIZEOF(t)   ( (sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1) )

#define _VA_STRUCT_ALIGN  16

#define _ALIGNOF(ap) ((((ap)+_VA_STRUCT_ALIGN - 1) & ~(_VA_STRUCT_ALIGN -1)) \
        - (ap))
#define _APALIGN(t,ap)  (__alignof(t) > 8 ? _ALIGNOF((uintptr_t) ap) : 0)

#else  /* defined (_M_IA64) */
#define _SLOTSIZEOF(t)   (sizeof(t))
#define _APALIGN(t,ap)  (__builtin_alignof(t))
#endif  /* defined (_M_IA64) */

#if defined (_M_CEE)

extern void __cdecl __va_start(va_list*, ...);
extern void * __cdecl __va_arg(va_list*, ...);
extern void __cdecl __va_end(va_list*);

#define va_start(ap,v)  ( __va_start(&ap, _ADDRESSOF(v), _SLOTSIZEOF(v), \
                                __builtin_alignof(v), _ADDRESSOF(v)) )
#define va_arg(ap,t)    ( *(t *)__va_arg(&ap, _SLOTSIZEOF(t), \
                                _APALIGN(t,ap), (t *)0) )
#define va_end(ap)      ( __va_end(&ap) )

#elif defined (_M_IX86)

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#elif defined (_M_MRX000)


/* Use these types and definitions if generating code for MIPS */

#define va_start(ap,v) ap  = (va_list)_ADDRESSOF(v) + sizeof(v)
#define va_end(list)
#define va_arg(list, mode) ((mode *)(list =\
 (char *) ((((int)list + (__builtin_alignof(mode)<=4?3:7)) &\
 (__builtin_alignof(mode)<=4?-4:-8))+sizeof(mode))))[-1]

/*  +++++++++++++++++++++++++++++++++++++++++++
    Because of parameter passing conventions in C:
    use mode=int for char, and short types
    use mode=double for float types
    use a pointer for array types
    +++++++++++++++++++++++++++++++++++++++++++ */


#elif defined (_M_ALPHA)


/* Use these types and definitions if generating code for ALPHA */

/*
 * The Alpha compiler supports two builtin functions that are used to
 * implement stdarg/varargs.  The __builtin_va_start function is used
 * by va_start to initialize the data structure that locates the next
 * argument.  The __builtin_isfloat function is used by va_arg to pick
 * which part of the home area a given register argument is stored in.
 * The home area is where up to six integer and/or six floating point
 * register arguments are stored down (so they can also be referenced
 * by a pointer like any arguments passed on the stack).
 */

extern void * __builtin_va_start(va_list, ...);

#ifdef _CFRONT
#define __builtin_isfloat(a) __builtin_alignof(a)
#endif  /* _CFRONT */

#define va_start(list, v) __builtin_va_start(list, v, 1)
#define va_end(list)
#define va_arg(list, mode) \
    ( *(        ((list).offset += ((int)sizeof(mode) + 7) & -8) , \
        (mode *)((list).a0 + (list).offset - \
                    ((__builtin_isfloat(mode) && (list).offset <= (6 * 8)) ? \
                        (6 * 8) + 8 : ((int)sizeof(mode) + 7) & -8) \
                ) \
       ) \
    )

#elif defined (_M_PPC)

/* Microsoft C8 front end (used in Motorola Merged compiler) */
/* bytes that a type occupies in the argument list */
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
/* return 'ap' adjusted for type 't' in arglist */
#define _ALIGNIT(ap,t) \
        ((((int)(ap))+(sizeof(t)<8?3:7)) & (sizeof(t)<8?~3:~7))

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap = (char *) (_ALIGNIT(ap, t) + _INTSIZEOF(t))) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#elif defined (_M_IA64)

#ifdef __cplusplus
extern void __cdecl __va_start(va_list*, ...);
#define va_start(ap,v)  ( __va_start(&ap, _ADDRESSOF(v), _SLOTSIZEOF(v), \
                                     _ADDRESSOF(v)) )
#else  /* __cplusplus */
#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _SLOTSIZEOF(v) )
#endif  /* __cplusplus */

#define va_arg(ap,t)    (*(t *)((ap += _SLOTSIZEOF(t)+ _APALIGN(t,ap)) \
                                                     -_SLOTSIZEOF(t)))

#define va_end(ap)      ( ap = (va_list)0 )

#else  /* defined (_M_IA64) */

/* A guess at the proper definitions for other platforms */

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#endif  /* defined (_M_IA64) */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_STDARG */
