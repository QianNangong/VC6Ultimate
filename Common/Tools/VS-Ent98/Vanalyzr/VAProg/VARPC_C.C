/* this ALWAYS GENERATED file contains the RPC client stubs */


/* File created by MIDL compiler version 3.03.0110 */
/* at Mon May 25 22:01:56 1998
 */
/* Compiler settings for varpc.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#if defined( _ALPHA_ )
#include <stdarg.h>
#endif

#include "varpc.h"

#define TYPE_FORMAT_STRING_SIZE   39                                
#define PROC_FORMAT_STRING_SIZE   409                               

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;

/* Standard interface: IRPC_MicrosoftVisualStudioAnalyzer, ver. 1.0,
   GUID={0x6c736d30,0xe80d,0x11d0,{0x96,0xe7,0x00,0xa0,0xc9,0x0f,0x27,0xe2}} */

handle_t VsaRpcl_IfHandle;


static const RPC_CLIENT_INTERFACE IRPC_MicrosoftVisualStudioAnalyzer___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x6c736d30,0xe80d,0x11d0,{0x96,0xe7,0x00,0xa0,0xc9,0x0f,0x27,0xe2}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE IRPC_MicrosoftVisualStudioAnalyzer_v1_0_c_ifspec = (RPC_IF_HANDLE)& IRPC_MicrosoftVisualStudioAnalyzer___RpcClientInterface;

extern const MIDL_STUB_DESC IRPC_MicrosoftVisualStudioAnalyzer_StubDesc;

static RPC_BINDING_HANDLE IRPC_MicrosoftVisualStudioAnalyzer__MIDL_AutoBindHandle;


long VSAOpenConnection( 
    /* [out] */ long __RPC_FAR *vaaId)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,vaaId);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  ( unsigned char __RPC_FAR * )&vaaId);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSACloseConnection( 
    /* [in] */ long vaaId)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,vaaId);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[24],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[24],
                  ( unsigned char __RPC_FAR * )&vaaId);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[24],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSABeginSession( 
    /* [in] */ long vaaId,
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *strSessionName)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,strSessionName);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[48],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[48],
                  ( unsigned char __RPC_FAR * )&vaaId,
                  ( unsigned char __RPC_FAR * )&guidSourceID,
                  ( unsigned char __RPC_FAR * )&strSessionName);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[48],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSAEndSession( 
    /* [in] */ long vaaId)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,vaaId);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[84],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[84],
                  ( unsigned char __RPC_FAR * )&vaaId);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[84],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSAIsActive( 
    /* [in] */ long vaaId)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,vaaId);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[108],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[108],
                  ( unsigned char __RPC_FAR * )&vaaId);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[108],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSAFireEvent( 
    /* [in] */ long vaaId,
    /* [string][in] */ wchar_t __RPC_FAR *guidEvent,
    /* [in] */ long prgCount,
    /* [size_is][string][in] */ wchar_t __RPC_FAR *__RPC_FAR prgKeys[  ],
    /* [size_is][string][in] */ wchar_t __RPC_FAR *__RPC_FAR prgValues[  ],
    /* [in] */ long dwTimeLow,
    /* [in] */ long dwTimeHigh,
    /* [in] */ VSAEventFlags dwFlags)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,dwFlags);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[132],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[132],
                  ( unsigned char __RPC_FAR * )&vaaId,
                  ( unsigned char __RPC_FAR * )&guidEvent,
                  ( unsigned char __RPC_FAR * )&prgCount,
                  ( unsigned char __RPC_FAR * )&prgKeys,
                  ( unsigned char __RPC_FAR * )&prgValues,
                  ( unsigned char __RPC_FAR * )&dwTimeLow,
                  ( unsigned char __RPC_FAR * )&dwTimeHigh,
                  ( unsigned char __RPC_FAR * )&dwFlags);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[132],
                  ( unsigned char __RPC_FAR * )&vaaId);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSARegisterSource( 
    /* [string][in] */ wchar_t __RPC_FAR *strVisibleName,
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,guidSourceID);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[198],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[198],
                  ( unsigned char __RPC_FAR * )&strVisibleName,
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[198],
                  ( unsigned char __RPC_FAR * )&strVisibleName);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSAIsSourceRegistered( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,guidSourceID);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[228],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[228],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[228],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSARegisterStockEvent( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidEventID)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,guidEventID);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[252],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[252],
                  ( unsigned char __RPC_FAR * )&guidSourceID,
                  ( unsigned char __RPC_FAR * )&guidEventID);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[252],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSARegisterCustomEvent( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidEventID,
    /* [string][in] */ wchar_t __RPC_FAR *strVisibleName,
    /* [string][in] */ wchar_t __RPC_FAR *strDescription,
    /* [in] */ long nEventType,
    /* [string][in] */ wchar_t __RPC_FAR *guidCategory)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,guidCategory);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[282],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[282],
                  ( unsigned char __RPC_FAR * )&guidSourceID,
                  ( unsigned char __RPC_FAR * )&guidEventID,
                  ( unsigned char __RPC_FAR * )&strVisibleName,
                  ( unsigned char __RPC_FAR * )&strDescription,
                  ( unsigned char __RPC_FAR * )&nEventType,
                  ( unsigned char __RPC_FAR * )&guidCategory);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[282],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSARegisterEventCategory( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidCategoryID,
    /* [string][in] */ wchar_t __RPC_FAR *guidParentID,
    /* [string][in] */ wchar_t __RPC_FAR *strVisibileName,
    /* [string][in] */ wchar_t __RPC_FAR *strDescription)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,strDescription);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[336],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[336],
                  ( unsigned char __RPC_FAR * )&guidSourceID,
                  ( unsigned char __RPC_FAR * )&guidCategoryID,
                  ( unsigned char __RPC_FAR * )&guidParentID,
                  ( unsigned char __RPC_FAR * )&strVisibileName,
                  ( unsigned char __RPC_FAR * )&strDescription);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[336],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#endif
    return ( long  )_RetVal.Simple;
    
}


