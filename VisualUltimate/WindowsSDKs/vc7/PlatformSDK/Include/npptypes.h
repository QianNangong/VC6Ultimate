//=============================================================================
//  Microsoft (R) Network Monitor (tm). 
//  Copyright (C) 1997-1999. All rights reserved.
//
//  MODULE: NPPTypes.h
//
//  The source of everything outsiders need to know about NPP
//=============================================================================

#ifndef __npptypes_h__
#define __npptypes_h__

#include <unknwn.h>


#ifndef LPBYTE
typedef BYTE *LPBYTE;
#endif

typedef const void * HBLOB;

#pragma pack(1)

//=============================================================================
// DWORD  General constants.
//=============================================================================

#define MAC_TYPE_UNKNOWN                             0     //... not supported
#define MAC_TYPE_ETHERNET                            1     //... ethernet and 802.3
#define MAC_TYPE_TOKENRING                           2     //... tokenring (802.5)
#define MAC_TYPE_FDDI                                3     //... fddi.
                                                     
#define MACHINE_NAME_LENGTH                         16 
#define USER_NAME_LENGTH                            32 
#define ADAPTER_COMMENT_LENGTH                      32 
                                                
#define CONNECTION_FLAGS_WANT_CONVERSATION_STATS    0x00000001


//=============================================================================
//  Handle to a network
//=============================================================================
typedef DWORD HNETWORK;

//=============================================================================
//  Statistics structure.
//=============================================================================

// -----------
// Structure
// -----------
typedef struct _STATISTICS
{
    __int64          TimeElapsed; // in millionths of a second

    //...Buffer statistics
    DWORD           TotalFramesCaptured;
    DWORD           TotalBytesCaptured;

    //...Filtered statistics
    DWORD           TotalFramesFiltered;
    DWORD           TotalBytesFiltered;
    DWORD           TotalMulticastsFiltered;
    DWORD           TotalBroadcastsFiltered;

    //...Overall statistics.
    DWORD           TotalFramesSeen;
    DWORD           TotalBytesSeen;
    DWORD           TotalMulticastsReceived;
    DWORD           TotalBroadcastsReceived;
    DWORD           TotalFramesDropped;
    DWORD           TotalFramesDroppedFromBuffer;
    
    //... Statistics kept by MAC driver.
    DWORD           MacFramesReceived;
    DWORD           MacCRCErrors;
    __int64         MacBytesReceivedEx;
    DWORD           MacFramesDropped_NoBuffers;
    DWORD           MacMulticastsReceived;
    DWORD           MacBroadcastsReceived;
    DWORD           MacFramesDropped_HwError;
} STATISTICS;

typedef STATISTICS *LPSTATISTICS;

#define STATISTICS_SIZE    sizeof(STATISTICS)

//=============================================================================
//  Address structures
//=============================================================================

// -----------
// Constants
// -----------
#define MAX_NAME_SIZE       32 
#define IP_ADDRESS_SIZE     4  
#define MAC_ADDRESS_SIZE    6  

// Q: What is the maximum address size that we could have to copy?
// A: IPX == DWORD + 6 bytes == 10
#define MAX_ADDRESS_SIZE    10 
                                    
#define ADDRESS_TYPE_ETHERNET       0 
#define ADDRESS_TYPE_IP             1 
#define ADDRESS_TYPE_IPX            2 
#define ADDRESS_TYPE_TOKENRING      3 
#define ADDRESS_TYPE_FDDI           4 

#define ADDRESS_TYPE_XNS            5 
#define ADDRESS_TYPE_ANY            6   // NOTE: Special ANY and ANY_GROUP are placeholders, meaning, ignore this type
#define ADDRESS_TYPE_ANY_GROUP      7   
#define ADDRESS_TYPE_FIND_HIGHEST   8   
#define ADDRESS_TYPE_VINES_IP       9   
#define ADDRESS_TYPE_LOCAL_ONLY     10   


#define ADDRESSTYPE_FLAGS_NORMALIZE     0x0001 
#define ADDRESSTYPE_FLAGS_BIT_REVERSE   0x0002 


// -----------
// Structure
// -----------
// Vines IP Address Structure
typedef struct _VINES_IP_ADDRESS
{
    DWORD   NetID;
    WORD    SubnetID;
} VINES_IP_ADDRESS;
typedef VINES_IP_ADDRESS *LPVINES_IP_ADDRESS;
#define VINES_IP_ADDRESS_SIZE sizeof(VINES_IP_ADDRESS)

// IPX Address Structure
typedef struct _IPX_ADDRESS
{
    BYTE    Subnet[4];
    BYTE    Address[6];
} IPX_ADDRESS;
typedef IPX_ADDRESS *LPIPX_ADDRESS;
#define IPX_ADDRESS_SIZE    sizeof(IPX_ADDRESS)

// XNS Address Structure
typedef IPX_ADDRESS XNS_ADDRESS;
typedef IPX_ADDRESS *LPXNS_ADDRESS;

// ETHERNET SOURCE ADDRESS
typedef struct _ETHERNET_SRC_ADDRESS
{
    BYTE    RoutingBit:     1;
    BYTE    LocalBit:       1;
    BYTE    Byte0:          6;
    BYTE    Reserved[5];
} ETHERNET_SRC_ADDRESS;
typedef ETHERNET_SRC_ADDRESS *LPETHERNET_SRC_ADDRESS;

