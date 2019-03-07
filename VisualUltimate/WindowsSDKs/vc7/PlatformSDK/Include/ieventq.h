
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Mon Sep 20 16:32:23 1999
 */
/* Compiler settings for ieventq.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ieventq_h__
#define __ieventq_h__

/* Forward Declarations */ 

#ifndef __IAddEvents_FWD_DEFINED__
#define __IAddEvents_FWD_DEFINED__
typedef interface IAddEvents IAddEvents;
#endif 	/* __IAddEvents_FWD_DEFINED__ */


#ifndef __IReadEvents_FWD_DEFINED__
#define __IReadEvents_FWD_DEFINED__
typedef interface IReadEvents IReadEvents;
#endif 	/* __IReadEvents_FWD_DEFINED__ */


#ifndef __EVENTQUEUE_FWD_DEFINED__
#define __EVENTQUEUE_FWD_DEFINED__

#ifdef __cplusplus
typedef class EVENTQUEUE EVENTQUEUE;
#else
typedef struct EVENTQUEUE EVENTQUEUE;
#endif /* __cplusplus */

#endif 	/* __EVENTQUEUE_FWD_DEFINED__ */


/* header files for imported files */
#include "nmevent.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_ieventq_0000 */
/* [local] */ 

// The new event notification callback, expose this function:
// void WINAPI Callback(LPVOID pInstanceData, DWORD QueueIndex, DWORD EventIndex);
typedef void (WINAPI * PEVENTQCALLBACK )( LPVOID, DWORD, DWORD );
typedef struct _COLUMNENTRY
    {
    DWORD ColumnNumber;
    char __RPC_FAR *pColumnString;
    }	COLUMNENTRY;

typedef COLUMNENTRY __RPC_FAR *PCOLUMNENTRY;

typedef struct _COLUMNSTRINGTABLE
    {
    DWORD NumStrings;
    SIZE_T TotalStringLength;
    /* [size_is] */ COLUMNENTRY ColumnEntry[ 1 ];
    }	COLUMNSTRINGTABLE;

typedef COLUMNSTRINGTABLE __RPC_FAR *PCOLUMNSTRINGTABLE;

typedef struct _COLUMNFORMATARRAY
    {
    DWORD NumColumns;
    /* [size_is] */ DWORD FormatFlags[ 1 ];
    }	COLUMNFORMATARRAY;

typedef COLUMNFORMATARRAY __RPC_FAR *PCOLUMNFORMATARRAY;

#define FORMAT_HEX    0x00000001
#define FORMAT_YESNO  0x00000002
#define FORMAT_ONOFF  0x00000004
#define FORMAT_NODATE 0x00000010
typedef 
enum _EVENTFILTEROP
    {	EVENTFILTEROP_Equal	= 0,
	EVENTFILTEROP_NotEqual	= EVENTFILTEROP_Equal + 1,
	EVENTFILTEROP_GreaterThan	= EVENTFILTEROP_NotEqual + 1,
	EVENTFILTEROP_EqualOrGreaterThan	= EVENTFILTEROP_GreaterThan + 1,
	EVENTFILTEROP_LessThan	= EVENTFILTEROP_EqualOrGreaterThan + 1,
	EVENTFILTEROP_EqualOrLessThan	= EVENTFILTEROP_LessThan + 1
    }	EVENTFILTEROP;

typedef struct _EVENTFILTERENTRY
    {
    DWORD dwColumnNumber;
    EVENTFILTEROP Operator;
    NMCOLUMNVARIANT ColumnVariant;
    }	EVENTFILTERENTRY;

typedef EVENTFILTERENTRY __RPC_FAR *PEVENTFILTERENTRY;

typedef struct _EVENTFILTER
    {
    DWORD NumEntries;
    /* [size_is] */ EVENTFILTERENTRY Entry[ 1 ];
    }	EVENTFILTER;

typedef EVENTFILTER __RPC_FAR *PEVENTFILTER;

