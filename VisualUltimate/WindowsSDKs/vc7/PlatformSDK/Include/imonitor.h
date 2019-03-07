/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Thu Nov 19 19:26:51 1998
 */
/* Compiler settings for imonitor.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __imonitor_h__
#define __imonitor_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMonitor_FWD_DEFINED__
#define __IMonitor_FWD_DEFINED__
typedef interface IMonitor IMonitor;
#endif 	/* __IMonitor_FWD_DEFINED__ */


#ifndef __IMonitorEventer_FWD_DEFINED__
#define __IMonitorEventer_FWD_DEFINED__
typedef interface IMonitorEventer IMonitorEventer;
#endif 	/* __IMonitorEventer_FWD_DEFINED__ */


/* header files for imported files */
#include "nmevent.h"
#include "nmMCS.h"
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMonitor_INTERFACE_DEFINED__
#define __IMonitor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMonitor
 * at Thu Nov 19 19:26:51 1998
 * using MIDL 3.01.75
 ****************************************/
/* [local][unique][uuid][object] */ 



EXTERN_C const IID IID_IMonitor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("B22A7F70-9398-11d1-A5D6-00A024BA4B62")
    IMonitor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DoInitialize( 
            /* [in] */ IUnknown __RPC_FAR *pUnkMonitorCtrl,
            /* [out][in] */ HBLOB hNPPBlob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoConfigure( 
            /* [in] */ char __RPC_FAR *pName,
            /* [in] */ char __RPC_FAR *pConfiguration,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppScriptInstance) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnStart( 
            /* [in] */ IUnknown __RPC_FAR *pUnkNPP) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnStop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnFrames( 
            /* [in] */ UPDATE_EVENT Event) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnStatus( 
            /* [in] */ UPDATE_EVENT Event) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMonitorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMonitor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMonitor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoInitialize )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkMonitorCtrl,
            /* [out][in] */ HBLOB hNPPBlob);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoConfigure )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ char __RPC_FAR *pName,
            /* [in] */ char __RPC_FAR *pConfiguration,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppScriptInstance);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnStart )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkNPP);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnStop )( 
            IMonitor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnFrames )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ UPDATE_EVENT Event);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnStatus )( 
            IMonitor __RPC_FAR * This,
            /* [in] */ UPDATE_EVENT Event);
        
        END_INTERFACE
    } IMonitorVtbl;

    interface IMonitor
    {
        CONST_VTBL struct IMonitorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMonitor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMonitor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMonitor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMonitor_DoInitialize(This,pUnkMonitorCtrl,hNPPBlob)	\
    (This)->lpVtbl -> DoInitialize(This,pUnkMonitorCtrl,hNPPBlob)

#define IMonitor_DoConfigure(This,pName,pConfiguration,ppScriptInstance)	\
    (This)->lpVtbl -> DoConfigure(This,pName,pConfiguration,ppScriptInstance)

#define IMonitor_OnStart(This,pUnkNPP)	\
    (This)->lpVtbl -> OnStart(This,pUnkNPP)

#define IMonitor_OnStop(This)	\
    (This)->lpVtbl -> OnStop(This)

#define IMonitor_OnFrames(This,Event)	\
    (This)->lpVtbl -> OnFrames(This,Event)

#define IMonitor_OnStatus(This,Event)	\
    (This)->lpVtbl -> OnStatus(This,Event)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMonitor_DoInitialize_Proxy( 
    IMonitor __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkMonitorCtrl,
    /* [out][in] */ HBLOB hNPPBlob);


void __RPC_STUB IMonitor_DoInitialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitor_DoConfigure_Proxy( 
    IMonitor __RPC_FAR * This,
    /* [in] */ char __RPC_FAR *pName,
    /* [in] */ char __RPC_FAR *pConfiguration,
    /* [out] */ char __RPC_FAR *__RPC_FAR *ppScriptInstance);


void __RPC_STUB IMonitor_DoConfigure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitor_OnStart_Proxy( 
    IMonitor __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkNPP);


void __RPC_STUB IMonitor_OnStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitor_OnStop_Proxy( 
    IMonitor __RPC_FAR * This);


void __RPC_STUB IMonitor_OnStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitor_OnFrames_Proxy( 
    IMonitor __RPC_FAR * This,
    /* [in] */ UPDATE_EVENT Event);


void __RPC_STUB IMonitor_OnFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitor_OnStatus_Proxy( 
    IMonitor __RPC_FAR * This,
    /* [in] */ UPDATE_EVENT Event);


void __RPC_STUB IMonitor_OnStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMonitor_INTERFACE_DEFINED__ */


#ifndef __IMonitorEventer_INTERFACE_DEFINED__
#define __IMonitorEventer_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMonitorEventer
 * at Thu Nov 19 19:26:51 1998
 * using MIDL 3.01.75
 ****************************************/
/* [local][unique][uuid][object] */ 



EXTERN_C const IID IID_IMonitorEventer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("7390CC40-95D8-11d1-A5D8-00A024BA4B62")
    IMonitorEventer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEventData( 
            /* [in] */ BYTE bNumColumns,
            /* [out] */ PNMEVENTDATA __RPC_FAR *ppNMEventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendNMEvent( 
            /* [in] */ PNMEVENTDATA pNMEventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeEventData( 
            /* [in] */ PNMEVENTDATA pNMEventData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMonitorEventerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMonitorEventer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMonitorEventer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMonitorEventer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventData )( 
            IMonitorEventer __RPC_FAR * This,
            /* [in] */ BYTE bNumColumns,
            /* [out] */ PNMEVENTDATA __RPC_FAR *ppNMEventData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendNMEvent )( 
            IMonitorEventer __RPC_FAR * This,
            /* [in] */ PNMEVENTDATA pNMEventData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FreeEventData )( 
            IMonitorEventer __RPC_FAR * This,
            /* [in] */ PNMEVENTDATA pNMEventData);
        
        END_INTERFACE
    } IMonitorEventerVtbl;

    interface IMonitorEventer
    {
        CONST_VTBL struct IMonitorEventerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMonitorEventer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMonitorEventer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMonitorEventer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMonitorEventer_GetEventData(This,bNumColumns,ppNMEventData)	\
    (This)->lpVtbl -> GetEventData(This,bNumColumns,ppNMEventData)

#define IMonitorEventer_SendNMEvent(This,pNMEventData)	\
    (This)->lpVtbl -> SendNMEvent(This,pNMEventData)

#define IMonitorEventer_FreeEventData(This,pNMEventData)	\
    (This)->lpVtbl -> FreeEventData(This,pNMEventData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMonitorEventer_GetEventData_Proxy( 
    IMonitorEventer __RPC_FAR * This,
    /* [in] */ BYTE bNumColumns,
    /* [out] */ PNMEVENTDATA __RPC_FAR *ppNMEventData);


void __RPC_STUB IMonitorEventer_GetEventData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitorEventer_SendNMEvent_Proxy( 
    IMonitorEventer __RPC_FAR * This,
    /* [in] */ PNMEVENTDATA pNMEventData);


void __RPC_STUB IMonitorEventer_SendNMEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMonitorEventer_FreeEventData_Proxy( 
    IMonitorEventer __RPC_FAR * This,
    /* [in] */ PNMEVENTDATA pNMEventData);


void __RPC_STUB IMonitorEventer_FreeEventData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMonitorEventer_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_imonitor_0140
 * at Thu Nov 19 19:26:51 1998
 * using MIDL 3.01.75
 ****************************************/
/* [local] */ 


__MIDL_DECLSPEC_DLLEXPORT HRESULT DllGetMonitorObject( 
    REFIID riid,
    LPVOID __RPC_FAR *ppObj);

__MIDL_DECLSPEC_DLLEXPORT HRESULT OnLoadingDLL( 
    HBLOB hFilterBlob,
    DWORD __RPC_FAR *pCreateFlags,
    char __RPC_FAR *__RPC_FAR *ppDefaultName,
    char __RPC_FAR *__RPC_FAR *ppDescription,
    char __RPC_FAR *__RPC_FAR *ppDefaultScript,
    char __RPC_FAR *__RPC_FAR *ppDefaultConfiguration);



extern RPC_IF_HANDLE __MIDL_itf_imonitor_0140_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_imonitor_0140_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
