//+----------------------------------------------------------------------------
//
// Microsoft Windows Media Technologies.
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//
//  File:       wmdm_ver.h
//
//  Description: This file contains common definitions for verion definitions
//               for WMDM binaries.
//+----------------------------------------------------------------------------

#include <winver.h>


#include "ntverp.h"

// WMDM version information 
// Version: 9.0.0.BUILD
// Meaning: Major.Minor.Update.Build

#define VER_BUILDVERSION_STR2(x,y) #x "." #y
#define VER_BUILDVERSION_STR1(x,y) VER_BUILDVERSION_STR2(x, y)
#define VER_BUILDVERSION_STRING   VER_BUILDVERSION_STR1(VER_PRODUCTBUILD, VER_PRODUCTBUILD_QFE)

#define VER_WMDM_PRODUCTVERSION_STR	 "9.0." VER_BUILDVERSION_STRING
#define VER_WMDM_PRODUCTVERSION	9,0,VER_PRODUCTBUILD,VER_PRODUCTBUILD_QFE


#define VER_WMDM_PRODUCTNAME_STR        "Windows Media Device Manager\0"
#define VER_WMDM_COMPANYNAME_STR        "Microsoft Corporation\0"
#define VER_WMDM_LEGALCOPYRIGHT_STR     "Copyright (C) Microsoft Corp.\0"
#define VER_WMDM_FILEOS                 VOS_NT_WINDOWS32

#ifdef EXPORT_CONTROLLED

#ifdef EXPORT
#define EXPORT_TAG  " (Export Version)\0"
#else
#define EXPORT_TAG  " (Domestic Use Only)\0"
#endif

#else           /* Not Export Controlled */

#define EXPORT_TAG

#endif
