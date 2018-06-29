/*=============================================================================

    Copyright (c) 2003  Microsoft Corporation

    Module Name:

        clfsmgmtw32.h

    Abstract:

        Declares the exported API set for the Common Log Win32
        Management API.

    Author:

        JR Tipton [jrtipton] 24-Aug-2003

    Environment:

        User Mode

    Revision History:


=============================================================================*/
#ifndef __CLFSMGMTW32_H__
#define __CLFSMGMTW32_H__


#ifdef KERNEL_MODE
#    undef KERNEL_MODE
#endif KERNEL_MODE

#include <clfsmgmt.h>

#ifdef __cplusplus
extern "C"
{
#endif __cplusplus

typedef
VOID
(*PLOG_TAIL_ADVANCE_CALLBACK) (
    IN HANDLE           hLogFile,
    IN CLFS_LSN         lsnTarget,
    IN PVOID            pvClientContext
    );

typedef
VOID
(*PLOG_FULL_HANDLER_CALLBACK) (
    IN HANDLE           hLogFile,
    IN DWORD            dwError,
    IN BOOL             fLogIsPinned,
    IN PVOID            pvClientContext
    );

typedef
VOID
(*PLOG_UNPINNED_CALLBACK) (
    IN HANDLE           hLogFile,
    IN PVOID            pvClientContext
    );


typedef struct _LOG_MANAGEMENT_CALLBACKS
{
    PVOID   CallbackContext;

    PLOG_TAIL_ADVANCE_CALLBACK AdvanceTailCallback;
    PLOG_FULL_HANDLER_CALLBACK LogFullHandlerCallback;
    PLOG_UNPINNED_CALLBACK     LogUnpinnedCallback;

} LOG_MANAGEMENT_CALLBACKS, *PLOG_MANAGEMENT_CALLBACKS;


//------------------------------------------------------------------------------
// RegisterManageableLogClient
// 
// Registers a client as one that wishes to participate in the CLFS management
// scheme.  The client can receive notifications such as "advance base lsn" if
// pCallbacks is non-null.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI RegisterManageableLogClient (
    IN HANDLE                         hLog,
    IN PLOG_MANAGEMENT_CALLBACKS      pCallbacks
    );

//------------------------------------------------------------------------------
// DeregisterManageableLogClient
// 
// When a log client no longer wishes to be registered, either the log handle
// can be closed (deregistering the client implicitly) or this routine can
// be invoked which will deregister the client explicitly.  The client will
// no longer receive notifications related to management.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI DeregisterManageableLogClient (
    IN HANDLE hLog
    );


//------------------------------------------------------------------------------
// ReadLogNotification
// 
// Allows a registered log management client to read a management notification.
// Only possible if the client is not receiving callbacks.  Can function in
// synchronous or asynchronous mode.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI ReadLogNotification (
    IN  HANDLE                   hLog,
    OUT PCLFS_MGMT_NOTIFICATION  pNotification,
    IN  LPOVERLAPPED             lpOverlapped
    );


//------------------------------------------------------------------------------
// InstallLogPolicy
// 
// Installs a policy on a log.  The possible policies are described by the 
// enumerated type CLFS_MGMT_POLICY_TYPE.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI InstallLogPolicy (
    IN HANDLE            hLog,
    IN PCLFS_MGMT_POLICY pPolicy
    );

//------------------------------------------------------------------------------
// InstallLogPolicy
// 
// Removes a policy from a log, reverting back to the default behavior for the
// policy indicated.  The possible policies are described by the enumerated type 
// CLFS_MGMT_POLICY_TYPE.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI RemoveLogPolicy (
    IN HANDLE                hLog,
    IN CLFS_MGMT_POLICY_TYPE ePolicyType
    );

//------------------------------------------------------------------------------
// QueryLogPolicy
// 
// Given a policy type, returns the associated policy data.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI QueryLogPolicy (
    IN     HANDLE                hLog,
    IN     CLFS_MGMT_POLICY_TYPE ePolicyType,
    OUT    PCLFS_MGMT_POLICY     pPolicyBuffer,
    IN OUT PULONG                pcbPolicyBuffer
    );

//------------------------------------------------------------------------------
// SetLogFileSizeWithPolicy
// 
// Sets the size of a log file, obeying any policies present on the log file
// at the time.
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI SetLogFileSizeWithPolicy(
    IN  HANDLE     hLog,
    IN  PULONGLONG pDesiredSize,
    OUT PULONGLONG pResultingSize
    );

//------------------------------------------------------------------------------
// HandleLogFull
// 
// Handles a log full condition according to policy (either by growing the log
// or demanding that other streams advance their log bases).
//
// This routine may do the work asynchronously (returning FALSE with last error
// of ERROR_IO_PENDING).
//------------------------------------------------------------------------------
CLFSUSER_API BOOL WINAPI HandleLogFull(
    IN HANDLE hLog
    );

#endif    __CLFSMGMTW32_H__

#ifdef __cplusplus
} // extern "C"
#endif __cplusplus

//-----------------------------------------------------------------------------
//                                        END OF FILE
//-----------------------------------------------------------------------------
