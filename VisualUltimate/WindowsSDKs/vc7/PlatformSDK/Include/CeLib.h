//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (C) Microsoft Corporation, 1995 - 1999
//
//  File:       celib.h
//
//  Contents:   helper functions
//
//--------------------------------------------------------------------------

#ifndef __CELIB_H__
#define __CELIB_H__


#define CENCODEMAX	(64 * 1024)

// Bitmap manipulation routines.  Fetch or set a bit, given a base and index.
#define GETBIT(pb, i)	((pb)[(i) / 8] & (1 << ((i) % 8)))
#define SETBIT(pb, i)	((pb)[(i) / 8] |= (1 << ((i) % 8)))
#define CLEARBIT(pb, i)	((pb)[(i) / 8] &= ~(1 << ((i) % 8)))

#define BITSTOBYTES(b)	((int)(((b) + 7) / 8))

// Size of a fixed array:
#define ARRAYSIZE(a)		(sizeof(a)/sizeof((a)[0]))

#define wszCERTENROLLSHAREPATH	L"CertSrv\\CertEnroll"

#define cwcHRESULTSTRING	40

#define SAFE_SUBTRACT_POINTERS(p1, p2) \
    (assert(sizeof((*p1)) == sizeof(*(p2))), ((p1) - (p2)))

#define _PrintErrorStr(hr, pszMessage, pwsz) \
    _PrintError((hr), (pszMessage))

#define _PrintErrorStr2(hr, pszMessage, pwsz, hr2) \
    _PrintError((hr), (pszMessage))

#define _PrintError2(hr, pszMessage, hr2) \
    _PrintError((hr), (pszMessage))

#define _PrintError(hr, pszMessage) \
	    ceERRORPRINTLINE((pszMessage), (hr))


#define _PrintIfErrorStr(hr, pszMessage, pwsz) \
    _PrintIfError((hr), (pszMessage))

#define _PrintIfErrorStr2(hr, pszMessage, pwsz, hr2) \
    _PrintIfError((hr), (pszMessage))

#define _PrintIfError2(hr, pszMessage, hr2) \
    _PrintIfError((hr), (pszMessage))

#define _PrintIfError(hr, pszMessage) \
    { \
	if (S_OK != (hr)) \
	{ \
	    ceERRORPRINTLINE((pszMessage), (hr)); \
	} \
    }


#define _JumpErrorStr(hr, label, pszMessage, pwsz) \
    _JumpError((hr), label, (pszMessage))

#define _JumpError(hr, label, pszMessage) \
    { \
	ceERRORPRINTLINE((pszMessage), (hr)); \
	goto label; \
    }


#define _JumpIfErrorStr(hr, label, pszMessage, pwsz) \
    _JumpIfError((hr), label, (pszMessage))

#define _JumpIfErrorStr2(hr, label, pszMessage, pwsz, hr2) \
    _JumpIfError((hr), label, (pszMessage))

#define _JumpIfError2(hr, label, pszMessage, hr2) \
    _JumpIfError((hr), label, (pszMessage))

#define _JumpIfError(hr, label, pszMessage) \
    { \
	if (S_OK != (hr)) \
	{ \
	    ceERRORPRINTLINE((pszMessage), (hr)); \
	    goto label; \
	} \
    }


#define ceERRORPRINTLINE(pszMessage, hr) \
	    ceErrorPrintLine(__FILE__, __LINE__, (pszMessage), (hr))

#define DBGPRINT(a)	ceDbgPrintf a

int WINAPIV ceDbgPrintf(BOOL fDebug, char const *pszfmt, ...);


VOID
ceErrorPrintLine(
    IN char const *pszFile,
    IN DWORD line,
    IN char const *pszMessage,
    IN HRESULT hr);

HRESULT
ceHLastError(VOID);

HRESULT
ceHError(
    IN HRESULT hr);


// Constants chosen to avoid DWORD overflow:

#define CVT_WEEKS	(7 * CVT_DAYS)
#define CVT_DAYS	(24 * CVT_HOURS)
#define CVT_HOURS	(60 * CVT_MINUTES)
#define CVT_MINUTES	(60 * CVT_SECONDS)
#define CVT_SECONDS	(1)
#define CVT_BASE	(1000 * 1000 * 10)


enum ENUM_PERIOD
{
    ENUM_PERIOD_INVALID = -1,
    ENUM_PERIOD_SECONDS = 0,
    ENUM_PERIOD_MINUTES,
    ENUM_PERIOD_HOURS,
    ENUM_PERIOD_DAYS,
    ENUM_PERIOD_WEEKS,
    ENUM_PERIOD_MONTHS,
    ENUM_PERIOD_YEARS
};

HRESULT
ceMakeExprDate(
    IN OUT DATE *pDate,
    IN LONG lDelta,
    IN enum ENUM_PERIOD enumPeriod);