// ETHERNET DESTINATION ADDRESS
typedef struct _ETHERNET_DST_ADDRESS
{
    BYTE    GroupBit:       1;
    BYTE    AdminBit:       1;
    BYTE    Byte0:          6;
    BYTE    Reserved[5];
} ETHERNET_DST_ADDRESS;
typedef ETHERNET_DST_ADDRESS *LPETHERNET_DST_ADDRESS;

// FDDI addresses
typedef ETHERNET_SRC_ADDRESS FDDI_SRC_ADDRESS;
typedef ETHERNET_DST_ADDRESS FDDI_DST_ADDRESS;

typedef FDDI_SRC_ADDRESS *LPFDDI_SRC_ADDRESS;
typedef FDDI_DST_ADDRESS *LPFDDI_DST_ADDRESS;

// TOKENRING Source Address
typedef struct _TOKENRING_SRC_ADDRESS
{
    BYTE    Byte0:          6;
    BYTE    LocalBit:       1;
    BYTE    RoutingBit:     1;
    BYTE    Byte1;
    BYTE    Byte2:          7;
    BYTE    Functional:     1;
    BYTE    Reserved[3];
} TOKENRING_SRC_ADDRESS;
typedef TOKENRING_SRC_ADDRESS *LPTOKENRING_SRC_ADDRESS;

// TOKENRING Destination Address
typedef struct _TOKENRING_DST_ADDRESS
{
    BYTE    Byte0:          6;
    BYTE    AdminBit:       1;
    BYTE    GroupBit:       1;
    BYTE    Reserved[5];
} TOKENRING_DST_ADDRESS;
typedef TOKENRING_DST_ADDRESS *LPTOKENRING_DST_ADDRESS;



// Address Structure
typedef struct _ADDRESS
{
    DWORD                       Type;

    union
    {
        // ADDRESS_TYPE_ETHERNET
        // ADDRESS_TYPE_TOKENRING
        // ADDRESS_TYPE_FDDI
        BYTE                    MACAddress[MAC_ADDRESS_SIZE];

        // IP
        BYTE                    IPAddress[IP_ADDRESS_SIZE];

        // raw IPX
        BYTE                    IPXRawAddress[IPX_ADDRESS_SIZE];

        // real IPX
        IPX_ADDRESS             IPXAddress;

        // raw Vines IP
        BYTE                    VinesIPRawAddress[VINES_IP_ADDRESS_SIZE];

        // real Vines IP
        VINES_IP_ADDRESS        VinesIPAddress;

        // ethernet with bits defined
        ETHERNET_SRC_ADDRESS    EthernetSrcAddress;

        // ethernet with bits defined
        ETHERNET_DST_ADDRESS    EthernetDstAddress;

        // tokenring with bits defined
        TOKENRING_SRC_ADDRESS   TokenringSrcAddress;

        // tokenring with bits defined
        TOKENRING_DST_ADDRESS   TokenringDstAddress;

        // fddi with bits defined
        FDDI_SRC_ADDRESS        FddiSrcAddress;

        // fddi with bits defined
        FDDI_DST_ADDRESS        FddiDstAddress;
    };
    
    WORD                        Flags;
} ADDRESS;
typedef ADDRESS *LPADDRESS;
#define ADDRESS_SIZE   sizeof(ADDRESS)

//=============================================================================
//  Address Pair Structure
//=============================================================================

// -----------
// Constants
// -----------
#define ADDRESS_FLAGS_MATCH_DST         0x0001 //... Match dest. address.
#define ADDRESS_FLAGS_MATCH_SRC         0x0002 //... Match src. address.
#define ADDRESS_FLAGS_EXCLUDE           0x0004 //... Perform logical XOR vs logical OR.
#define ADDRESS_FLAGS_DST_GROUP_ADDR    0x0008 //... Match group bit only.

#define ADDRESS_FLAGS_MATCH_BOTH        0x0003 //... 0x0001 | 0x0002.

// -----------
// Structure
// -----------
typedef struct _ADDRESSPAIR
{
    WORD        AddressFlags;
    WORD        NalReserved;
    ADDRESS     DstAddress;
    ADDRESS     SrcAddress;
} ADDRESSPAIR;
typedef ADDRESSPAIR *LPADDRESSPAIR;
#define ADDRESSPAIR_SIZE  sizeof(ADDRESSPAIR)


//=============================================================================
//  Address table.
//=============================================================================

// -----------
// Constants
// -----------
#define MAX_ADDRESS_PAIRS   8

// -----------
// Structure
// -----------
typedef struct _ADDRESSTABLE
{
    DWORD           nAddressPairs;
    DWORD           nNonMacAddressPairs;
    ADDRESSPAIR     AddressPair[MAX_ADDRESS_PAIRS];
} ADDRESSTABLE;

typedef ADDRESSTABLE *LPADDRESSTABLE;

#define ADDRESSTABLE_SIZE sizeof(ADDRESSTABLE)

//=============================================================================
//  Network information.
//=============================================================================

// -----------
// Constants
// -----------
#define NETWORKINFO_FLAGS_PMODE_NOT_SUPPORTED           0x00000001 

#define NETWORKINFO_FLAGS_REMOTE_NAL                    0x00000004 
#define NETWORKINFO_FLAGS_REMOTE_NAL_CONNECTED          0x00000008 
#define NETWORKINFO_FLAGS_REMOTE_CARD                   0x00000010 
#define NETWORKINFO_FLAGS_RAS                           0x00000020 

