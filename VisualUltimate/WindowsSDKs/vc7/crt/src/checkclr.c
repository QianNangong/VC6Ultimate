/***
* checkclr.c - Ensure CLR version will work with our compiled code
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Ensure correct CLR version
*
*******************************************************************************/
#include <windows.h>
#include "stdlib.h"
#include "stddef.h"
#include "crtdbg.h"
#include "internal.h"
#include "rterr.h"

#ifdef _M_IX86
void _check_commonlanguageruntime_version(void);

/*
This function pointer will be initialised only when the linker forces this module to be present
*/

/* This section comes first among static inits */
#define VERY_EARLY_STATIC_INIT ".CRT$XCA0"

#pragma section(VERY_EARLY_STATIC_INIT)

__declspec(allocate(VERY_EARLY_STATIC_INIT)) void (__cdecl *_pfn_check_commonlanguageruntime_version)(void) = &_check_commonlanguageruntime_version;

/*
Function pointer prototype for GetCORSystemDirectory
*/
typedef HRESULT (WINAPI *PFNGETCORSYSTEMDIRECTORY)
(
    LPWSTR /* pbuffer */,
    DWORD  /* cchBuffer */,
    DWORD* /* dwLength */
);

/*
Function pointer prototype for GetFileVersion
*/
typedef HRESULT (WINAPI *PFNGETFILEVERSION)
(
    LPCWSTR /* szFilename */,
    LPWSTR /* szBuffer */,
    DWORD /* cchBuffer */,
    DWORD* /* dwLength */
);

static const wchar_t rgwchMsCORLib[]=L"mscorlib.dll";

/***
*_check_commonlanguageruntime_version
*
*Purpose:
*       Ensure we are compatible with the common language runtime version in use
*
*       VS .NET 7.0 used the initial appdomain for all calls between managed and unmanaged
*       code. This was inappropriate and undesirable in the presence of multiple appdomains
*       or the destruction of the initial appdomain
*
*       VS .NET 7.1 compiles code differently by default, using the current appdomain instead
*       of the initial one. However, this new managed code is not compatible with the 1.0
*       CLR, so we have to perform version checking here.
*
*       This function does its conversion manually because the crt is not initialised yet. We
*       do allow ourselves to use init free stuff like wcscat, wcslen.
*
*Entry:
*       During CRT startup before any user code has been run.
*
*Exit:
*       TRUE for success
*
*Exceptions:
*       AMSG_EXIT.
*
*******************************************************************************/

