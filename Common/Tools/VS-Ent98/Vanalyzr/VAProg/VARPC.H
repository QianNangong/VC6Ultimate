/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.03.0110 */
/* at Mon May 25 22:01:56 1998
 */
/* Compiler settings for varpc.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __varpc_h__
#define __varpc_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

/* header files for imported files */
#include "vaevt.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IRPC_MicrosoftVisualStudioAnalyzer_INTERFACE_DEFINED__
#define __IRPC_MicrosoftVisualStudioAnalyzer_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IRPC_MicrosoftVisualStudioAnalyzer
 * at Mon May 25 22:01:56 1998
 * using MIDL 3.03.0110
 ****************************************/
/* [implicit_handle][version][uuid] */ 


long VSAOpenConnection( 
    /* [out] */ long __RPC_FAR *vaaId);

long VSACloseConnection( 
    /* [in] */ long vaaId);

long VSABeginSession( 
    /* [in] */ long vaaId,
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *strSessionName);

long VSAEndSession( 
    /* [in] */ long vaaId);

long VSAIsActive( 
    /* [in] */ long vaaId);

long VSAFireEvent( 
    /* [in] */ long vaaId,
    /* [string][in] */ wchar_t __RPC_FAR *guidEvent,
    /* [in] */ long prgCount,
    /* [size_is][string][in] */ wchar_t __RPC_FAR *__RPC_FAR prgKeys[  ],
    /* [size_is][string][in] */ wchar_t __RPC_FAR *__RPC_FAR prgValues[  ],
    /* [in] */ long dwTimeLow,
    /* [in] */ long dwTimeHigh,
    /* [in] */ VSAEventFlags dwFlags);

long VSARegisterSource( 
    /* [string][in] */ wchar_t __RPC_FAR *strVisibleName,
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID);

long VSAIsSourceRegistered( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID);

long VSARegisterStockEvent( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidEventID);

long VSARegisterCustomEvent( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidEventID,
    /* [string][in] */ wchar_t __RPC_FAR *strVisibleName,
    /* [string][in] */ wchar_t __RPC_FAR *strDescription,
    /* [in] */ long nEventType,
    /* [string][in] */ wchar_t __RPC_FAR *guidCategory);

long VSARegisterEventCategory( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID,
    /* [string][in] */ wchar_t __RPC_FAR *guidCategoryID,
    /* [string][in] */ wchar_t __RPC_FAR *guidParentID,
    /* [string][in] */ wchar_t __RPC_FAR *strVisibileName,
    /* [string][in] */ wchar_t __RPC_FAR *strDescription);

long VSAUnregisterSource( 
    /* [string][in] */ wchar_t __RPC_FAR *guidSourceID);


extern handle_t VsaRpcl_IfHandle;


extern RPC_IF_HANDLE IRPC_MicrosoftVisualStudioAnalyzer_v1_0_c_ifspec;
extern RPC_IF_HANDLE IRPC_MicrosoftVisualStudioAnalyzer_v1_0_s_ifspec;
#endif /* __IRPC_MicrosoftVisualStudioAnalyzer_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