#define EVENT_FIXED_DISPLAYABLE_COLUMNS 7          


extern RPC_IF_HANDLE __MIDL_itf_ieventq_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ieventq_0000_v0_0_s_ifspec;

#ifndef __IAddEvents_INTERFACE_DEFINED__
#define __IAddEvents_INTERFACE_DEFINED__

/* interface IAddEvents */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IAddEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D710A6AE-3371-11d1-BE5B-00C04FC9E2BB")
    IAddEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddQueue( 
            /* [in] */ const char __RPC_FAR *szBaseDirectory,
            /* [in] */ char __RPC_FAR *szQueueName,
            /* [out] */ DWORD __RPC_FAR *pdwQueueIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddEvent( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ PNMEVENTDATA pEvent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAddEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAddEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAddEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAddEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddQueue )( 
            IAddEvents __RPC_FAR * This,
            /* [in] */ const char __RPC_FAR *szBaseDirectory,
            /* [in] */ char __RPC_FAR *szQueueName,
            /* [out] */ DWORD __RPC_FAR *pdwQueueIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddEvent )( 
            IAddEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ PNMEVENTDATA pEvent);
        
        END_INTERFACE
    } IAddEventsVtbl;

    interface IAddEvents
    {
        CONST_VTBL struct IAddEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAddEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAddEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAddEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAddEvents_AddQueue(This,szBaseDirectory,szQueueName,pdwQueueIndex)	\
    (This)->lpVtbl -> AddQueue(This,szBaseDirectory,szQueueName,pdwQueueIndex)

#define IAddEvents_AddEvent(This,dwQueueIndex,pEvent)	\
    (This)->lpVtbl -> AddEvent(This,dwQueueIndex,pEvent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAddEvents_AddQueue_Proxy( 
    IAddEvents __RPC_FAR * This,
    /* [in] */ const char __RPC_FAR *szBaseDirectory,
    /* [in] */ char __RPC_FAR *szQueueName,
    /* [out] */ DWORD __RPC_FAR *pdwQueueIndex);


void __RPC_STUB IAddEvents_AddQueue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAddEvents_AddEvent_Proxy( 
    IAddEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ PNMEVENTDATA pEvent);


void __RPC_STUB IAddEvents_AddEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAddEvents_INTERFACE_DEFINED__ */


#ifndef __IReadEvents_INTERFACE_DEFINED__
#define __IReadEvents_INTERFACE_DEFINED__

/* interface IReadEvents */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IReadEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F64AEFDE-3376-11d1-BE5B-00C04FC9E2BB")
    IReadEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterNotificationCallback( 
            /* [in] */ LPVOID pEventCallback,
            /* [in] */ LPVOID pInstanceData,
            /* [out] */ DWORD __RPC_FAR *pCallbackKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeregisterNotificationCallback( 
            /* [in] */ DWORD CallbackKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLatestQueueIndex( 
            /* [out] */ DWORD __RPC_FAR *pdwQueueIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQueueName( 
            /* [in] */ DWORD dwQueueIndex,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLatestEventIndex( 
            /* [in] */ DWORD dwQueueIndex,
            /* [out] */ DWORD __RPC_FAR *pdwEventIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoesEventPassFilter( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PEVENTFILTER pFilter,
            /* [out] */ BOOL __RPC_FAR *pfDoesItPass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompareEvents( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwFirstEventIndex,
            /* [in] */ DWORD dwSecondEventIndex,
            /* [in] */ DWORD dwColumnNumber,
            /* [out] */ long __RPC_FAR *plResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEvent( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [out] */ PNMEVENTDATA __RPC_FAR *ppEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFullFormattedEvent( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
            /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSparseFormattedEvent( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
            /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetColumnIndex( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ char __RPC_FAR *szColumnName,
            /* [out] */ DWORD __RPC_FAR *pdwColumnNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetColumnName( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwColumnNumber,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FlushEvents( 
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwNumberOfEventsToDelete) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReadEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IReadEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IReadEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterNotificationCallback )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ LPVOID pEventCallback,
            /* [in] */ LPVOID pInstanceData,
            /* [out] */ DWORD __RPC_FAR *pCallbackKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeregisterNotificationCallback )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD CallbackKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLatestQueueIndex )( 
            IReadEvents __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwQueueIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetQueueName )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLatestEventIndex )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [out] */ DWORD __RPC_FAR *pdwEventIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoesEventPassFilter )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PEVENTFILTER pFilter,
            /* [out] */ BOOL __RPC_FAR *pfDoesItPass);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompareEvents )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwFirstEventIndex,
            /* [in] */ DWORD dwSecondEventIndex,
            /* [in] */ DWORD dwColumnNumber,
            /* [out] */ long __RPC_FAR *plResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEvent )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [out] */ PNMEVENTDATA __RPC_FAR *ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFullFormattedEvent )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
            /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSparseFormattedEvent )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwEventIndex,
            /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
            /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColumnIndex )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ char __RPC_FAR *szColumnName,
            /* [out] */ DWORD __RPC_FAR *pdwColumnNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColumnName )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwColumnNumber,
            /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FlushEvents )( 
            IReadEvents __RPC_FAR * This,
            /* [in] */ DWORD dwQueueIndex,
            /* [in] */ DWORD dwNumberOfEventsToDelete);
        
        END_INTERFACE
    } IReadEventsVtbl;

    interface IReadEvents
    {
        CONST_VTBL struct IReadEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReadEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReadEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReadEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReadEvents_RegisterNotificationCallback(This,pEventCallback,pInstanceData,pCallbackKey)	\
    (This)->lpVtbl -> RegisterNotificationCallback(This,pEventCallback,pInstanceData,pCallbackKey)

#define IReadEvents_DeregisterNotificationCallback(This,CallbackKey)	\
    (This)->lpVtbl -> DeregisterNotificationCallback(This,CallbackKey)

#define IReadEvents_GetLatestQueueIndex(This,pdwQueueIndex)	\
    (This)->lpVtbl -> GetLatestQueueIndex(This,pdwQueueIndex)

#define IReadEvents_GetQueueName(This,dwQueueIndex,ppColumnName)	\
    (This)->lpVtbl -> GetQueueName(This,dwQueueIndex,ppColumnName)

#define IReadEvents_GetLatestEventIndex(This,dwQueueIndex,pdwEventIndex)	\
    (This)->lpVtbl -> GetLatestEventIndex(This,dwQueueIndex,pdwEventIndex)

#define IReadEvents_DoesEventPassFilter(This,dwQueueIndex,dwEventIndex,pFilter,pfDoesItPass)	\
    (This)->lpVtbl -> DoesEventPassFilter(This,dwQueueIndex,dwEventIndex,pFilter,pfDoesItPass)

#define IReadEvents_CompareEvents(This,dwQueueIndex,dwFirstEventIndex,dwSecondEventIndex,dwColumnNumber,plResult)	\
    (This)->lpVtbl -> CompareEvents(This,dwQueueIndex,dwFirstEventIndex,dwSecondEventIndex,dwColumnNumber,plResult)

#define IReadEvents_GetEvent(This,dwQueueIndex,dwEventIndex,ppEvent)	\
    (This)->lpVtbl -> GetEvent(This,dwQueueIndex,dwEventIndex,ppEvent)

#define IReadEvents_GetFullFormattedEvent(This,dwQueueIndex,dwEventIndex,pColumnFormatArray,ppColumnStringTable)	\
    (This)->lpVtbl -> GetFullFormattedEvent(This,dwQueueIndex,dwEventIndex,pColumnFormatArray,ppColumnStringTable)

#define IReadEvents_GetSparseFormattedEvent(This,dwQueueIndex,dwEventIndex,pColumnFormatArray,ppColumnStringTable)	\
    (This)->lpVtbl -> GetSparseFormattedEvent(This,dwQueueIndex,dwEventIndex,pColumnFormatArray,ppColumnStringTable)

#define IReadEvents_GetColumnIndex(This,dwQueueIndex,szColumnName,pdwColumnNumber)	\
    (This)->lpVtbl -> GetColumnIndex(This,dwQueueIndex,szColumnName,pdwColumnNumber)

#define IReadEvents_GetColumnName(This,dwQueueIndex,dwColumnNumber,ppColumnName)	\
    (This)->lpVtbl -> GetColumnName(This,dwQueueIndex,dwColumnNumber,ppColumnName)

#define IReadEvents_FlushEvents(This,dwQueueIndex,dwNumberOfEventsToDelete)	\
    (This)->lpVtbl -> FlushEvents(This,dwQueueIndex,dwNumberOfEventsToDelete)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IReadEvents_RegisterNotificationCallback_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ LPVOID pEventCallback,
    /* [in] */ LPVOID pInstanceData,
    /* [out] */ DWORD __RPC_FAR *pCallbackKey);