// -----------
// Structure
// -----------
typedef struct _NETWORKINFO
{
    BYTE            PermanentAddr[6];       //... Permanent MAC address
    BYTE            CurrentAddr[6];         //... Current  MAC address
    ADDRESS         OtherAddress;           //... Other address supported (IP, IPX, etc...)
    DWORD           LinkSpeed;              //... Link speed in Mbits.
    DWORD           MacType;                //... Media type.
    DWORD           MaxFrameSize;           //... Max frame size allowed.
    DWORD           Flags;                  //... Informational flags.
    DWORD           TimestampScaleFactor;   //... 1 = 1/1 ms, 10 = 1/10 ms, 100 = 1/100 ms, etc.
    BYTE            NodeName[32];           //... Name of remote workstation.
    BOOL            PModeSupported;         //... Card claims to support P-Mode
    BYTE            Comment[ADAPTER_COMMENT_LENGTH]; // Adapter comment field.
} NETWORKINFO;
typedef NETWORKINFO *LPNETWORKINFO;
#define NETWORKINFO_SIZE    sizeof(NETWORKINFO)
#define MINIMUM_FRAME_SIZE   32


//=============================================================================
//  Pattern structure.
//=============================================================================

// -----------
// Constants
// -----------
#define MAX_PATTERN_LENGTH  16 

// When set this flag will cause those frames which do NOT have the specified pattern
// in the proper stop to be kept.
#define PATTERN_MATCH_FLAGS_NOT                 0x00000001

// This flag was used in previous versions of Network Monitor and its value is therefore 
// reserved for compatability reasons
#define PATTERN_MATCH_FLAGS_RESERVED_1          0x00000002

// When set this flag indicates that the user is not interested in a pattern match within 
// IP or IPX, but in the protocol that follows.  The driver will ensure that the protocol
// given in OffsetBasis is there and then that the port in the fram matches the port given.
// It will then calculate the offset from the beginning of the protocol that follows IP or IPX.
// NOTE: This flag is ignored if it is used with any OffsetBasis other than 
// OFFSET_BASIS_RELATIVE_TO_IPX or OFFSET_BASIS_RELATIVE_TO_IP
#define PATTERN_MATCH_FLAGS_PORT_SPECIFIED      0x00000008


// The offset given is relative to the beginning of the frame. The 
// PATTERN_MATCH_FLAGS_PORT_SPECIFIED flag is ignored.
#define OFFSET_BASIS_RELATIVE_TO_FRAME                0

// The offset given is relative to the beginning of the Effective Protocol.
// The Effective Protocol is defined as the protocol that follows
// the last protocol that determines Etype/SAP. In normal terms this means 
// that the Effective Protocol will be IP, IPX, XNS, or any of their ilk.
// The PATTERN_MATCH_FLAGS_PORT_SPECIFIED flag is ignored.
#define OFFSET_BASIS_RELATIVE_TO_EFFECTIVE_PROTOCOL   1

// The offset given is relative to the beginning of IPX. If IPX is not present
// then the frame does not match. If the PATTERN_MATCH_FLAGS_PORT_SPECIFIED
// flag is set then the offset is relative to the beginning of the protocol
// which follows IPX.
#define OFFSET_BASIS_RELATIVE_TO_IPX                  2

// The offset given is relative to the beginning of IP. If IP is not present
// then the frame does not match. If the PATTERN_MATCH_FLAGS_PORT_SPECIFIED
// flag is set then the offset is relative to the beginning of the protocol
// which follows IP.
#define OFFSET_BASIS_RELATIVE_TO_IP                   3

// -----------
// Structure
// -----------
typedef union
{
    BYTE IPPort;
    WORD ByteSwappedIPXPort;
} GENERIC_PORT;


typedef struct _PATTERNMATCH
{
    DWORD        Flags;
    BYTE         OffsetBasis;
    GENERIC_PORT Port;
    WORD         Offset;
    WORD         Length;
    BYTE         PatternToMatch[MAX_PATTERN_LENGTH];//... Pattern to match.
} PATTERNMATCH;
typedef PATTERNMATCH *LPPATTERNMATCH;
#define PATTERNMATCH_SIZE sizeof(PATTERNMATCH)

//=============================================================================
//  Expression structure.
//=============================================================================

// -----------
// Constants
// -----------
#define MAX_PATTERNS    4

// -----------
// Structures
// -----------
typedef struct _ANDEXP
{
    DWORD            nPatternMatches;
    PATTERNMATCH     PatternMatch[MAX_PATTERNS];
} ANDEXP;
typedef ANDEXP *LPANDEXP;
#define ANDEXP_SIZE     sizeof(ANDEXP)

typedef struct _EXPRESSION
{
    DWORD       nAndExps;
    ANDEXP      AndExp[MAX_PATTERNS];
} EXPRESSION;
typedef EXPRESSION *LPEXPRESSION;
#define EXPRESSION_SIZE    sizeof(EXPRESSION)

//
//  Trigger.
//

// -----------
// Constants
// -----------

#define TRIGGER_TYPE_PATTERN_MATCH                       1  
#define TRIGGER_TYPE_BUFFER_CONTENT                      2  
#define TRIGGER_TYPE_PATTERN_MATCH_THEN_BUFFER_CONTENT   3  
#define TRIGGER_TYPE_BUFFER_CONTENT_THEN_PATTERN_MATCH   4  

#define TRIGGER_FLAGS_FRAME_RELATIVE                   0x00000000  
#define TRIGGER_FLAGS_DATA_RELATIVE                    0x00000001  

#define TRIGGER_ACTION_NOTIFY                          0x00  
#define TRIGGER_ACTION_STOP                            0x02  
#define TRIGGER_ACTION_PAUSE                           0x03  