long VSAUnregisterSource( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,guidSourceID);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[384],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[384],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&IRPC_MicrosoftVisualStudioAnalyzer_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[384],
                  ( unsigned char __RPC_FAR * )&guidSourceID);
#endif
    return ( long  )_RetVal.Simple;
    
}


static const MIDL_STUB_DESC IRPC_MicrosoftVisualStudioAnalyzer_StubDesc = 
    {
    (void __RPC_FAR *)& IRPC_MicrosoftVisualStudioAnalyzer___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &VsaRpcl_IfHandle,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    0, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x303006e, /* MIDL Version 3.3.110 */
    0,
    0,
    0,  /* Reserved1 */
    0,  /* Reserved2 */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure VSAOpenConnection */

			0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/*  2 */	NdrFcShort( 0x0 ),	/* 0 */
#ifndef _ALPHA_
/*  4 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x10 ),	/* 16 */
/* 10 */	0x4,		/* 4 */
			0x2,		/* 2 */

	/* Parameter vaaId */

/* 12 */	NdrFcShort( 0x2150 ),	/* 8528 */
#ifndef _ALPHA_
/* 14 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 16 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 18 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 20 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 22 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSACloseConnection */

/* 24 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 26 */	NdrFcShort( 0x1 ),	/* 1 */
#ifndef _ALPHA_
/* 28 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 30 */	NdrFcShort( 0x8 ),	/* 8 */
/* 32 */	NdrFcShort( 0x8 ),	/* 8 */
/* 34 */	0x4,		/* 4 */
			0x2,		/* 2 */

	/* Parameter vaaId */

/* 36 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 38 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 44 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSABeginSession */

/* 48 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 50 */	NdrFcShort( 0x2 ),	/* 2 */
#ifndef _ALPHA_
/* 52 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	0x6,		/* 6 */
			0x4,		/* 4 */

	/* Parameter vaaId */

/* 60 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 62 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter guidSourceID */

/* 66 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 68 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 70 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter strSessionName */

/* 72 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 74 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 76 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 80 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSAEndSession */

/* 84 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 86 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 88 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	NdrFcShort( 0x8 ),	/* 8 */
/* 94 */	0x4,		/* 4 */
			0x2,		/* 2 */

	/* Parameter vaaId */

/* 96 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 98 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 104 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSAIsActive */

/* 108 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 110 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 112 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 116 */	NdrFcShort( 0x8 ),	/* 8 */
/* 118 */	0x4,		/* 4 */
			0x2,		/* 2 */

	/* Parameter vaaId */

/* 120 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 122 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 128 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSAFireEvent */

/* 132 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 134 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 136 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 138 */	NdrFcShort( 0x28 ),	/* 40 */
/* 140 */	NdrFcShort( 0x8 ),	/* 8 */
/* 142 */	0x6,		/* 6 */
			0x9,		/* 9 */

	/* Parameter vaaId */

/* 144 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 146 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter guidEvent */

/* 150 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 152 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 154 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter prgCount */

/* 156 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 158 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter prgKeys */

/* 162 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 164 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 166 */	NdrFcShort( 0x8 ),	/* Type Offset=8 */

	/* Parameter prgValues */

/* 168 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 172 */	NdrFcShort( 0x8 ),	/* Type Offset=8 */

	/* Parameter dwTimeLow */

/* 174 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 176 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwTimeHigh */

/* 180 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 182 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwFlags */

/* 186 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 188 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 190 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 192 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 194 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 196 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSARegisterSource */

/* 198 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 200 */	NdrFcShort( 0x6 ),	/* 6 */
#ifndef _ALPHA_
/* 202 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x8 ),	/* 8 */
/* 208 */	0x6,		/* 6 */
			0x3,		/* 3 */

	/* Parameter strVisibleName */

/* 210 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 212 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 214 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter guidSourceID */

/* 216 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 218 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 220 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 224 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSAIsSourceRegistered */

/* 228 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 230 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 232 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 238 */	0x6,		/* 6 */
			0x2,		/* 2 */

	/* Parameter guidSourceID */

/* 240 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 242 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 244 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 248 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSARegisterStockEvent */

/* 252 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 254 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 256 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x8 ),	/* 8 */
/* 262 */	0x6,		/* 6 */
			0x3,		/* 3 */

	/* Parameter guidSourceID */

/* 264 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 266 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 268 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter guidEventID */

/* 270 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 272 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 274 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 276 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 278 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSARegisterCustomEvent */

/* 282 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 284 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 286 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 288 */	NdrFcShort( 0x8 ),	/* 8 */
/* 290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 292 */	0x6,		/* 6 */
			0x7,		/* 7 */

	/* Parameter guidSourceID */

/* 294 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 296 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 298 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter guidEventID */

/* 300 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 302 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 304 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter strVisibleName */

/* 306 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 308 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 310 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter strDescription */

/* 312 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 314 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 316 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter nEventType */

/* 318 */	NdrFcShort( 0x48 ),	/* 72 */
#ifndef _ALPHA_
/* 320 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter guidCategory */

/* 324 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 326 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 328 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 332 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSARegisterEventCategory */

/* 336 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 338 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 340 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 346 */	0x6,		/* 6 */
			0x6,		/* 6 */

	/* Parameter guidSourceID */

/* 348 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 350 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 352 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter guidCategoryID */

/* 354 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 356 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 358 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter guidParentID */

/* 360 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 362 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 364 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter strVisibileName */

/* 366 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 368 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 370 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter strDescription */

/* 372 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 374 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 376 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 378 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 380 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure VSAUnregisterSource */

/* 384 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x40,		/* 64 */
/* 386 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 388 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0x8 ),	/* 8 */
/* 394 */	0x6,		/* 6 */
			0x2,		/* 2 */

	/* Parameter guidSourceID */

/* 396 */	NdrFcShort( 0x10b ),	/* 267 */
#ifndef _ALPHA_
/* 398 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 400 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 402 */	NdrFcShort( 0x70 ),	/* 112 */
#ifndef _ALPHA_
/* 404 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  2 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  4 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  6 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  8 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 10 */	NdrFcShort( 0x4 ),	/* 4 */
/* 12 */	0x28,		/* 40 */
			0x0,		/*  */
#ifndef _ALPHA_
/* 14 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 16 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 18 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 20 */	NdrFcShort( 0x4 ),	/* 4 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x1 ),	/* 1 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 32 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 34 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 36 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };
