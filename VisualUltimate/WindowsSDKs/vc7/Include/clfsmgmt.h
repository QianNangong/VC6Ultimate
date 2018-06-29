/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    clfsmgmt.h

Abstract:

    This module exposes the API for managing CLFS logs for kernel
    mode clients.  "Managing" in this sense means manipulating
    log file policies and performing operations (such as log growth)
    which honor the policies.

Author:

    J.R. Tipton (jrtipton) 15-Aug-2003

Revision History:

--*/

#ifndef _CLFSMGMT_H_
#define _CLFSMGMT_H_
#pragma once

#include <clfslsn.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// We start with the information that is shared
// between user and kernel mode.
//

typedef enum _CLFS_MGMT_POLICY_TYPE {

    ClfsMgmtPolicyMaximumSize = 0x0,
    ClfsMgmtPolicyMinimumSize,
    ClfsMgmtPolicyNewContainerSize,
    ClfsMgmtPolicyGrowthRate,
    ClfsMgmtPolicyLogTail,
    ClfsMgmtPolicyAutoShrink,
    ClfsMgmtPolicyAutoGrow,
    ClfsMgmtPolicyNewContainerPrefix,
    ClfsMgmtPolicyNewContainerSuffix,

    ClfsMgmtPolicyInvalid

} CLFS_MGMT_POLICY_TYPE, *PCLFS_MGMT_POLICY_TYPE;

#define CLFS_MGMT_NUM_POLICIES ((ULONG)ClfsMgmtPolicyInvalid)


//
// Relative sizes used when explicitly setting log size.
//
#define CLFS_LOG_SIZE_MINIMUM ((ULONGLONG)(0))
#define CLFS_LOG_SIZE_MAXIMUM ((ULONGLONG)(-1))


//
// The version of a given policy structure.  See CLFS_MGMT_POLICY.
//
#define CLFS_MGMT_POLICY_VERSION (0x01)


//
// Log policy flags.
//
// LOG_POLICY_OVERWRITE: If set when adding a log policy, the previous
//                       policy of given type will be replaced.
//
// LOG_POLICY_PERSIST:   If set when adding a log policy, the policy
//                       will be persisted with the log metadata.
//
#define LOG_POLICY_OVERWRITE   (0x01)
#define LOG_POLICY_PERSIST     (0x02)


//
// CLFS_MGMT_POLICY
//
// This structure describes one particular policy that
// may be present on a log file.  These are installed
// via InstallLogPolicy (Win32) or ClfsMgmtInstallPolicy (kernel).
//
typedef struct _CLFS_MGMT_POLICY {

    //
    // Version of the structure.  Should be CLFS_MGMT_POLICY_VERSION.
    //
    ULONG                   Version;

    //
    // The entire length of the structure.
    //
    ULONG                   LengthInBytes;

    //
    // Flags which apply to all policies, such as LOG_POLICY_OVERWRITE
    // and LOG_POLICY_PERSIST.
    //
    ULONG                   PolicyFlags;

    //
    // Determines how PolicyParameters union is interpreted.
    //
    CLFS_MGMT_POLICY_TYPE   PolicyType;

    //
    // The way to interpret the PolicyParameters union is
    // determined by the value of PolicyType -- if it is
    // ClfsMgmtPolicyMaximumSize, for instance, then the
    // MaximumSize structure is the relevant one.
    //

    union {

        struct {
            ULONG       Containers;
        } MaximumSize;

        struct {
            ULONG       Containers;
        } MinimumSize;

        struct {
            ULONG       AbsoluteGrowthInContainers;
            ULONG       RelativeGrowthPercentage;
        } GrowthRate;

        struct {
            ULONG       MinimumAvailablePercentage;
            ULONG       MinimumAvailableContainers;
        } LogTail;

        struct {
            ULONG       Percentage;
        } AutoShrink;

        struct {
            ULONG       Enabled;
        } AutoGrow;

        struct {
            USHORT      PrefixLengthInBytes;
            WCHAR       PrefixString[1]; // dynamic in length depending on PrefixLength
        } NewContainerPrefix;

        struct {
            ULONGLONG   NextContainerSuffix;
        } NewContainerSuffix;

    } PolicyParameters;

    //
    // Nothing will be added down here since the structure above
    // can be of dynamic length.
    //

} CLFS_MGMT_POLICY, *PCLFS_MGMT_POLICY;



//
// CLFS_MGMT_NOTIFICATION_TYPE
// 
// The types of notifications given to either the callback proxy
// or to readers of notifications.
//

typedef enum _CLFS_MGMT_NOTIFICATION_TYPE
{

    //
    // Notification to advance base LSN.
    //

    ClfsMgmtAdvanceTailNotification = 0,

    //
    // Notification that a request to handle log full condition
    // has completed.
    //

    ClfsMgmtLogFullHandlerNotification,

    //
    // Notification that a previously pinned log is now considered
    // unpinned.
    //

    ClfsMgmtLogUnpinnedNotification

} CLFS_MGMT_NOTIFICATION_TYPE, *PCLFS_MGMT_NOTIFICATION_TYPE;


//
// CLFS_MGMT_NOTIFICATION
//
// A notification and associated parameters.
//

