//+---------------------------------------------------------------------------
//
// Microsoft Windows
// Copyright (c) Microsoft Corporation.  All rights reserved.
// 
// File:     p2p.h
// 
// Contents: Win32 APIs and structures for the Microsoft Peer To Peer infrastructure.
//
//----------------------------------------------------------------------------
//
//  If defined, the following flags inhibit definition of the indicated items.
//
//  NO_P2P_GROUP    - no Peer-to-Peer Grouping
//  NO_P2P_GRAPH    - no Peer-to-Peer Graphing
//  NO_P2P_IDENTITY - no Peer-to-Peer Identity manager
//
//----------------------------------------------------------------------------

#ifndef _P2P_H_
#define _P2P_H_

#include <winsock2.h>
#include <ws2tcpip.h>  // for sockaddrin_6

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//////////////////////////////////////////////////////////////////////////////
// Constants

typedef enum peer_record_change_type_tag {
    PEER_RECORD_ADDED                           = 1,
    PEER_RECORD_UPDATED                         = 2,
    PEER_RECORD_DELETED                         = 3,
    PEER_RECORD_EXPIRED                         = 4,
} PEER_RECORD_CHANGE_TYPE;

typedef enum peer_connection_status_tag {
    PEER_CONNECTED                              = 1,
    PEER_DISCONNECTED                           = 2,
    PEER_CONNECTION_FAILED                      = 3,
} PEER_CONNECTION_STATUS;

typedef enum peer_connection_flags_tag {
    PEER_CONNECTION_NEIGHBOR                    = 0x0001,
    PEER_CONNECTION_DIRECT                      = 0x0002,
} PEER_CONNECTION_FLAGS;

typedef enum peer_record_flags_tag {
    PEER_RECORD_FLAG_AUTOREFRESH                = 0x0001,
    PEER_RECORD_FLAG_DELETED                    = 0x0002,
} PEER_RECORD_FLAGS;

// Maximum number of characters in a record attribute string
#define MAX_ATTRIBUTE_STREAM_LENGTH     100000


////////////////////////////////////////////////////////////////////////////////////
// Types

typedef PVOID HPEERENUM, *PHPEERENUM;
typedef PVOID HPEEREVENT, *PHPEEREVENT;

typedef struct peer_version_data_tag {
    WORD        wVersion;
    WORD        wHighestVersion;
} PEER_VERSION_DATA, *PPEER_VERSION_DATA;

typedef struct peer_data_tag {
    ULONG       cbData;
    PBYTE       pbData;
} PEER_DATA, * PPEER_DATA;

typedef struct peer_record_tag {
    DWORD       dwSize;
    GUID        type;
    GUID        id;
    DWORD       dwVersion;
    DWORD       dwFlags;
    PWSTR       pwzCreatorId;
    PWSTR       pwzModifiedById;
    PWSTR       pwzAttributes;
    FILETIME    ftCreation;
    FILETIME    ftExpiration;
    FILETIME    ftLastModified;
    PEER_DATA   securityData;
    PEER_DATA   data;
} PEER_RECORD, *PPEER_RECORD;

typedef struct peer_address_tag {
    DWORD                   dwSize;
    SOCKADDR_IN6            sin6;
} PEER_ADDRESS, *PPEER_ADDRESS;

typedef struct peer_connection_info_tag {
    DWORD              dwSize;
    DWORD              dwFlags;
    ULONGLONG          ullConnectionId;
    ULONGLONG          ullNodeId;
    PWSTR              pwzPeerId;
    PEER_ADDRESS       address;
} PEER_CONNECTION_INFO;

typedef struct peer_event_incoming_data_tag {
    DWORD                   dwSize;
    ULONGLONG               ullConnectionId;
    GUID                    type;
    PEER_DATA               data;
} PEER_EVENT_INCOMING_DATA, *PPEER_EVENT_INCOMING_DATA;

typedef struct peer_event_record_change_data_tag {
    DWORD                   dwSize;
    PEER_RECORD_CHANGE_TYPE changeType;
    GUID                    recordId;
    GUID                    recordType;
} PEER_EVENT_RECORD_CHANGE_DATA, * PPEER_EVENT_RECORD_CHANGE_DATA;

typedef struct peer_event_connection_change_data_tag {
    DWORD                   dwSize;
    PEER_CONNECTION_STATUS  status;
    ULONGLONG               ullConnectionId;
    ULONGLONG               ullNodeId;
} PEER_EVENT_CONNECTION_CHANGE_DATA, * PPEER_EVENT_CONNECTION_CHANGE_DATA;

typedef struct peer_event_synchronized_data_tag {
    DWORD                   dwSize;
    GUID                    recordType;
} PEER_EVENT_SYNCHRONIZED_DATA, * PPEER_EVENT_SYNCHRONIZED_DATA;


#ifndef   NO_P2P_GRAPH

/////////////////////////////////////////////////////////////////////////////
//
//  G R A P H I N G
//
/////////////////////////////////////////////////////////////////////////////

typedef PVOID HGRAPH, *PHGRAPH;

/////////////////////////////////////////////////////////////////////////////
// Constants