HRESULT
ceTranslatePeriodUnits(
    IN WCHAR const *pwszPeriod,
    IN LONG lCount,
    OUT enum ENUM_PERIOD *penumPeriod,
    OUT LONG *plCount);

WCHAR const *
ceGetOIDNameA(
    IN char const *pszObjId);

WCHAR const *
ceGetOIDName(
    IN WCHAR const *pwszObjId);

BOOL
ceDecodeObject(
    IN DWORD dwEncodingType,
    IN LPCSTR lpszStructType,
    IN BYTE const *pbEncoded,
    IN DWORD cbEncoded,
    IN BOOL fCoTaskMemAlloc,
    OUT VOID **ppvStructInfo,
    OUT DWORD *pcbStructInfo);

BOOL
ceEncodeObject(
    DWORD dwEncodingType,
    IN LPCSTR lpszStructType,
    IN VOID const *pvStructInfo,
    IN DWORD dwFlags,
    IN BOOL fCoTaskMemAlloc,
    OUT BYTE **ppbEncoded,
    OUT DWORD *pcbEncoded);

WCHAR *
ceDuplicateString(
    IN WCHAR const *pwsz);

BOOL
ceConvertWszToSz(
    OUT char **ppsz,
    IN WCHAR const *pwc,
    IN LONG cb);

BOOL
ceConvertWszToBstr(
    OUT BSTR *pbstr,
    IN WCHAR const *pwc,
    IN LONG cb);

BOOL
ceConvertSzToWsz(
    OUT WCHAR **ppwsz,
    IN char const *pch,
    IN LONG cch);

BOOL
ceConvertSzToBstr(
    OUT BSTR *pbstr,
    IN CHAR const *pch,
    IN LONG cch);

VOID
ceFreeBstr(
    IN OUT BSTR *pstr);

HRESULT
ceDateToFileTime(
    IN DATE const *pDate,
    OUT FILETIME *pft);

HRESULT
ceFileTimeToDate(
    IN FILETIME const *pft,
    OUT DATE *pDate);

HRESULT
ceVerifyObjId(
    IN WCHAR const *pwszObjId);

HRESULT
ceVerifyAltNameString(
    IN LONG NameChoice,
    IN WCHAR const *pwszName);

HRESULT
ceDispatchSetErrorInfo(
    IN HRESULT hrError,
    IN WCHAR const *pwszDescription,
    OPTIONAL IN WCHAR const *pwszProgId,
    OPTIONAL IN IID const *piid);

VOID
ceInitErrorMessageText(
    IN DWORD idsUnexpected,
    IN DWORD idsUnknownErrorCode);	// L"Error %ws %ws"

WCHAR *
ceGetErrorMessageText(
    IN HRESULT hr,
    IN BOOL fHResultString);

WCHAR const *
ceHResultToString(
    IN OUT WCHAR *awchr,
    IN HRESULT hr);


#define cwcFILENAMESUFFIXMAX		20
#define cwcSUFFIXMAX	(1 + 5 + 1)	// five decimal digits plus parentheses

#define wszFCSAPARM_SERVERDNSNAME	L"%1"
#define wszFCSAPARM_SERVERSHORTNAME	L"%2"
#define wszFCSAPARM_SANITIZEDCANAME	L"%3"
#define wszFCSAPARM_CERTFILENAMESUFFIX	L"%4"
#define wszFCSAPARM_DOMAINDN		L"%5"
#define wszFCSAPARM_CONFIGDN		L"%6"
#define wszFCSAPARM_SANITIZEDCANAMEHASH	L"%7"
#define wszFCSAPARM_CRLFILENAMESUFFIX	L"%8"
#define wszFCSAPARM_CRLDELTAFILENAMESUFFIX L"%9"
#define wszFCSAPARM_DSCRLATTRIBUTE	L"%10"
#define wszFCSAPARM_DSCACERTATTRIBUTE	L"%11"


HRESULT
ceFormatCertsrvStringArray(
    IN BOOL fURL,
    IN LPCWSTR pwszServerName_p1_2,
    IN LPCWSTR pwszSanitizedName_p3_7,
    IN DWORD   iCert_p4,
    IN LPCWSTR pwszDomainDN_p5,
    IN LPCWSTR pwszConfigDN_p6,
    IN DWORD   iCRL_p8,
    IN BOOL    fDeltaCRL_p9,
    IN BOOL    fDSAttrib_p10_11,
    IN DWORD   cStrings,
    IN LPCWSTR *apwszStringsIn,
    OUT LPWSTR *apwszStringsOut);

HRESULT
ceInternetCanonicalizeUrl(
    IN WCHAR const *pwszIn,
    OUT WCHAR **ppwszOut);

int 
ceWtoI(
    IN WCHAR const *pwszDigitString,
    OUT BOOL *pfValid);

#endif // __CELIB_H__