void _check_commonlanguageruntime_version()
{
    HMODULE hmMSCOREE=NULL;                                 /* module handle of the CLR EE */
    PFNGETCORSYSTEMDIRECTORY pfnGetCORSystemDirectory=NULL; /* The system dir function */
    PFNGETFILEVERSION pfnGetFileVersion=NULL;               /* The version function */
    WCHAR rgwchCORVersion[200]=L"";                         /* Way larger than the version string should ever get */
    DWORD dwVersionCharCount=0;                             /* returned count */
    HRESULT hr=S_OK;
    WCHAR *pwchCurrent=NULL;                                /* Current parse into version string */
    WCHAR rgwchSystemDirectory[_MAX_PATH]=L"";              /* COR System dir output */
    DWORD dwSystemDirectoryLength=0;
    size_t iDllNameLen=0;

    hmMSCOREE=GetModuleHandle("mscoree.dll");
    if(hmMSCOREE==NULL || hmMSCOREE==INVALID_HANDLE_VALUE)
    {
        /*
        MSCOREE not loaded into process at this time, so we can't preemptively catch this
        */
        return;
    }

    /*
    Get some functions we need to call without static linking
    */
    pfnGetCORSystemDirectory=(PFNGETCORSYSTEMDIRECTORY)(GetProcAddress(hmMSCOREE,"GetCORSystemDirectory"));
    if(pfnGetCORSystemDirectory!=NULL)
    {
        pfnGetFileVersion=(PFNGETFILEVERSION)(GetProcAddress(hmMSCOREE,"GetFileVersion"));
        if(pfnGetFileVersion!=NULL)
        {
            hr=pfnGetCORSystemDirectory(rgwchSystemDirectory,
                                        sizeof(rgwchSystemDirectory)/sizeof(rgwchSystemDirectory[0]),
                                        &dwSystemDirectoryLength);

            if( !SUCCEEDED(hr) ||
                dwSystemDirectoryLength==0 ||
                dwSystemDirectoryLength>=(sizeof(rgwchSystemDirectory)/sizeof(rgwchSystemDirectory[0])) ||
                rgwchSystemDirectory[0]==L'\0')
            {
                return;
            }

            /* ensure termination */
            rgwchSystemDirectory[dwSystemDirectoryLength]='\0';

            /* ensure we find the first terminator */
            dwSystemDirectoryLength=(DWORD)wcslen(rgwchSystemDirectory);

            /* ensure slash */
            if(rgwchSystemDirectory[dwSystemDirectoryLength-1]!=L'\\')
            {
                if(dwSystemDirectoryLength+1==(sizeof(rgwchSystemDirectory)/sizeof(rgwchSystemDirectory[0])))
                {
                    return;
                }
                rgwchSystemDirectory[dwSystemDirectoryLength]=L'\\';
                ++dwSystemDirectoryLength;
                rgwchSystemDirectory[dwSystemDirectoryLength]=L'\0';
            }

            iDllNameLen=wcslen(rgwchSystemDirectory)+wcslen(rgwchMsCORLib)+1;

            if(iDllNameLen>=(sizeof(rgwchSystemDirectory)/sizeof(rgwchSystemDirectory[0])))
            {
                return;
            }

            wcscat(rgwchSystemDirectory, rgwchMsCORLib);

            hr=pfnGetFileVersion(   rgwchSystemDirectory,
                                    rgwchCORVersion,
                                    sizeof(rgwchCORVersion)/sizeof(rgwchCORVersion[0]),
                                    &dwVersionCharCount);

            if( !SUCCEEDED(hr) ||
                dwVersionCharCount==0 ||
                dwVersionCharCount >= sizeof(rgwchCORVersion)/sizeof(rgwchCORVersion[0]) ||
                rgwchCORVersion[0]==L'\0')
            {
                return;
            }

            /*
            Ensure string is terminated
            */
            rgwchCORVersion[dwVersionCharCount]='\0';

                        /* ensure we find the first terminator */
                        dwVersionCharCount=(DWORD)wcslen(rgwchCORVersion);

            /*
            Parse out the version info to confirm it is safe

            This is a pretty simple and tolerant parser.

            Any parsing error is taken to mean success, since we assume all newer runtimes will
            be compatible with this mode.
            */
            pwchCurrent=rgwchCORVersion;

            /* first skip the leading v and any leading zeros */
            while(*pwchCurrent && (*pwchCurrent==' ' || *pwchCurrent=='v' || *pwchCurrent=='V' || *pwchCurrent=='0'))
            {
                ++pwchCurrent;
            }

            if(!*pwchCurrent)
            {
                /*
                string terminated before we found any numbers at all, so assume success
                */
                return;
            }

            /*
            This is something we cannot understand as a number at all, so assume it is ok

            Slightly unsatisfying, but catches the case where a dud version string has arisen.
            */
            if(*pwchCurrent<'0' || *pwchCurrent>'9')
            {
                return;
            }

            /* First digit of ver that will fail is 1 */
            if(*pwchCurrent!='1')
            {
                return;
            }

            if(!*pwchCurrent)
            {
                /*
                string terminated before we found any numbers at all, so assume success
                */
                return;
            }
            else
            {
                ++pwchCurrent;
            }

            /* First digit of ver that will fail is 1. */
            if(*pwchCurrent!='.')
            {
                return;
            }

            if(!*pwchCurrent)
            {
                /*
                string terminated before we found any numbers at all, so assume success
                */
                return;
            }
            else
            {
                ++pwchCurrent;
            }

            while(*pwchCurrent && (*pwchCurrent==' ' || *pwchCurrent=='.'))
            {
                ++pwchCurrent;
            }

            if(!*pwchCurrent)
            {
                /*
                string terminated before we found any numbers at all, so assume success
                */
                return;
            }

            /*
            Skip any leading zeros
            */
            while(*pwchCurrent && *pwchCurrent=='0')
            {
                ++pwchCurrent;
            }

            if(!*pwchCurrent)
            {
                /*
                string terminated before we found any numbers at all, so assume success
                */
                return;
            }

            /*
            This is the moment we've all been waiting for
            */
            if(*pwchCurrent>='0' && *pwchCurrent <='9')
            {
                /*
                1.00<etc>001 or later, hence not 1.0
                */
                return;
            }
        }
        else
        {
            /*
            MSCOREE loaded, supports GetCORSystemDirectory, but not GetFileVersion. Very likely to be RTM 1.0.
            So error
            */
        }
    }
    else
    {
        /*
        MSCOREE loaded, but doesn't support GetCORSystemDirectory. Assume this means a very old EE, and
        hence that we should warn about the problem.
        */
    }


    /* Give the developer Some idea of what has caused the problem */
    OutputDebugString("Load failed due to incompatible .NET Runtime version");

    /* we exit here even if we are a DLL, since this is pretty fatal */
    _amsg_exit(_RT_BADCLRVERSION);
    /* never returns */
    _ASSERTE(("Unexpected return from _amsg_exit",FALSE));
}

#endif  /* _M_IX86 */