void __RPC_STUB IReadEvents_RegisterNotificationCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_DeregisterNotificationCallback_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD CallbackKey);


void __RPC_STUB IReadEvents_DeregisterNotificationCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetLatestQueueIndex_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwQueueIndex);


void __RPC_STUB IReadEvents_GetLatestQueueIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetQueueName_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName);


void __RPC_STUB IReadEvents_GetQueueName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetLatestEventIndex_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [out] */ DWORD __RPC_FAR *pdwEventIndex);


void __RPC_STUB IReadEvents_GetLatestEventIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_DoesEventPassFilter_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwEventIndex,
    /* [in] */ PEVENTFILTER pFilter,
    /* [out] */ BOOL __RPC_FAR *pfDoesItPass);


void __RPC_STUB IReadEvents_DoesEventPassFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_CompareEvents_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwFirstEventIndex,
    /* [in] */ DWORD dwSecondEventIndex,
    /* [in] */ DWORD dwColumnNumber,
    /* [out] */ long __RPC_FAR *plResult);


void __RPC_STUB IReadEvents_CompareEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetEvent_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwEventIndex,
    /* [out] */ PNMEVENTDATA __RPC_FAR *ppEvent);


void __RPC_STUB IReadEvents_GetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetFullFormattedEvent_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwEventIndex,
    /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
    /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable);