#define TRIGGER_BUFFER_FULL_25_PERCENT         0  
#define TRIGGER_BUFFER_FULL_50_PERCENT         1  
#define TRIGGER_BUFFER_FULL_75_PERCENT         2  
#define TRIGGER_BUFFER_FULL_100_PERCENT        3  

// -----------
// Structure
// -----------
// this is just needed in the driver
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef struct _TRIGGER
{
    BOOL            TriggerActive;          //... Whether trigger is running
    BYTE            TriggerType;            //... Opcode of trigger
    BYTE            TriggerAction;          //... Action to take when trigger occurs.
    DWORD           TriggerFlags;           //... Trigger flags.
    PATTERNMATCH    TriggerPatternMatch;    //... Trigger pattern match.
    DWORD           TriggerBufferSize;      //... Trigger buffer size.
    DWORD           TriggerReserved;        //... Set to all zeros - do not use
    char            TriggerCommandLine[MAX_PATH];
} TRIGGER;
typedef TRIGGER *LPTRIGGER;
#define TRIGGER_SIZE    sizeof(TRIGGER)

//
//  Capture filter.
//

// -----------
// Constants
// -----------
//  Capture filter flags. By default all frames are rejected and
//  NalSetCaptureFilter enables them based on the CAPTUREFILTER flags
//  defined below.

#define CAPTUREFILTER_FLAGS_INCLUDE_ALL_SAPS    0x0001      //... include saps.
#define CAPTUREFILTER_FLAGS_INCLUDE_ALL_ETYPES  0x0002      //... include etypes.
#define CAPTUREFILTER_FLAGS_TRIGGER             0x0004      //... process trigger.
#define CAPTUREFILTER_FLAGS_LOCAL_ONLY          0x0008      //... non-promiscuous.
#define CAPTUREFILTER_FLAGS_DISCARD_COMMENTS    0x0010      //... throw away our internal comment frames
#define CAPTUREFILTER_FLAGS_KEEP_RAW        0x0020      //... Keep SMT and Token Ring MAC frames

//.. The following causes both saps and etypes to be included.
#define CAPTUREFILTER_FLAGS_INCLUDE_ALL         0x0003      //... 0x0001 | 0x0002

#define BUFFER_FULL_25_PERCENT                  0 
#define BUFFER_FULL_50_PERCENT                  1 
#define BUFFER_FULL_75_PERCENT                  2 
#define BUFFER_FULL_100_PERCENT                 3 

// -----------
// Structure
// -----------
typedef struct _CAPTUREFILTER
{
    DWORD           FilterFlags;            //... filter flags,
    LPBYTE          lpSapTable;             //... sap table.
    LPWORD          lpEtypeTable;           //... etype table.
    WORD            nSaps;                  //... sap table length.
    WORD            nEtypes;                //... Etype table length.
    LPADDRESSTABLE  AddressTable;           //... Address table.
    EXPRESSION      FilterExpression;       //... Filter expression.
    TRIGGER         Trigger;                //... Trigger structure.
    DWORD           nFrameBytesToCopy;      //... Number of byte of each frame to copy.
    DWORD           Reserved;
} CAPTUREFILTER;
typedef CAPTUREFILTER *LPCAPTUREFILTER;
#define CAPTUREFILTER_SIZE    sizeof(CAPTUREFILTER)

//=============================================================================
//  Frame type.
//=============================================================================

// -----------
// Structure
// -----------
#pragma warning(disable:4200)
typedef struct _FRAME
{
    __int64  TimeStamp;      //... Relative time in 1/1,000,000th seconds.
    DWORD     FrameLength;  //... MAC frame length.
    DWORD     nBytesAvail;  //... Actual frame length copied.
    BYTE      MacFrame[0];  //... Frame data.
} FRAME;
#pragma warning(default:4200)
typedef FRAME *LPFRAME;
typedef FRAME UNALIGNED *ULPFRAME;
#define FRAME_SIZE sizeof(FRAME)

//=============================================================================
//  Frame descriptor type.
//=============================================================================

// -----------
// Constants
// -----------
#define LOW_PROTOCOL_IPX     OFFSET_BASIS_RELATIVE_TO_IPX  
#define LOW_PROTOCOL_IP      OFFSET_BASIS_RELATIVE_TO_IP   
#define LOW_PROTOCOL_UNKNOWN ((BYTE)-1)
                                                            
// -----------
// Structure
// -----------
typedef struct _FRAME_DESCRIPTOR
{
    LPBYTE  FramePointer;       //... Pointer to frame data
    __int64 TimeStamp;      //... Relative time in 1/1,000,000th seconds.
    DWORD   FrameLength;    //... MAC frame length.
    DWORD   nBytesAvail;    //... Actual frame length copied.
    WORD    Etype;
    BYTE    Sap;       
    BYTE    LowProtocol;           //... Index of Protocol found
    WORD    LowProtocolOffset;     //... Offset to protocol data
    GENERIC_PORT HighPort;         //... Port of high protocol in low protocol
    WORD    HighProtocolOffset;    //... Offset to protocol data
} FRAME_DESCRIPTOR;
typedef FRAME_DESCRIPTOR *LPFRAME_DESCRIPTOR;
#define FRAME_DESCRIPTOR_SIZE sizeof(FRAME_DESCRIPTOR)



//=============================================================================
//  Frame descriptor table.
//=============================================================================
// -----------
// Structure
// -----------
typedef struct _FRAMETABLE
{
    DWORD   FrameTableLength;
    DWORD   StartIndex;
    DWORD   EndIndex;
    DWORD   FrameCount;
    FRAME_DESCRIPTOR    Frames[1];
} FRAMETABLE;

typedef FRAMETABLE *LPFRAMETABLE;