#define PEER_GRAPH_VERSION MAKEWORD(1,0)

typedef enum peer_graph_event_type_tag {
    PEER_GRAPH_EVENT_STATUS_CHANGED             = 1,
    PEER_GRAPH_EVENT_PROPERTY_CHANGED           = 2,
    PEER_GRAPH_EVENT_RECORD_CHANGED             = 3,
    PEER_GRAPH_EVENT_DIRECT_CONNECTION          = 4,
    PEER_GRAPH_EVENT_NEIGHBOR_CONNECTION        = 5,
    PEER_GRAPH_EVENT_INCOMING_DATA              = 6,
    PEER_GRAPH_EVENT_CONNECTION_REQUIRED        = 7,
    PEER_GRAPH_EVENT_NODE_CHANGED               = 8,
    PEER_GRAPH_EVENT_SYNCHRONIZED               = 9,
} PEER_GRAPH_EVENT_TYPE;

typedef enum peer_node_change_type_tag {
    PEER_NODE_CHANGE_CONNECTED              = 1,
    PEER_NODE_CHANGE_DISCONNECTED           = 2,
    PEER_NODE_CHANGE_UPDATED                = 3,
} PEER_NODE_CHANGE_TYPE;

typedef enum peer_graph_status_flags_tag {
    PEER_GRAPH_STATUS_LISTENING            = 0x0001,
    PEER_GRAPH_STATUS_HAS_CONNECTIONS      = 0x0002,
    PEER_GRAPH_STATUS_SYNCHRONIZED         = 0x0004,
} PEER_GRAPH_STATUS_FLAGS;

typedef enum peer_graph_property_flags_tag {
    PEER_GRAPH_PROPERTY_HEARTBEATS         = 0x0001,
    PEER_GRAPH_PROPERTY_DEFER_EXPIRATION   = 0x0002,
} PEER_GRAPH_PROPERTY_FLAGS;

typedef enum peer_graph_scope_tag {
    PEER_GRAPH_SCOPE_ANY                     = 0,
    PEER_GRAPH_SCOPE_GLOBAL                  = 1,
    PEER_GRAPH_SCOPE_SITELOCAL               = 2,
    PEER_GRAPH_SCOPE_LINKLOCAL               = 3,
    PEER_GRAPH_SCOPE_LOOPBACK                = 4
} PEER_GRAPH_SCOPE;


/////////////////////////////////////////////////////////////////////////////
// Structures

typedef struct peer_graph_properties_tag {
    DWORD              dwSize;
    DWORD              dwFlags;          // PEER_GRAPH_PROPERTY_FLAGS
    DWORD              dwScope;
    DWORD              dwMaxRecordSize;
    PWSTR              pwzGraphId;
    PWSTR              pwzCreatorId;
    PWSTR              pwzFriendlyName;
    PWSTR              pwzComment;
    ULONG              ulPresenceLifetime;
    ULONG              cPresenceMax;
} PEER_GRAPH_PROPERTIES, *PPEER_GRAPH_PROPERTIES;

typedef struct peer_node_info_tag {
    DWORD              dwSize;
    ULONGLONG          ullNodeId;
    PWSTR              pwzPeerId;
    ULONG              cAddresses;
    PPEER_ADDRESS      pAddresses;
    PWSTR              pwzAttributes;
} PEER_NODE_INFO, *PPEER_NODE_INFO;

typedef struct peer_event_node_change_data_tag {
    DWORD                       dwSize;
    PEER_NODE_CHANGE_TYPE       changeType;
    ULONGLONG                   ullNodeId;
    PWSTR                       pwzPeerId;
} PEER_EVENT_NODE_CHANGE_DATA, *PPEER_EVENT_NODE_CHANGE_DATA;

typedef struct peer_graph_event_registration_tag {
    PEER_GRAPH_EVENT_TYPE eventType;
    GUID *                pType;
} PEER_GRAPH_EVENT_REGISTRATION, *PPEER_GRAPH_EVENT_REGISTRATION;

typedef struct peer_graph_event_data_tag {
    PEER_GRAPH_EVENT_TYPE eventType;
    union {
       PEER_GRAPH_STATUS_FLAGS  dwStatus;
       PEER_EVENT_INCOMING_DATA  incomingData;
       PEER_EVENT_RECORD_CHANGE_DATA  recordChangeData;
       PEER_EVENT_CONNECTION_CHANGE_DATA  connectionChangeData;
       PEER_EVENT_NODE_CHANGE_DATA  nodeChangeData;
       PEER_EVENT_SYNCHRONIZED_DATA  synchronizedData;
    };
} PEER_GRAPH_EVENT_DATA, *PPEER_GRAPH_EVENT_DATA;


typedef HRESULT (CALLBACK * PFNPEER_VALIDATE_RECORD)(
                IN HGRAPH                   hGraph,
                IN PVOID                    pvContext,
                IN PPEER_RECORD             pRecord,
                IN PEER_RECORD_CHANGE_TYPE  changeType);