void __RPC_STUB IReadEvents_GetFullFormattedEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetSparseFormattedEvent_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwEventIndex,
    /* [in] */ PCOLUMNFORMATARRAY pColumnFormatArray,
    /* [out] */ PCOLUMNSTRINGTABLE __RPC_FAR *ppColumnStringTable);


void __RPC_STUB IReadEvents_GetSparseFormattedEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetColumnIndex_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ char __RPC_FAR *szColumnName,
    /* [out] */ DWORD __RPC_FAR *pdwColumnNumber);


void __RPC_STUB IReadEvents_GetColumnIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_GetColumnName_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwColumnNumber,
    /* [out] */ char __RPC_FAR *__RPC_FAR *ppColumnName);


void __RPC_STUB IReadEvents_GetColumnName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReadEvents_FlushEvents_Proxy( 
    IReadEvents __RPC_FAR * This,
    /* [in] */ DWORD dwQueueIndex,
    /* [in] */ DWORD dwNumberOfEventsToDelete);


void __RPC_STUB IReadEvents_FlushEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReadEvents_INTERFACE_DEFINED__ */



#ifndef __EventQLib_LIBRARY_DEFINED__
#define __EventQLib_LIBRARY_DEFINED__

/* library EventQLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_EventQLib;

EXTERN_C const CLSID CLSID_EVENTQUEUE;

#ifdef __cplusplus

class DECLSPEC_UUID("6E0FF466-339E-11d1-BE5B-00C04FC9E2BB")
EVENTQUEUE;
#endif
#endif /* __EventQLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