//=============================================================================
//  Station statistics.
//=============================================================================

// -----------
// Constants
// -----------
#define STATIONSTATS_FLAGS_INITIALIZED  0x0001      //... Structure has been initialized.
#define STATIONSTATS_FLAGS_EVENTPOSTED  0x0002      //... Nal has posted an event for this entry.


#define STATIONSTATS_POOL_SIZE      100

// -----------
// Structure
// -----------
typedef struct _STATIONSTATS
{
    DWORD           NextStationStats;       //... Index of next Station
    DWORD           SessionPartnerList;     //... Index to the session table
    DWORD           Flags;
    BYTE            StationAddress[6];
    WORD            Pad;                    //... DWORD alignment.
    DWORD           TotalPacketsReceived;
    DWORD           TotalDirectedPacketsSent;
    DWORD           TotalBroadcastPacketsSent;
    DWORD           TotalMulticastPacketsSent;
    DWORD           TotalBytesReceived;
    DWORD           TotalBytesSent;
} STATIONSTATS;
typedef STATIONSTATS * LPSTATIONSTATS;
#define STATIONSTATS_SIZE sizeof(STATIONSTATS)

//=============================================================================
//  Session statistics.
//=============================================================================


// -----------
// Constants
// -----------
#define SESSION_FLAGS_INITIALIZED  0x0001           //... Structure has been initialized.
#define SESSION_FLAGS_EVENTPOSTED  0x0002           //... Nal has posted an event for this entry.


#define SESSION_POOL_SIZE           100

// -----------
// Structure
// -----------
typedef struct _SESSIONSTATS
{
    DWORD           NextSession;            // Index of next session
    DWORD           StationOwner;           // Index of one station
    DWORD           StationPartner;         // Index of the other station
    DWORD           Flags;
    DWORD           TotalPacketsSent;
} SESSIONSTATS;
typedef SESSIONSTATS * LPSESSIONSTATS;
#define SESSIONSTATS_SIZE  sizeof(SESSIONSTATS)


//=============================================================================
//  Station Query
//=============================================================================

// -----------
// Constants
// -----------
#define STATIONQUERY_FLAGS_LOADED       0x0001      //... Driver is loaded but the kernel is not.
#define STATIONQUERY_FLAGS_RUNNING      0x0002      //... Driver is loaded but not capturing.
#define STATIONQUERY_FLAGS_CAPTURING    0x0004      //... Driver is actively capturing.
#define STATIONQUERY_FLAGS_TRANSMITTING 0x0008      //... Driver is actively transmitting.

// -----------
// Structure
// -----------
#define STATIONQUERY_VERSION_MINOR      0x01
#define STATIONQUERY_VERSION_MAJOR      0x02

typedef struct _OLDSTATIONQUERY
{
    DWORD           Flags;                              //... Current state of Network Monitor.
    BYTE            BCDVerMinor;                        //... Binary coded decimal (minor).
    BYTE            BCDVerMajor;                        //... Binary coded decimal (major).
    DWORD           LicenseNumber;                      //... Software license number.
    BYTE            MachineName[MACHINE_NAME_LENGTH];   //... Machine name, if any.
    BYTE            UserName[USER_NAME_LENGTH];         //... Username name, if any.
    BYTE            Reserved[32];                       //... Reserved.
    BYTE            AdapterAddress[6];                  //... NIC address.
} OLDSTATIONQUERY;
typedef OLDSTATIONQUERY *LPOLDSTATIONQUERY;
#define OLDSTATIONQUERY_SIZE sizeof(OLDSTATIONQUERY)

typedef struct _STATIONQUERY
{
    DWORD           Flags;                              //... Current state of Network Monitor.
    BYTE            BCDVerMinor;                        //... Binary coded decimal (minor).
    BYTE            BCDVerMajor;                        //... Binary coded decimal (major).
    DWORD           LicenseNumber;                      //... Software license number.
    BYTE            MachineName[MACHINE_NAME_LENGTH];   //... Machine name, if any.
    BYTE            UserName[USER_NAME_LENGTH];         //... Username name, if any.
    BYTE            Reserved[32];                       //... Reserved.
    BYTE            AdapterAddress[6];                  //... NIC address.
    WCHAR           WMachineName[MACHINE_NAME_LENGTH];  //... Wide Machine name, if any.
    WCHAR           WUserName[USER_NAME_LENGTH];        //... Wide Username name, if any.
} STATIONQUERY;
typedef STATIONQUERY *LPSTATIONQUERY;
#define STATIONQUERY_SIZE sizeof(STATIONQUERY)

//=============================================================================
//  QUERYTABLE structure.
//=============================================================================

// -----------
// Structure
// -----------
typedef struct _QUERYTABLE
{
                                DWORD           nStationQueries;
                                STATIONQUERY    StationQuery[1];
} QUERYTABLE;
typedef QUERYTABLE *LPQUERYTABLE;
#define QUERYTABLE_SIZE sizeof(QUERYTABLE)


//=============================================================================
//  The LINK structure is used to chain structures together into a list.
//=============================================================================

#ifndef _LINK_
#define _LINK_

typedef struct _LINK *LPLINK;

typedef struct _LINK
{
    LPLINK     PrevLink;                    //... Previous or back pointer.
    LPLINK     NextLink;                    //... Next or forward pointer.
} LINK;

#endif //_LINK_

//=============================================================================
//  Security Request packet
//=============================================================================

