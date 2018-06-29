/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    pnrpns.h

Abstract:
    The PNRP interface participates as a WinSock 2.0 Name Space Provider (NSP)
    The actual functions are defined in winsock2.h of the platform SDK

    This header file defines the specific structures and data used for PNRP.

--*/

#pragma once

#include "pnrpdef.h"

// NSP Provider GUID
DEFINE_GUID(NS_PROVIDER_PNRPNAME,   0x03fe89cd, 0x766d, 0x4976, 0xb9, 0xc1, 0xbb, 0x9b, 0xc4, 0x2c, 0x7b, 0x4d);
DEFINE_GUID(NS_PROVIDER_PNRPCLOUD,  0x03fe89ce, 0x766d, 0x4976, 0xb9, 0xc1, 0xbb, 0x9b, 0xc4, 0x2c, 0x7b, 0x4d);

// Service Class GUID (specific to PNRP)
DEFINE_GUID(SVCID_PNRPNAME,     0xc2239ce5, 0x00c0, 0x4fbf, 0xba, 0xd6, 0x18, 0x13, 0x93, 0x85, 0xa4, 0x9a);
DEFINE_GUID(SVCID_PNRPCLOUD,    0xc2239ce6, 0x00c0, 0x4fbf, 0xba, 0xd6, 0x18, 0x13, 0x93, 0x85, 0xa4, 0x9a);


// Namespaces
#ifndef NS_PNRPNAME
#define NS_PNRPNAME     (38) 
#endif

#ifndef NS_PNRPCLOUD
#define NS_PNRPCLOUD    (39)
#endif

//
// Bit values used for dwFlags in PNRPINFO
//

#define PNRPINFO_HINT           0x00000001      // set if Hint valid

//
// PNRP specific data that is referenced by the lpBlob pointer of WSAQUERYSET
//

typedef struct _PNRPINFO
{
    DWORD                       dwSize;                 // size of this struct
    LPWSTR                      lpwszIdentity;          // identity name string
    DWORD                       nMaxResolve;            // number of desired resolutions
    DWORD                       dwTimeout;              // time in seconds to wait for responses
    DWORD                       dwLifetime;             // time in seconds for validity
    PNRP_RESOLVE_CRITERIA       enResolveCriteria;      // type of matching for resolves
    DWORD                       dwFlags;                // set of flags
    SOCKET_ADDRESS              saHint;                 // service location part of an ID
    PNRP_REGISTERED_ID_STATE    enNameState;            // state of registered name
} PNRPINFO, * PPNRPINFO;

//
// Cloud specific data that is referenced by the lpBlob pointer of WSAQUERYSET
//

typedef struct _PNRPCLOUDINFO
{
    DWORD                       dwSize;                 // size of this struct
    PNRP_CLOUD_ID               Cloud;                  // network cloud information
    PNRP_CLOUD_STATE            enCloudState;           // state of cloud
    PNRP_CLOUD_FLAGS            enCloudFlags;           // flags for cloud
                                                        //   PNRP_CLOUD_NAME_LOCAL - not a network name
} PNRPCLOUDINFO, * PPNRPCLOUDINFO;
