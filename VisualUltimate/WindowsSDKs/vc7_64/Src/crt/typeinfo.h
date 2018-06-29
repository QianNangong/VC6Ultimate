/***
*typeinfo.h - Defines the type_info structure and exceptions used for RTTI
*
*       Copyright (c) 1994-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the type_info structure and exceptions used for
*       Runtime Type Identification.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif  /* __cplusplus */

#ifndef _INC_TYPEINFO
#define _INC_TYPEINFO

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

class type_info {
public:
    _CRTIMP virtual ~type_info();
    _CRTIMP int operator==(const type_info& rhs) const;
    _CRTIMP int operator!=(const type_info& rhs) const;
    _CRTIMP int before(const type_info& rhs) const;
    _CRTIMP const char* name() const;
    _CRTIMP const char* raw_name() const;
private:
    void *_m_data;
    char _m_d_name[1];
    type_info(const type_info& rhs);
    type_info& operator=(const type_info& rhs);
};

#ifndef _TICORE

// This include must occur below the definition of class type_info
#include <stdexcpt.h>

class _CRTIMP bad_cast : public exception {
public:
    bad_cast(const char * _Message = "bad cast");
    bad_cast(const bad_cast &);
    virtual ~bad_cast();
#ifndef _INTERNAL_IFSTRIP_
#ifdef CRTDLL
private:
    // This is aliased to public:bad_cast(const char * const &) to provide
    // the old, non-conformant constructor.
    bad_cast(const char * const * _Message);
#endif  /* CRTDLL */
#endif  /* _INTERNAL_IFSTRIP_ */
};

class _CRTIMP bad_typeid : public exception {
public:
    bad_typeid(const char * _Message = "bad typeid");
    bad_typeid(const bad_typeid &);
    virtual ~bad_typeid();
};

class _CRTIMP __non_rtti_object : public bad_typeid {
public:
    __non_rtti_object(const char * _Message);
    __non_rtti_object(const __non_rtti_object &);
    virtual ~__non_rtti_object();
};

#ifdef __RTTI_OLDNAMES
// Some synonyms for folks using older standard
typedef type_info Type_info;
typedef bad_cast Bad_cast;
typedef bad_typeid Bad_typeid;
#endif  /* __RTTI_OLDNAMES */

#endif  /* _TICORE */

#endif  /* _INC_TYPEINFO */