typedef struct _SECURITY_PERMISSION_CHECK
{
    UINT        Version;
    DWORD       RandomNumber;
    BYTE        MachineName[MACHINE_NAME_LENGTH];
    BYTE        UserName[USER_NAME_LENGTH];
    UINT        MacType;                            //... Mac type.
    BYTE        PermanentAdapterAddress[MAC_ADDRESS_SIZE];
    BYTE        CurrentAdapterAddress[MAC_ADDRESS_SIZE];
    WCHAR       WMachineName[MACHINE_NAME_LENGTH];
    WCHAR       WUserName[USER_NAME_LENGTH];
} SECURITY_PERMISSION_CHECK;

typedef SECURITY_PERMISSION_CHECK * LPSECURITY_PERMISSION_CHECK;

typedef SECURITY_PERMISSION_CHECK UNALIGNED * ULPSECURITY_PERMISSION_CHECK;

#define SECURITY_PERMISSION_CHECK_SIZE     sizeof(SECURITY_PERMISSION_CHECK)


//=============================================================================
//  Security Response packet
//=============================================================================

#define MAX_SECURITY_BREACH_REASON_SIZE             100
#define MAX_SIGNATURE_LENGTH                        128
#define MAX_USER_NAME_LENGTH                        256

typedef struct _SECURITY_PERMISSION_RESPONSE
{
    UINT        Version;
    DWORD       RandomNumber;
    BYTE        MachineName[MACHINE_NAME_LENGTH];
    BYTE        Address[MAC_ADDRESS_SIZE];
    BYTE        UserName[MAX_USER_NAME_LENGTH];
    BYTE        Reason[MAX_SECURITY_BREACH_REASON_SIZE];
    DWORD       SignatureLength;
    BYTE        Signature[MAX_SIGNATURE_LENGTH];
} SECURITY_PERMISSION_RESPONSE;

typedef SECURITY_PERMISSION_RESPONSE * LPSECURITY_PERMISSION_RESPONSE;

typedef SECURITY_PERMISSION_RESPONSE UNALIGNED * ULPSECURITY_PERMISSION_RESPONSE;

#define SECURITY_PERMISSION_RESPONSE_SIZE     sizeof(SECURITY_PERMISSION_RESPONSE)


//
//  Callback type
//
//
// An UpdateEvent structure is part of an open context (defined below).
// As events occur, the update thread is released. While the thread
// is processing events, we want to be sure we don't lose any events.
// A queue of UpdateEvent structures is used for that purpose.
//

// -----------
// Constants
// -----------
// generic events
#define UPDATE_EVENT_TERMINATE_THREAD               0x00000000
#define UPDATE_EVENT_NETWORK_STATUS                 0x00000001

// rtc events
#define UPDATE_EVENT_RTC_INTERVAL_ELAPSED           0x00000002
#define UPDATE_EVENT_RTC_FRAME_TABLE_FULL           0x00000003
#define UPDATE_EVENT_RTC_BUFFER_FULL                0x00000004

// delayed events
#define UPDATE_EVENT_TRIGGER_BUFFER_CONTENT         0x00000005
#define UPDATE_EVENT_TRIGGER_PATTERN_MATCH          0x00000006
#define UPDATE_EVENT_TRIGGER_BUFFER_PATTERN         0x00000007
#define UPDATE_EVENT_TRIGGER_PATTERN_BUFFER         0x00000008

// transmit events
#define UPDATE_EVENT_TRANSMIT_STATUS                0x00000009

// Security events
#define UPDATE_EVENT_SECURITY_BREACH                0x0000000A

// Remote failure event
#define UPDATE_EVENT_REMOTE_FAILURE                 0x0000000B

// actions
#define UPDATE_ACTION_TERMINATE_THREAD              0x00000000
#define UPDATE_ACTION_NOTIFY                        0x00000001
#define UPDATE_ACTION_STOP_CAPTURE                  0x00000002
#define UPDATE_ACTION_PAUSE_CAPTURE                 0x00000003
#define UPDATE_ACTION_RTC_BUFFER_SWITCH             0x00000004
                                                        
// -----------
// Structures
// -----------
typedef struct _UPDATE_EVENT 
{
    USHORT          Event;          //... Actual event being recorded
    DWORD           Action;         //... What action was taken
    DWORD           Status;         //... Usually network status
    DWORD           Value;          //... Auxilary counter variable
    __int64         TimeStamp;      //... Mark the events( in millionths of a second)
    LPVOID          lpUserContext;  //... User context given by App
    LPVOID          lpReserved;     //... Driver/NAL use
    UINT            FramesDropped;  //... RTF frames dropped in this buffer
    union
    {
        DWORD           Reserved;       //... No data comes back with this switch option.
        LPFRAMETABLE    lpFrameTable;   //... RTF only
        SECURITY_PERMISSION_RESPONSE SecurityResponse;// remote security monitor response
    };

    // this is only filled in on non-security related stops (ie. triggers)
    LPSTATISTICS lpFinalStats;

} UPDATE_EVENT;
typedef UPDATE_EVENT *PUPDATE_EVENT;

// note for c++ users:
// the declaration for this callback should be in the public part of the header file:
// static WINAPI DWORD NetworkCallback( UPDATE_EVENT events);
// and the implementation should be, in the protected section of the cpp file:
// DWORD WINAPI ClassName::NetworkCallback( UPDATE_EVENT events) {};
//typedef DWORD (WINAPI *LPNETWORKCALLBACKPROC)( UPDATE_EVENT);
typedef DWORD (WINAPI *LPNETWORKCALLBACKPROC)( UPDATE_EVENT);
 

//=============================================================================
//  NETWORKSTATUS data structure.
//=============================================================================

