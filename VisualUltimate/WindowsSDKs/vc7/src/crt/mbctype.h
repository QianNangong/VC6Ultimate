/***
*mbctype.h - MBCS character conversion macros
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines macros for MBCS character classification/conversion.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MBCTYPE
#define _INC_MBCTYPE

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

/* include the standard ctype.h header file */

#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


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


/*
 * MBCS - Multi-Byte Character Set
 */

/*
 * This declaration allows the user access the _mbctype[] look-up array.
 */
#ifndef _INTERNAL_IFSTRIP_
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP unsigned char * __cdecl __p__mbctype(void);
_CRTIMP unsigned char * __cdecl __p__mbcasemap(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */
_CRTIMP extern unsigned char _mbctype[];
_CRTIMP extern unsigned char _mbcasemap[];

#ifndef _INTERNAL_IFSTRIP_
#ifdef _MT
#ifndef _THREADMBCINFO
typedef struct threadmbcinfostruct {
        int refcount;
        int mbcodepage;
        int ismbcodepage;
        int mblcid;
        unsigned short mbulinfo[6];
        char mbctype[257];
        char mbcasemap[256];
} threadmbcinfo;
typedef threadmbcinfo * pthreadmbcinfo;
#define _THREADMBCINFO
#endif  /* _THREADMBCINFO */
extern pthreadmbcinfo __ptmbcinfo;
pthreadmbcinfo __cdecl __updatetmbcinfo(void);
#endif  /* _MT */
#endif  /* _INTERNAL_IFSTRIP_ */

/* bit masks for MBCS character types */

#define _MS     0x01    /* MBCS single-byte symbol */
#define _MP     0x02    /* MBCS punct */
#define _M1     0x04    /* MBCS 1st (lead) byte */
#define _M2     0x08    /* MBCS 2nd byte*/

#define _SBUP   0x10    /* SBCS upper char */
#define _SBLOW  0x20    /* SBCS lower char */

/* byte types  */

#define _MBC_SINGLE     0       /* valid single byte char */
#define _MBC_LEAD       1       /* lead byte */
#define _MBC_TRAIL      2       /* trailing byte */
#define _MBC_ILLEGAL    (-1)    /* illegal byte */

#define _KANJI_CP   932

/* _setmbcp parameter defines */
#define _MB_CP_SBCS     0
#define _MB_CP_OEM      -2
#define _MB_CP_ANSI     -3
#define _MB_CP_LOCALE   -4


#ifndef _MBCTYPE_DEFINED

/* MB control routines */

_CRTIMP int __cdecl _setmbcp(int);
_CRTIMP int __cdecl _getmbcp(void);


/* MBCS character classification function prototypes */


/* byte routines */
_CRTIMP int __cdecl _ismbbkalnum( unsigned int );
_CRTIMP int __cdecl _ismbbkana( unsigned int );
_CRTIMP int __cdecl _ismbbkpunct( unsigned int );
_CRTIMP int __cdecl _ismbbkprint( unsigned int );
_CRTIMP int __cdecl _ismbbalpha( unsigned int );
_CRTIMP int __cdecl _ismbbpunct( unsigned int );
_CRTIMP int __cdecl _ismbbalnum( unsigned int );
_CRTIMP int __cdecl _ismbbprint( unsigned int );
_CRTIMP int __cdecl _ismbbgraph( unsigned int );

#ifndef _MBLEADTRAIL_DEFINED
_CRTIMP int __cdecl _ismbblead( unsigned int );
_CRTIMP int __cdecl _ismbbtrail( unsigned int );
_CRTIMP int __cdecl _ismbslead( const unsigned char *, const unsigned char *);
_CRTIMP int __cdecl _ismbstrail( const unsigned char *, const unsigned char *);
#ifndef _INTERNAL_IFSTRIP_
#ifdef _MT
_CRTIMP int __cdecl __ismbslead_mt(pthreadmbcinfo, const unsigned char *,
                                   const unsigned char *);
#endif  /* _MT */
#endif  /* _INTERNAL_IFSTRIP_ */
#define _MBLEADTRAIL_DEFINED
#endif  /* _MBLEADTRAIL_DEFINED */

#define _MBCTYPE_DEFINED
#endif  /* _MBCTYPE_DEFINED */

/*
 * char byte classification macros
 */

#define _ismbbkalnum(_c)    ((_mbctype+1)[(unsigned char)(_c)] & _MS)
#define _ismbbkprint(_c)    ((_mbctype+1)[(unsigned char)(_c)] & (_MS|_MP))
#define _ismbbkpunct(_c)    ((_mbctype+1)[(unsigned char)(_c)] & _MP)

#define _ismbbalnum(_c) (((_ctype+1)[(unsigned char)(_c)] & (_ALPHA|_DIGIT))||_ismbbkalnum(_c))
#define _ismbbalpha(_c) (((_ctype+1)[(unsigned char)(_c)] & (_ALPHA))||_ismbbkalnum(_c))
#define _ismbbgraph(_c) (((_ctype+1)[(unsigned char)(_c)] & (_PUNCT|_ALPHA|_DIGIT))||_ismbbkprint(_c))
#define _ismbbprint(_c) (((_ctype+1)[(unsigned char)(_c)] & (_BLANK|_PUNCT|_ALPHA|_DIGIT))||_ismbbkprint(_c))
#define _ismbbpunct(_c) (((_ctype+1)[(unsigned char)(_c)] & _PUNCT)||_ismbbkpunct(_c))

#define _ismbblead(_c)  ((_mbctype+1)[(unsigned char)(_c)] & _M1)
#define _ismbbtrail(_c) ((_mbctype+1)[(unsigned char)(_c)] & _M2)

#define _ismbbkana(_c)  ((_mbctype+1)[(unsigned char)(_c)] & (_MS|_MP))

#ifndef _INTERNAL_IFSTRIP_
#ifdef _MT
#define __ismbbalnum_mt(p, _c)  (((_ctype+1)[(unsigned char)(_c)] & \
                                (_ALPHA|_DIGIT)) || \
                                ((p->mbctype+1)[(unsigned char)(_c)] & _MS))
#define __ismbbalpha_mt(p, _c)  (((_ctype+1)[(unsigned char)(_c)] & \
                                (_ALPHA)) || \
                                ((p->mbctype+1)[(unsigned char)(_c)] & _MS))
#define __ismbbgraph_mt(p, _c)  (((_ctype+1)[(unsigned char)(_c)] & \
                                (_PUNCT|_ALPHA|_DIGIT)) || \
                                ((p->mbctype+1)[(unsigned char)(_c)] & (_MS|_MP)))
#define __ismbbprint_mt(p, _c)  (((_ctype + 1)[(unsigned char)(_c)] & \
                                (_BLANK|_PUNCT|_ALPHA|_DIGIT)) || \
                                ((p->mbctype + 1)[(unsigned char)(_c)] & (_MS|_MP)))
#define __ismbbpunct_mt(p, _c)  (((_ctype+1)[(unsigned char)(_c)] & _PUNCT) || \
                                ((p->mbctype+1)[(unsigned char)(_c)] & _MP))
#define __ismbblead_mt(p, _c)   ((p->mbctype + 1)[(unsigned char)(_c)] & _M1)
#define __ismbbtrail_mt(p, _c)  ((p->mbctype + 1)[(unsigned char)(_c)] & _M2)
#endif  /* _MT */
#endif  /* _INTERNAL_IFSTRIP_ */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_MBCTYPE */
