//***************************************************************************
//
//  WBEMSEC.H
//
//  Purpose: Provides prototypes for some security helper functions.
//
//  Copyright (c)1998 - 1999 Microsoft Corporation, All Rights Reserved
//
//***************************************************************************

#ifndef _WBEMSEC_H_
#define _WBEMSEC_H_

HRESULT InitializeSecurity(BOOL bAuthenticate);
HRESULT SetInterfaceSecurity(IUnknown * pInterface, LPWSTR pDomain, LPWSTR pUser, LPWSTR pPassword,bool bAuthenticate);
bool IsAuthenticated(IUnknown * pFrom);

#endif // _WBEMSEC_H_