typedef struct _NETWORKSTATUS
{
    DWORD       State;                                      //... Current state.
    DWORD       Flags;                                      //... Current flags.
//    DWORD       BufferSize;                                 //... Capture buffer size.
} NETWORKSTATUS;

typedef NETWORKSTATUS *LPNETWORKSTATUS;

#define NETWORKSTATUS_SIZE   sizeof(NETWORKSTATUS)

#define NETWORKSTATUS_STATE_VOID                0           //... Nothing state.
#define NETWORKSTATUS_STATE_INIT                1           //... Initial state.
#define NETWORKSTATUS_STATE_CAPTURING           2           //... Capturing state.
#define NETWORKSTATUS_STATE_PAUSED              3           //... Paused state.

#define NETWORKSTATUS_FLAGS_TRIGGER_PENDING     0x0001      //... Trigger pending.

//
//  BONEPACKET structure.
//=============================================================================

typedef struct _BONEPACKET
{
    DWORD       Signature;                  //... 'RTSS'
    BYTE        Command;                    //... packet command field.
    BYTE        Flags;                      //... flags.
    DWORD       Reserved;                   //... reserved.
    WORD        Length;                     //... length of data, if any.
} BONEPACKET;
typedef BONEPACKET *LPBONEPACKET;
#define BONEPACKET_SIZE     sizeof(BONEPACKET)

//=============================================================================
//  BONE alert packet.
//=============================================================================

typedef struct _ALERT
{
    DWORD           AlertCode;                          //... Alert code.
    WCHAR           WMachineName[MACHINE_NAME_LENGTH];   //... Machine name, if any.
    WCHAR           WUserName[USER_NAME_LENGTH];         //... Username name, if any.

    union
    {
        BYTE            Pad[32];                        //... Alert code-specific data.
        DWORD           nFramesToSend;                  //... Transmit alert data.
    };
} ALERT;

typedef ALERT *LPALERT;

#define ALERT_SIZE sizeof(ALERT)

//... Alert codes.

#define ALERT_CODE_BEGIN_TRANSMIT           0

//
//  BONEPACKET signature.
//

#define MAKE_WORD(l, h)         (((WORD) (l)) | (((WORD) (h)) << 8))
#define MAKE_LONG(l, h)         (((DWORD) (l)) | (((DWORD) (h)) << 16L))
#define MAKE_SIG(a, b, c, d)    MAKE_LONG(MAKE_WORD(a, b), MAKE_WORD(c, d))
#define BONE_PACKET_SIGNATURE   MAKE_SIG('R', 'T', 'S', 'S')

//
//  BONEPACKET commands.
//

#define BONE_COMMAND_STATION_QUERY_REQUEST      0
#define BONE_COMMAND_STATION_QUERY_RESPONSE     1
#define BONE_COMMAND_ALERT                      2
#define BONE_COMMAND_PERMISSION_CHECK           3
#define BONE_COMMAND_PERMISSION_RESPONSE        4
#define BONE_COMMAND_SECURITY_MONITOR_EVENT     5


//
//  STATISTICS parameter structure return from OpenNetwork().
//
#define MAX_SESSIONS    100
#define MAX_STATIONS    100

typedef struct _STATISTICSPARAM
{
    DWORD           StatisticsSize;             //... Number of bytes.
    STATISTICS      Statistics;
    DWORD           StatisticsTableEntries;     //... Number of entries.
    STATIONSTATS    StatisticsTable[MAX_STATIONS];
    DWORD           SessionTableEntries;        //... Number of entries.
    SESSIONSTATS    SessionTable[MAX_SESSIONS];
} STATISTICSPARAM;
typedef STATISTICSPARAM *LPSTATISTICSPARAM;
#define STATISTICSPARAM_SIZE  sizeof(STATISTICSPARAM)

//=============================================================================
//  Capture file header.
//=============================================================================

#define CAPTUREFILE_VERSION_MAJOR   2
#define CAPTUREFILE_VERSION_MINOR   0

#define MakeVersion(Major, Minor)   ((DWORD) MAKEWORD(Minor, Major))

#define GetCurrentVersion()         MakeVersion(CAPTUREFILE_VERSION_MAJOR, CAPTUREFILE_VERSION_MINOR)

#define NETMON_1_0_CAPTUREFILE_SIGNATURE     MAKE_IDENTIFIER('R', 'T', 'S', 'S')
#define NETMON_2_0_CAPTUREFILE_SIGNATURE     MAKE_IDENTIFIER('G', 'M', 'B', 'U')

typedef struct _CAPTUREFILE_HEADER_VALUES
{
    DWORD      Signature;                //... Unique identifier: 'RTSS'.
    BYTE       BCDVerMinor;              //... Binary coded decimal (minor).
    BYTE       BCDVerMajor;              //... Binary coded decimal (major).
    WORD       MacType;                  //... Topology type.
    SYSTEMTIME TimeStamp;                //... time of capture.
    DWORD      FrameTableOffset;         //... Frame index table.
    DWORD      FrameTableLength;         //... Frame index table size.
    DWORD      UserDataOffset;           //... User data offset.
    DWORD      UserDataLength;           //... User data length.
    DWORD      CommentDataOffset;        //... Comment Data offset
    DWORD      CommentDataLength;        //... Length of comment data.
    DWORD      StatisticsOffset;         //....offset to STATISTICS STRUCTURE
    DWORD      StatisticsLength;         //....length of stats struct
    DWORD      NetworkInfoOffset;        //....offset to network info structure
    DWORD      NetworkInfoLength;        //....length of network info structure
    DWORD      ConversationStatsOffset;  //....offset of conv stats structure
    DWORD      ConversationStatsLength;  //....length of conv stats structure
} CAPTUREFILE_HEADER_VALUES;

