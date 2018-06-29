// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

// AfxIsValid... function are only defined in DEBUG -- they 
// should not be used in retail mode as they call IsBadXXXPtr, bad API

// AfxIsValidString() returns TRUE if the passed pointer
// references a string of at least the given length in characters.
// A length of -1 (the default parameter) means that the string
// buffer's minimum length isn't known, and the function will
// return TRUE no matter how long the string is. The memory
// used by the string can be read-only.

BOOL AFXAPI AfxIsValidString(LPCWSTR lpsz, int nLength /* = -1 */)
{
#if defined(_DEBUG)
	if (lpsz == NULL)
		return FALSE;
	return afxData.bWin95 || ::IsBadStringPtrW(lpsz, nLength) == 0;
#else
	nLength;
	return lpsz != NULL;
#endif
}

// As above, but for ANSI strings.

BOOL AFXAPI AfxIsValidString(LPCSTR lpsz, int nLength /* = -1 */)
{
#if defined(_DEBUG)
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtrA(lpsz, nLength) == 0;
#else
	nLength;
	return lpsz != NULL;
#endif
}

// AfxIsValidAddress() returns TRUE if the passed parameter points
// to at least nBytes of accessible memory. If bReadWrite is TRUE,
// the memory must be writeable; if bReadWrite is FALSE, the memory
// may be const.

BOOL AFXAPI AfxIsValidAddress(const void* lp, UINT_PTR nBytes,
	BOOL bReadWrite /* = TRUE */)
{
#if defined(_DEBUG)
	// simple version using Win-32 APIs for pointer validation.
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
#else
	nBytes; bReadWrite;
	return lp != NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////