typedef HRESULT (CALLBACK * PFNPEER_SECURE_RECORD)(
                IN HGRAPH       hGraph,
                IN PVOID        pvContext,
                IN PPEER_RECORD pRecord,
                IN PEER_RECORD_CHANGE_TYPE  changeType,
                OUT PPEER_DATA * ppSecurityData);

typedef HRESULT (CALLBACK * PFNPEER_FREE_SECURITY_DATA)(
                IN HGRAPH       hGraph,
                IN PVOID        pvContext,
                IN PPEER_DATA   pSecurityData);

typedef struct peer_security_interface_tag {
    DWORD                       dwSize;
    PWSTR                       pwzSspFilename;
    PWSTR                       pwzPackageName;
    ULONG                       cbSecurityInfo;
    PBYTE                       pbSecurityInfo;
    PVOID                       pvContext;
    PFNPEER_VALIDATE_RECORD     pfnValidateRecord;
    PFNPEER_SECURE_RECORD       pfnSecureRecord;
    PFNPEER_FREE_SECURITY_DATA  pfnFreeSecurityData;
}PEER_SECURITY_INTERFACE, * PPEER_SECURITY_INTERFACE;


/////////////////////////////////////////////////////////////////////////////
// API Definitions

HRESULT WINAPI PeerGraphStartup(
                IN  WORD  wVersionRequested,
                OUT PPEER_VERSION_DATA pVersionData);

HRESULT WINAPI PeerGraphShutdown();

VOID WINAPI PeerGraphFreeData(
                IN  PVOID       pvData);

HRESULT WINAPI PeerGraphGetItemCount(
                IN  HPEERENUM   hPeerEnum,
                OUT PULONG      pCount);

HRESULT WINAPI PeerGraphGetNextItem(
                IN  HPEERENUM   hPeerEnum,
                IN OUT PULONG   pCount,
                OUT PVOID     * ppvItems);

HRESULT WINAPI PeerGraphEndEnumeration(
                IN  HPEERENUM   hPeerEnum);

HRESULT WINAPI PeerGraphCreate(
                IN  PPEER_GRAPH_PROPERTIES   pGraphProperties,
                IN  PCWSTR      pwzDatabaseName,
                IN  PPEER_SECURITY_INTERFACE pSecurityInterface,
                OUT PHGRAPH     phGraph);

HRESULT WINAPI PeerGraphOpen(
                IN  PCWSTR      pwzGraphId,
                IN  PCWSTR      pwzPeerId,
                IN  PCWSTR      pwzDatabaseName,
                IN  PPEER_SECURITY_INTERFACE pSecurityInterface,
                IN  ULONG       cRecordTypeSyncPrecedence,
                IN  GUID *      pRecordTypeSyncPrecedence,
                OUT PHGRAPH     phGraph);

HRESULT WINAPI PeerGraphListen(
                IN  HGRAPH      hGraph,
                IN  DWORD       dwScope,
                IN  DWORD       dwScopeId,
                IN  WORD        wPort);

HRESULT WINAPI PeerGraphConnect(
                IN  HGRAPH      hGraph,
                IN  PCWSTR      pwzPeerId,
                IN  PPEER_ADDRESS pAddress,
                OUT ULONGLONG * pullConnectionId);

HRESULT WINAPI PeerGraphClose(
                IN  HGRAPH      hGraph);

HRESULT WINAPI PeerGraphDelete(
                IN  PCWSTR      pwzGraphId,
                IN  PCWSTR      pwzPeerId,
                IN  PCWSTR      pwzDatabaseName);

HRESULT WINAPI PeerGraphGetStatus(
                IN  HGRAPH       hGraph,
                OUT DWORD      * pdwStatus);

HRESULT WINAPI PeerGraphGetProperties(
                IN  HGRAPH      hGraph,
                OUT PPEER_GRAPH_PROPERTIES * ppGraphProperties);

HRESULT WINAPI PeerGraphSetProperties(
                IN  HGRAPH      hGraph,
                IN  PPEER_GRAPH_PROPERTIES pGraphProperties);

// Eventing interfaces

HRESULT WINAPI PeerGraphRegisterEvent(
                IN  HGRAPH       hGraph,
                IN  HANDLE       hEvent,
                IN  ULONG        cEventRegistrations,
                IN  PPEER_GRAPH_EVENT_REGISTRATION pEventRegistrations,
                OUT HPEEREVENT * phPeerEvent);

HRESULT WINAPI PeerGraphUnregisterEvent(
                IN  HPEEREVENT   hPeerEvent);

HRESULT WINAPI PeerGraphGetEventData(
                IN  HPEEREVENT   hPeerEvent,
                OUT PPEER_GRAPH_EVENT_DATA * ppEventData);


// Data Storage

HRESULT WINAPI PeerGraphGetRecord(
                IN  HGRAPH       hGraph,
                IN  GUID *       pRecordId,
                OUT PPEER_RECORD * ppRecord);

HRESULT WINAPI PeerGraphAddRecord(
                IN  HGRAPH       hGraph,
                IN  PPEER_RECORD pRecord,
                OUT GUID       * pRecordId);

HRESULT WINAPI PeerGraphUpdateRecord(
                IN  HGRAPH       hGraph,
                IN  PPEER_RECORD pRecord);