typedef struct _CLFS_MGMT_NOTIFICATION
{

    //
    // Nature of the notification.
    //

    CLFS_MGMT_NOTIFICATION_TYPE     Notification;
    
    //
    // Target LSN for base LSN advancement if the
    // notification type is ClfsMgmtAdvanceTailNotification.
    //

    CLFS_LSN                        Lsn;

    //
    // TRUE if the log is pinned, FALSE otherwise.
    // Especially meaningful when receiving an error
    // status for ClfsMgmtLogFullHandlerNotification.
    //

    BOOL                            LogIsPinned;

} CLFS_MGMT_NOTIFICATION, *PCLFS_MGMT_NOTIFICATION;


//
// Kernel interface described below.
//

#ifdef KERNEL_MODE

//
// The advance tail callback is required when log clients 
// register for management.  It is invoked whenever the 
// management library decides that this client needs to 
// advance the tail of its log. Only minimal processing is 
// allowed.
//
typedef
NTSTATUS
(*PCLFS_CLIENT_ADVANCE_TAIL_CALLBACK) (
    IN PLOG_FILE_OBJECT LogFile,
    IN PCLFS_LSN        TargetLsn,
    IN PVOID            ClientData
    );

//
// The log file full handler complete callback is invoked upon
// completion of a log growth request (that is, via a call 
// to ClfsMgmtHandleLogFileFull).
//
typedef
VOID
(*PCLFS_CLIENT_LFF_HANDLER_COMPLETE_CALLBACK) (
    IN PLOG_FILE_OBJECT LogFile,
    IN NTSTATUS         OperationStatus,
    IN BOOLEAN          LogIsPinned,
    IN PVOID            ClientData
    );

//
// The log pinned callback is invoked when log space is freed up
// after a log file full handler completion callback indicates an 
// NT_ERROR status code and LogIsPinned = TRUE.
//

typedef
VOID
(*PCLFS_CLIENT_LOG_UNPINNED_CALLBACK) (
    IN PLOG_FILE_OBJECT LogFile,
    IN PVOID            ClientData
    );


//
// The log size complete callback is invoked whenever
// ClfsMgmtSetLogFileSize operation which returned
// STATUS_PENDING is completed.
//

typedef
VOID
(*PCLFS_SET_LOG_SIZE_COMPLETE_CALLBACK) (
    IN PLOG_FILE_OBJECT LogFile,
    IN NTSTATUS         OperationStatus,
    IN PVOID            ClientData
    );


//
// CLFS_MGMT_CLIENT_REGISTRATION
//
// This structure is given to the CLFS management infrastructure
// by clients who wish to be managed (via ClfsMgmtRegisterManagedClient).
// The CLFS_MGMT_CLIENT_REGISTRATION_VERSION value must be stored
// in the 'Version' field of the structure.
//

#define CLFS_MGMT_CLIENT_REGISTRATION_VERSION (0x1)

typedef struct _CLFS_MGMT_CLIENT_REGISTRATION {

    //
    // Initialize Version to CLFS_MGMT_CLIENT_REGISTRATION_VERSION.
    //

    ULONG   Version;  

    PCLFS_CLIENT_ADVANCE_TAIL_CALLBACK AdvanceTailCallback;
    PVOID                              AdvanceTailCallbackData;

    PCLFS_CLIENT_LFF_HANDLER_COMPLETE_CALLBACK LogGrowthCompleteCallback;
    PVOID                                      LogGrowthCompleteCallbackData;

    PCLFS_CLIENT_LOG_UNPINNED_CALLBACK LogUnpinnedCallback;
    PVOID                              LogUnpinnedCallbackData;

} CLFS_MGMT_CLIENT_REGISTRATION, *PCLFS_MGMT_CLIENT_REGISTRATION;


//
// CLFS_MGMT_CLIENT
//
// This is the cookie that clients are given when registering and
// must give back to the management infrastructure whenever 
// performing an operation.
//
typedef PVOID CLFS_MGMT_CLIENT, *PCLFS_MGMT_CLIENT;


NTSTATUS
ClfsMgmtRegisterManagedClient(
    IN  PLOG_FILE_OBJECT LogFile,
    IN  PCLFS_MGMT_CLIENT_REGISTRATION RegistrationData,
    OUT PCLFS_MGMT_CLIENT ClientCookie
    );


NTSTATUS
ClfsMgmtDeregisterManagedClient(
    IN CLFS_MGMT_CLIENT ClientCookie
    );

NTSTATUS
ClfsMgmtTailAdvanceFailure(
    IN CLFS_MGMT_CLIENT  Client,
    IN NTSTATUS          Reason
    );


NTSTATUS
ClfsMgmtHandleLogFileFull(
    IN CLFS_MGMT_CLIENT Client
    );


NTSTATUS
ClfsMgmtInstallPolicy(
    IN PLOG_FILE_OBJECT  LogFile,
    IN PCLFS_MGMT_POLICY Policy,
    IN ULONG             PolicyLength
    );

NTSTATUS
ClfsMgmtRemovePolicy(
    IN PLOG_FILE_OBJECT      LogFile,
    IN CLFS_MGMT_POLICY_TYPE PolicyType
    );

NTSTATUS
ClfsMgmtSetLogFileSize(
    IN  PLOG_FILE_OBJECT  LogFile,
    IN  PULONGLONG        NewSizeInContainers,
    OUT PULONGLONG        ResultingSizeInContainers,
    IN  PCLFS_SET_LOG_SIZE_COMPLETE_CALLBACK CompletionRoutine,
    IN  PVOID                                CompletionRoutineData
    );

#endif // KERNEL_MODE

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _CLFSMGMT_H_