typedef CAPTUREFILE_HEADER_VALUES  *LPCAPTUREFILE_HEADER_VALUES;

#define CAPTUREFILE_HEADER_VALUES_SIZE sizeof(CAPTUREFILE_HEADER_VALUES)


//=============================================================================
//  Capture file.
//=============================================================================


typedef struct _CAPTUREFILE_HEADER
{
    union
    {
        CAPTUREFILE_HEADER_VALUES  ActualHeader;
        BYTE  Buffer[CAPTUREFILE_HEADER_VALUES_SIZE];
    };

    BYTE                    Reserved[128 - CAPTUREFILE_HEADER_VALUES_SIZE];
} CAPTUREFILE_HEADER;

typedef CAPTUREFILE_HEADER         *LPCAPTUREFILE_HEADER;

#define CAPTUREFILE_HEADER_SIZE     sizeof(CAPTUREFILE_HEADER)


//=============================================================================
//  Stats Frame definitions.
//=============================================================================


//=============================================================================
//  Stats Frame definitions.
//=============================================================================
typedef struct _EFRAMEHDR
{
    BYTE    SrcAddress[6];
    BYTE    DstAddress[6];
    WORD    Length;
    BYTE    DSAP;
    BYTE    SSAP;
    BYTE    Control;
    BYTE    ProtocolID[3];
    WORD    EtherType;
} EFRAMEHDR;
 
typedef struct _TRFRAMEHDR
{
    BYTE    AC;
    BYTE    FC;
    BYTE    SrcAddress[6];
    BYTE    DstAddress[6];
    BYTE    DSAP;
    BYTE    SSAP;
    BYTE    Control;
    BYTE    ProtocolID[3];
    WORD    EtherType;
} TRFRAMEHDR;
 
#define DEFAULT_TR_AC       0x00
#define DEFAULT_TR_FC       0x40
#define DEFAULT_SAP         0xAA
#define DEFAULT_CONTROL     0x03
#define DEFAULT_ETHERTYPE   0x8419
 
typedef struct _FDDIFRAMEHDR
{
    BYTE    FC;
    BYTE    SrcAddress[6];
    BYTE    DstAddress[6];
    BYTE    DSAP;
    BYTE    SSAP;
    BYTE    Control;
    BYTE    ProtocolID[3];
    WORD    EtherType;
} FDDIFRAMEHDR;
#define DEFAULT_FDDI_FC      0x10
 
typedef struct _FDDISTATFRAME
{
    __int64     TimeStamp;          //... Relative time in milliseconds.
    DWORD       FrameLength;        //... MAC frame length.
    DWORD       nBytesAvail;        //... Actual frame length copied.
    FDDIFRAMEHDR  FrameHeader;
    BYTE        FrameID[4];         //... $MST
    DWORD       Flags;              //... Flags
    DWORD       FrameType;          //... Statistics is always 103 (0x67)
    WORD        StatsDataLen;       //... Length of Version DWORD + STATISTICS
    DWORD       StatsVersion;
    STATISTICS  Statistics;
} FDDISTATFRAME;
 
typedef FDDISTATFRAME *LPFDDISTATFRAME;
typedef FDDISTATFRAME UNALIGNED *ULPFDDISTATFRAME;
 
#define FDDISTATFRAME_SIZE  sizeof(FDDISTATFRAME)
 
typedef struct _TRSTATFRAME
{
    __int64     TimeStamp;          //... Relative time in milliseconds.
    DWORD       FrameLength;        //... MAC frame length.
    DWORD       nBytesAvail;        //... Actual frame length copied.
    TRFRAMEHDR  FrameHeader;
    BYTE        FrameID[4];         //... $MST
    DWORD       Flags;              //... Flags
    DWORD       FrameType;          //... Statistics is always 103 (0x67)
    WORD        StatsDataLen;       //... Length of Version DWORD + STATISTICS
    DWORD       StatsVersion;
    STATISTICS  Statistics;
} TRSTATFRAME;
 
typedef TRSTATFRAME *LPTRSTATFRAME;
typedef TRSTATFRAME UNALIGNED *ULPTRSTATFRAME;
 
#define TRSTATFRAME_SIZE  sizeof(TRSTATFRAME)
 
typedef struct _ESTATFRAME
{
    __int64     TimeStamp;          //... Relative time in milliseconds.
    DWORD       FrameLength;        //... MAC frame length.
    DWORD       nBytesAvail;        //... Actual frame length copied.
    EFRAMEHDR   FrameHeader;
    BYTE        FrameID[4];         //... $MST
    DWORD       Flags;              //... Flags
    DWORD       FrameType;          //... Statistics is always 103 (0x67)
    WORD        StatsDataLen;       //... Length of Version DWORD + STATISTICS
    DWORD       StatsVersion;
    STATISTICS  Statistics;
} ESTATFRAME;
 
 
#define STATISTICS_VERSION_1_0  0x00000000
#define STATISTICS_VERSION_2_0  0x00000020
 
typedef ESTATFRAME *LPESTATFRAME;
typedef ESTATFRAME UNALIGNED *ULPESTATFRAME;

#define ESTATFRAME_SIZE  sizeof(ESTATFRAME)
 
// this variable could change if any of the above sizes changed
#define MAX_STATSFRAME_SIZE sizeof(TRSTATFRAME)
 
#define STATS_FRAME_TYPE   103

#pragma pack()
#endif //__npptypes_h__