HRESULT WINAPI PeerGraphDeleteRecord(
                IN  HGRAPH       hGraph,
                IN  GUID *       pRecordId,
                IN  BOOL         fLocal);

HRESULT WINAPI PeerGraphEnumRecords(
                IN  HGRAPH       hGraph,
                IN  GUID *       pRecordType,
                IN  PCWSTR       pwzPeerId,
                OUT HPEERENUM *  phPeerEnum);

HRESULT WINAPI PeerGraphSearchRecords(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzCriteria,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGraphExportDatabase(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzFilePath);

HRESULT WINAPI PeerGraphImportDatabase(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzFilePath);

HRESULT WINAPI PeerGraphValidateDeferredRecords(
                IN  HGRAPH       hGraph,
                IN  ULONG        cRecordIds,
                IN  GUID *       pRecordIds);


// Node/Connection interfaces

HRESULT WINAPI PeerGraphOpenDirectConnection(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzPeerId,
                IN  PPEER_ADDRESS pAddress,
                OUT ULONGLONG *  pullConnectionId);

HRESULT WINAPI PeerGraphSendData(
                IN  HGRAPH       hGraph,
                IN  ULONGLONG    ullConnectionId,
                IN  GUID *       pType,
                IN  ULONG        cbData,
                IN  PVOID        pvData);

HRESULT WINAPI PeerGraphCloseDirectConnection(
                IN  HGRAPH       hGraph,
                IN  ULONGLONG    ullConnectionId);

HRESULT WINAPI PeerGraphEnumConnections(
                IN  HGRAPH       hGraph,
                IN  DWORD        dwFlags,         // PEER_CONNECTION_FLAGS
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGraphEnumNodes(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzPeerId,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGraphSetPresence(
                IN  HGRAPH       hGraph,
                IN  BOOL         fPresent);

HRESULT WINAPI PeerGraphGetNodeInfo(
                IN  HGRAPH       hGraph,
                IN  ULONGLONG    ullNodeId,
                OUT PPEER_NODE_INFO * ppNodeInfo);

HRESULT WINAPI PeerGraphSetNodeAttributes(
                IN  HGRAPH       hGraph,
                IN  PCWSTR       pwzAttributes);

HRESULT WINAPI PeerGraphPeerTimeToUniversalTime(
                IN  HGRAPH       hGraph,
                IN  FILETIME *   pftPeerTime,
                OUT FILETIME *   pftUniversalTime);

HRESULT WINAPI PeerGraphUniversalTimeToPeerTime(
                IN  HGRAPH       hGraph,
                IN  FILETIME *   pftUniversalTime,
                OUT FILETIME *   pftPeerTime);

#endif // NO_P2P_GRAPH


#if (!(defined (NO_P2P_GROUP)) || (!defined (NO_P2P_IDENTITY)))
//////////////////////////////////////////////////////////////////////////////
//
//  Common APIs used by Grouping and the Identity Manager
//
//////////////////////////////////////////////////////////////////////////////

#include <wincrypt.h>

VOID    WINAPI PeerFreeData(
                IN  PVOID        pvData);

HRESULT WINAPI PeerGetItemCount(
                IN  HPEERENUM    hPeerEnum,
                OUT PULONG       pCount);

HRESULT WINAPI PeerGetNextItem(
                IN  HPEERENUM    hPeerEnum,
                IN OUT PULONG    pCount,
                OUT PVOID      * ppvItems);

HRESULT WINAPI PeerEndEnumeration(
                IN  HPEERENUM    hPeerEnum);

#endif

#ifndef NO_P2P_GROUP
/////////////////////////////////////////////////////////////////////////////
//
//  G R O U P I N G
//
/////////////////////////////////////////////////////////////////////////////

typedef PVOID HGROUP, *PHGROUP;

typedef GUID  PEER_ROLE_ID;

/////////////////////////////////////////////////////////////////////////////
// Constants

#define PEER_GROUP_VERSION MAKEWORD(1,0)

typedef enum peer_group_event_type_tag {
    PEER_GROUP_EVENT_STATUS_CHANGED             = 1,
    PEER_GROUP_EVENT_PROPERTY_CHANGED           = 2,
    PEER_GROUP_EVENT_RECORD_CHANGED             = 3,
    PEER_GROUP_EVENT_DIRECT_CONNECTION          = 4,
    PEER_GROUP_EVENT_NEIGHBOR_CONNECTION        = 5,
    PEER_GROUP_EVENT_INCOMING_DATA              = 6,
    PEER_GROUP_EVENT_MEMBER_CHANGED             = 8,
    PEER_GROUP_EVENT_CONNECTION_FAILED          = 10
} PEER_GROUP_EVENT_TYPE;

typedef enum peer_group_status_tag {
    PEER_GROUP_STATUS_LISTENING            = 0x0001,
    PEER_GROUP_STATUS_HAS_CONNECTIONS      = 0x0002,
} PEER_GROUP_STATUS;

typedef enum peer_group_property_flags_tag {
    PEER_MEMBER_DATA_OPTIONAL              = 0x0001,
    PEER_DISABLE_PRESENCE                  = 0x0002,
    PEER_DEFER_EXPIRATION                  = 0x0004,
} PEER_GROUP_PROPERTY_FLAGS;

typedef enum peer_member_flags_tag {
    PEER_MEMBER_PRESENT                    = 0x0001,
} PEER_MEMBER_FLAGS;

typedef enum peer_member_change_type_tag {
    PEER_MEMBER_CONNECTED                  = 1,
    PEER_MEMBER_DISCONNECTED               = 2,
    PEER_MEMBER_UPDATED                    = 3,
    PEER_MEMBER_JOINED                     = 4,
    PEER_MEMBER_LEFT                       = 5,
} PEER_MEMBER_CHANGE_TYPE;

typedef enum peer_issue_credential_flags_tag {
    PEER_GROUP_STORE_CREDENTIALS      = 0x0001,
} PEER_GROUP_ISSUE_CREDENTIAL_FLAGS;


/////////////////////////////////////////////////////////////////////////////
// Structures

typedef struct peer_credential_info_tag
{
    DWORD                   dwSize;
    DWORD                   dwFlags;
    PWSTR                   pwzFriendlyName;
    CERT_PUBLIC_KEY_INFO  * pPublicKey;
    PWSTR                   pwzIssuerPeerName;
    PWSTR                   pwzIssuerFriendlyName;
    FILETIME                ftValidityStart;
    FILETIME                ftValidityEnd;
    ULONG                   cRoles;
    PEER_ROLE_ID          * pRoles;
} PEER_CREDENTIAL_INFO, * PPEER_CREDENTIAL_INFO;

typedef struct peer_member_tag {
    DWORD                   dwSize;
    DWORD                   dwFlags;     // PEER_MEMBER_FLAGS
    PWSTR                   pwzIdentity;
    PWSTR                   pwzAttributes;
    ULONGLONG               ullNodeId;
    ULONG                   cAddresses;
    PEER_ADDRESS          * pAddresses;
    PEER_CREDENTIAL_INFO  * pCredentialInfo;
} PEER_MEMBER, *PPEER_MEMBER;

typedef struct peer_group_properties_tag {
    DWORD            dwSize;
    DWORD            dwFlags;            // PEER_GROUP_PROPERTY_FLAGS
    PWSTR            pwzCloud;
    PWSTR            pwzClassifier;
    PWSTR            pwzGroupPeerName;
    PWSTR            pwzCreatorPeerName;
    PWSTR            pwzFriendlyName;
    PWSTR            pwzComment;
    ULONG            ulMemberDataLifetime;
    ULONG            ulPresenceLifetime;
} PEER_GROUP_PROPERTIES, *PPEER_GROUP_PROPERTIES;

typedef struct peer_invitation_info_tag {
    DWORD            dwSize;
    DWORD            dwFlags;
    PWSTR            pwzCloudName;
    DWORD            dwScope;
    DWORD            dwCloudFlags;
    PWSTR            pwzGroupPeerName;
    PWSTR            pwzIssuerPeerName;
    PWSTR            pwzSubjectPeerName;
    PWSTR            pwzGroupFriendlyName;
    PWSTR            pwzIssuerFriendlyName;
    PWSTR            pwzSubjectFriendlyName;
    FILETIME         ftValidityStart;
    FILETIME         ftValidityEnd;
    ULONG            cRoles;
    PEER_ROLE_ID   * pRoles;
    ULONG            cClassifiers;
    PWSTR*           ppwzClassifiers;
    CERT_PUBLIC_KEY_INFO * pSubjectPublicKey;
} PEER_INVITATION_INFO, *PPEER_INVITATION_INFO;

typedef struct peer_event_member_change_data_tag {
    DWORD                   dwSize;
    PEER_MEMBER_CHANGE_TYPE changeType;
    PWSTR                   pwzIdentity;
} PEER_EVENT_MEMBER_CHANGE_DATA, * PPEER_EVENT_MEMBER_CHANGE_DATA;

typedef struct peer_group_event_registration_tag {
    PEER_GROUP_EVENT_TYPE eventType;
    GUID *                pType;
} PEER_GROUP_EVENT_REGISTRATION, *PPEER_GROUP_EVENT_REGISTRATION;

typedef struct peer_group_event_data_tag {
    PEER_GROUP_EVENT_TYPE eventType;
    union {
       PEER_GROUP_STATUS                 dwStatus;
       PEER_EVENT_INCOMING_DATA          incomingData;
       PEER_EVENT_RECORD_CHANGE_DATA     recordChangeData;
       PEER_EVENT_CONNECTION_CHANGE_DATA connectionChangeData;
       PEER_EVENT_MEMBER_CHANGE_DATA     memberChangeData;
       HRESULT                           hrConnectionFailedReason;
    };
} PEER_GROUP_EVENT_DATA, *PPEER_GROUP_EVENT_DATA;

typedef struct peer_name_pair_tag {
    DWORD           dwSize;
    PWSTR           pwzPeerName;
    PWSTR           pwzFriendlyName;
} PEER_NAME_PAIR, * PPEER_NAME_PAIR;


/////////////////////////////////////////////////////////////////////////////
// API Definitions

HRESULT WINAPI PeerGroupStartup(
                IN  WORD         wVersionRequested,
                OUT PPEER_VERSION_DATA pVersionData);

HRESULT WINAPI PeerGroupShutdown();

HRESULT WINAPI PeerGroupCreate(
                IN  PPEER_GROUP_PROPERTIES pProperties,
                OUT HGROUP     * phGroup);

HRESULT WINAPI PeerGroupOpen(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzGroupPeerName,
                IN  PCWSTR       pwzCloud,
                OUT HGROUP     * phGroup);

HRESULT WINAPI PeerGroupJoin(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzInvitation,
                IN  PCWSTR       pwzCloud,
                OUT HGROUP     * phGroup);

HRESULT WINAPI PeerGroupConnect(
                IN  HGROUP       hGroup);

HRESULT WINAPI PeerGroupClose(
                IN  HGROUP       hGroup);

HRESULT WINAPI PeerGroupDelete(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzGroupPeerName);

HRESULT WINAPI PeerGroupCreateInvitation(
                IN  HGROUP       hGroup,
                IN  PCWSTR       pwzIdentityInfo,
                IN  FILETIME   * pftExpiration,
                IN  ULONG        cRoles,
                IN  GUID       * pRoles,
                OUT PWSTR      * ppwzInvitation);

HRESULT WINAPI PeerGroupParseInvitation(
                IN  PCWSTR       pwzInvitation,
                OUT PPEER_INVITATION_INFO * ppInvitationInfo);

HRESULT WINAPI PeerGroupGetStatus(
                IN  HGROUP       hGroup,
                OUT DWORD      * pdwStatus);

HRESULT WINAPI PeerGroupGetProperties(
                IN  HGROUP       hGroup,
                OUT PPEER_GROUP_PROPERTIES * ppProperties);

HRESULT WINAPI PeerGroupSetProperties(
                IN  HGROUP       hGroup,
                IN  PPEER_GROUP_PROPERTIES pProperties);

HRESULT WINAPI PeerGroupEnumMembers(
                IN  HGROUP       hGroup,
                IN  DWORD        dwFlags,      // PEER_MEMBER_FLAGS
                IN  PCWSTR       pwzIdentity,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGroupOpenDirectConnection(
                IN  HGROUP       hGroup,
                IN  PCWSTR       pwzIdentity,
                IN  PPEER_ADDRESS pAddress,
                OUT ULONGLONG  * pullConnectionId);

HRESULT WINAPI PeerGroupCloseDirectConnection(
                IN  HGROUP       hGroup,
                IN  ULONGLONG    ullConnectionId);

HRESULT WINAPI PeerGroupEnumConnections(
                IN  HGROUP       hGroup,
                IN  DWORD        dwFlags,         // PEER_CONNECTION_FLAGS
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGroupSendData(
                IN  HGROUP       hGroup,
                IN  ULONGLONG    ullConnectionId,
                IN  GUID *       pType,
                IN  ULONG        cbData,
                IN  PVOID        pvData);


// Eventing interfaces

HRESULT WINAPI PeerGroupRegisterEvent(
                IN  HGROUP       hGroup,
                IN  HANDLE       hEvent,
                IN  DWORD        cEventRegistration,
                IN  PPEER_GROUP_EVENT_REGISTRATION pEventRegistrations,
                OUT HPEEREVENT * phPeerEvent);

HRESULT WINAPI PeerGroupUnregisterEvent(
                IN  HPEEREVENT   hPeerEvent);

HRESULT WINAPI PeerGroupGetEventData(
                IN  HPEEREVENT   hPeerEvent,
                OUT PPEER_GROUP_EVENT_DATA * ppEventData);


// Data Storage

HRESULT WINAPI PeerGroupGetRecord(
                IN  HGROUP       hGroup,
                IN  GUID *       pRecordId,
                OUT PPEER_RECORD * ppRecord);

HRESULT WINAPI PeerGroupAddRecord(
                IN  HGROUP       hGroup,
                IN  PPEER_RECORD pRecord,
                OUT GUID       * pRecordId);

HRESULT WINAPI PeerGroupUpdateRecord(
                IN  HGROUP       hGroup,
                IN  PPEER_RECORD pRecord);

HRESULT WINAPI PeerGroupDeleteRecord(
                IN  HGROUP       hGroup,
                IN  GUID *       pRecordId);

HRESULT WINAPI PeerGroupEnumRecords(
                IN  HGROUP       hGroup,
                IN  GUID *       pRecordType,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGroupSearchRecords(
                IN  HGROUP       hGroup,
                IN  PCWSTR       pwzCriteria,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI PeerGroupExportDatabase(
                IN  HGROUP       hGroup,
                IN  PCWSTR       pwzFilePath);

HRESULT WINAPI PeerGroupImportDatabase(
                IN  HGROUP       hGroup,
                IN  PCWSTR       pwzFilePath);

HRESULT WINAPI PeerGroupIssueCredentials(
                IN HGROUP        hGroup,
                IN PCWSTR        pwzSubjectIdentity,
                IN PEER_CREDENTIAL_INFO * pCredentialInfo,
                IN DWORD         dwFlags,
                OUT PWSTR      * ppwzInvitation);

HRESULT WINAPI PeerGroupExportConfig(
                IN  HGROUP      hGroup, 
                IN  PCWSTR      pwzPassword, 
                OUT PWSTR*      ppwzXML);

HRESULT WINAPI PeerGroupImportConfig(
                IN PCWSTR       pwzXML,
                IN PCWSTR       pwzPassword, 
                IN BOOL         fOverwrite, 
                OUT PWSTR*      ppwzIdentity, 
                OUT PWSTR*      ppwzGroup);

HRESULT WINAPI PeerGroupPeerTimeToUniversalTime(
                IN  HGROUP       hGroup,
                IN  FILETIME *   pftPeerTime,
                OUT FILETIME *   pftUniversalTime);

HRESULT WINAPI PeerGroupUniversalTimeToPeerTime(
                IN  HGROUP       hGroup,
                IN  FILETIME *   pftUniversalTime,
                OUT FILETIME *   pftPeerTime);

#endif // NO_P2P_GROUP


#ifndef   NO_P2P_IDENTITY

/////////////////////////////////////////////////////////////////////////////
//
//  I D E N T I T Y
//
/////////////////////////////////////////////////////////////////////////////

// API Definitions

HRESULT WINAPI PeerIdentityCreate(
                IN  PCWSTR       pwzClassifier,
                IN  PCWSTR       pwzFriendlyName,
                IN  HCRYPTPROV   hCryptProv,
                OUT PWSTR      * ppwzIdentity);

HRESULT WINAPI  PeerIdentityGetFriendlyName(
                IN  PCWSTR       pwzIdentity,
                OUT PWSTR      * ppwzFriendlyName);

HRESULT WINAPI  PeerIdentitySetFriendlyName(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzFriendlyName);

HRESULT WINAPI  PeerIdentityGetCryptKey(
                IN  PCWSTR       pwzIdentity,
                OUT HCRYPTPROV * phCryptProv);

HRESULT WINAPI  PeerIdentityDelete(
                IN  PCWSTR       pwzIdentity);

HRESULT WINAPI  PeerEnumIdentities(
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI  PeerEnumGroups(
                IN  PCWSTR       pwzIdentity,
                OUT HPEERENUM  * phPeerEnum);

HRESULT WINAPI  PeerCreatePeerName(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzClassifier,
                OUT PWSTR      * ppwzPeerName);

HRESULT WINAPI PeerIdentityGetXML(
                IN  PCWSTR       pwzIdentity,
                OUT PWSTR      * ppwzIdentityXML);

HRESULT WINAPI PeerIdentityExport(
                IN  PCWSTR       pwzIdentity,
                IN  PCWSTR       pwzPassword,
                OUT PWSTR      * ppwzExportXML);

HRESULT WINAPI PeerIdentityImport(
                IN  PCWSTR       pwzImportXML,
                IN  PCWSTR       pwzPassword,
                OUT PWSTR      * ppwzIdentity);

#endif // NO_P2P_IDENTITY


#ifdef __cplusplus
}
#endif  /* __cplusplus */


/////////////////////////////////////////////////////////////////////////////
//
// Error codes
//
/////////////////////////////////////////////////////////////////////////////

#ifndef FACILITY_P2P
#define FACILITY_P2P  99
#endif

#define PEER_E_IPV6_NOT_INSTALLED            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0001)
#define PEER_E_NOT_INITIALIZED               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0002)
#define PEER_E_INVALID_GRAPH                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0010)
#define PEER_E_DBNAME_CHANGED                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0011)
#define PEER_E_DUPLICATE_GRAPH               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0012)
#define PEER_E_GRAPH_NOT_READY               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0013)
#define PEER_E_GRAPH_SHUTTING_DOWN           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0014)
#define PEER_E_GRAPH_IN_USE                  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0015)
#define PEER_E_INVALID_DATABASE              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0016)

#define PEER_E_LINK_PERSISTENT               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0100)
#define PEER_E_ALREADY_CONNECTED             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0101)
#define PEER_E_TOO_MANY_LINKS                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0102)
#define PEER_E_CONNECTION_NOT_FOUND          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0103)
#define PEER_E_NOT_LISTENING                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0104)
#define PEER_E_PRESENCE_DISABLED             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0105)
#define PEER_E_CONNECT_SELF                  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0106)
#define PEER_E_ALREADY_LISTENING             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0107)
#define PEER_E_NODE_NOT_FOUND                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0108)

#define PEER_E_CLASSIFIER_TOO_LONG           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0201)
#define PEER_E_TOO_MANY_IDENTITIES           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0202)
#define PEER_E_NO_KEY_ACCESS                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0203)
#define PEER_E_GROUPS_EXIST                  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0204)

// record error codes
#define PEER_E_RECORD_NOT_FOUND              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0301)
#define PEER_E_DATABASE_ACCESSDENIED         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0302)
#define PEER_E_DBINITIALIZATION_FAILED       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0303)
#define PEER_E_MAX_RECORD_SIZE_EXCEEDED      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0304)
#define PEER_E_DATABASE_ALREADY_PRESENT      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0305)
#define PEER_E_DATABASE_NOT_PRESENT          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0306)

#define PEER_E_IDENTITY_NOT_FOUND            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0401)

// eventing error
#define PEER_E_EVENT_HANDLE_NOT_FOUND        MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0501)

// searching error
#define PEER_E_INVALID_SEARCH                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0601)
#define PEER_E_INVALID_ATTRIBUTES            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0602)

// certificate verification error codes
#define PEER_E_INVITATION_NOT_TRUSTED        MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0701)
#define PEER_E_CERTIFICATE_NOT_TRUSTED       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0702)
#define PEER_E_CHAIN_TOO_LONG                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0703)

#define PEER_E_CERT_STORE_CORRUPTED          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x0801)

#define PEER_E_NO_CLOUD                      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x1001)
#define PEER_E_CLOUD_NAME_AMBIGUOUS          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x1005)

#define PEER_E_INVALID_RECORD                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2010)
#define PEER_E_UNDEFINED_RECORD_TYPE         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2011)
#define PEER_E_NOT_AUTHORIZED                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2020)
#define PEER_E_DEFERRED_VALIDATION           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2030)
#define PEER_E_INVALID_GROUP_PROPERTIES      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2040)
#define PEER_E_INVALID_PEER_NAME             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2050)
#define PEER_E_INVALID_CLASSIFIER            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2060)
#define PEER_E_INVALID_FRIENDLY_NAME         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2070)
#define PEER_E_INVALID_ROLE_PROPERTY         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2071)
#define PEER_E_INVALID_CLASSIFIER_PROPERTY   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2072)
#define PEER_E_INVALID_RECORD_EXPIRATION     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2080)
#define PEER_E_INVALID_CREDENTIAL_INFO       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2081)
#define PEER_E_INVALID_CREDENTIAL            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2082)
#define PEER_E_INVALID_RECORD_SIZE           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2083)
#define PEER_E_UNSUPPORTED_VERSION           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2090)
#define PEER_E_GROUP_NOT_READY               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2091)
#define PEER_E_GROUP_IN_USE                  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2092)
#define PEER_E_INVALID_GROUP                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2093)
#define PEER_E_NO_MEMBERS_FOUND              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2094)
#define PEER_E_NO_MEMBER_CONNECTIONS         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2095)
#define PEER_E_UNABLE_TO_LISTEN              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x2096)
#define PEER_E_IDENTITY_DELETED              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_P2P, 0x20A0)

// Special success codes
#define PEER_S_GRAPH_DATA_CREATED            MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_P2P, 0x0001)
#define PEER_S_NO_EVENT_DATA                 MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_P2P, 0x0002)
#define PEER_S_ALREADY_CONNECTED             MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_P2P, 0x2000)

//
// WSA_PNRP_* error codes returned by the NSP
//

// These errors are only used by PNRP NSP and applications using the NSP.
// Any usage from other system/apps not related to PNRP (for example PNRP based getaddrinfo
// implementations) would require these errors be moved to winerror.h

#define WSA_PNRP_ERROR_BASE                  11500
#define WSA_PNRP_CLOUD_NOT_FOUND             (WSA_PNRP_ERROR_BASE +  1)
#define WSA_PNRP_CLOUD_DISABLED              (WSA_PNRP_ERROR_BASE +  2)
#define WSA_PNRP_INVALID_IDENTITY            (WSA_PNRP_ERROR_BASE +  3)
#define WSA_PNRP_TOO_MUCH_LOAD               (WSA_PNRP_ERROR_BASE +  4)

//
// these are the error codes that are not defined by winerror.h, so we define them as our own
//
#define PEER_E_NOT_FOUND                     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_NOT_FOUND)
#define PEER_E_DISK_FULL                     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_DISK_FULL)
#define PEER_E_ALREADY_EXISTS                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_ALREADY_EXISTS)

#endif  // _P2P_H_


/////////////////////////////////////////////////////////////////////////////
//
// These GUIDs are outside conditional includes so you can
//   #include <p2p.h>   in precompiled header
// then
//   #include <initguid.h>  in a single source file
//   #include <p2p.h>   in that source file a second time to instantiate the GUIDs

#ifdef DEFINE_GUID

#ifndef NO_P2P_GROUP

DEFINE_GUID(PEER_GROUP_ROLE_ADMIN,     /* 04387127-aa56-450a-8ce5-4f565c6790f4 */
    0x04387127, 0xaa56, 0x450a, 0x8c, 0xe5, 0x4f, 0x56, 0x5c, 0x67, 0x90, 0xf4);

DEFINE_GUID(PEER_GROUP_ROLE_MEMBER,    /* f12dc4c7-0857-4ca0-93fc-b1bb19a3d8c2 */
    0xf12dc4c7, 0x0857, 0x4ca0, 0x93, 0xfc, 0xb1, 0xbb, 0x19, 0xa3, 0xd8, 0xc2);

#endif // NO_P2P_GROUP

#endif // DEFINE_GUID
