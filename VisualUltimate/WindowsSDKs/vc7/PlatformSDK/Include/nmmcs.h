//=============================================================================
//  Microsoft (R) Network Monitor (tm). 
//  Copyright (C) 1997-1999. All rights reserved.
//
//  MODULE: NMMCS.h
//
//  Header file to the public MCS structures
//=============================================================================

#ifndef NMMCS_H
#define NMMCS_H

#define NMERR_INVALID_LIST_INDEX        2000
#define NMERR_INVALID_MONITOR           2001
#define NMERR_INVALID_MONITOR_DLL       2002
#define NMERR_UNABLE_TO_CREATE_MONITOR  2003
#define NMERR_INVALID_MONITOR_CONFIG    2005
#define NMERR_INVALID_INDEX             2006
#define NMERR_MONITOR_ENABLED           2007
#define NMERR_MONITOR_NOT_RUNNING       2008
#define NMERR_MONITOR_IS_BUSY           2009
#define NMERR_MCS_IS_BUSY               2010
#define NMERR_NO_MONITORS               2011
#define NMERR_ONE_MONITOR_PER_NETCARD   2012
#define NMERR_CONFIGURATION_REQUIRED    2013
#define NMERR_MONITOR_NOT_CONNECTED     2014
#define NMERR_MONITOR_NOT_CONFIGURED    2015
#define NMERR_MONITOR_CONFIG_FAILED     2016
#define NMERR_MONITOR_INIT_FAILED       2017
#define NMERR_MONITOR_FAULTED           2018
#define NMERR_SAVE_ALL_FAILED           2019
#define NMERR_SAVE_MONITOR_FAILED       2029
#define NMERR_MONITOR_CONNECT_FAILED    2021
#define NMERR_MONITOR_START_FAILED      2022
#define NMERR_MONITOR_STOP_FAILED       2023

// Monitor status values returned from call to GetMonitorStatus
#define MONITOR_STATUS_ERROR            -1
#define MONITOR_STATUS_ENABLED          4
#define MONITOR_STATUS_CONFIGURED       5
#define MONITOR_STATUS_RUNNING          6
#define MONITOR_STATUS_RUNNING_FAULTED  9
#define MONITOR_STATUS_DELETED          10

#define MCS_COMMAND_ENABLE              13
#define MCS_COMMAND_DISABLE             14
#define MCS_COMMAND_SET_CONFIG          15
#define MCS_COMMAND_GET_CONFIG          16
#define MCS_COMMAND_START               17
#define MCS_COMMAND_STOP                18
#define MCS_COMMAND_CONNECT             19
#define MCS_COMMAND_RENAME              20
#define MCS_COMMAND_REFRESH_STATUS      21

// Monitor Creation Flags
#define MCS_CREATE_ONE_PER_NETCARD      0x00000001
#define MCS_CREATE_CONFIGS_BY_DEFAULT   0x00000010
#define MCS_CREATE_PMODE_NOT_REQUIRED   0x00000100

#ifdef __cplusplus
extern "C"
{
#endif


typedef __int64         HMONITOR;

// ****************************************************************************
// NPP_INFO
typedef struct 
{
    DWORD               ListIndex;
    char*               ShortName;
    char*               LongName;    
} NPP_INFO;
typedef NPP_INFO* PNPP_INFO;

// ****************************************************************************
// MONITOR_INFO
typedef struct _MONITOR_INFO
{
    // Our opaque pointer
    HMONITOR            MonitorInstance;

    HMONITOR            MonitorClass;
    DWORD               CreateFlags;
    DWORD               Status;
    DWORD               ListIndex;

    // The description of the monitor DLL
    char*               pDescription;
    char*               pScript;
    char*               pConfiguration;

    // The name of the monitor DLL
    char*               pName;

} MONITOR_INFO;
typedef MONITOR_INFO* PMONITOR_INFO;
// ****************************************************************************
typedef struct
{   
    HMONITOR            Monitor;
    DWORD               ListIndex;
    char*               pszMessage;
} MONITOR_MESSAGE;
typedef MONITOR_MESSAGE* PMONITOR_MESSAGE;
// ****************************************************************************
typedef struct
{
    HMONITOR            Monitor;
    DWORD               Command;
    DWORD               FailureCode;
    DWORD               ListIndex;
    DWORD               Status;
} COMMAND_FAILED_EVENT;
typedef COMMAND_FAILED_EVENT* PCOMMAND_FAILED_EVENT;
// ****************************************************************************

typedef struct
{
    HMONITOR            Monitor;
    DWORD               LastCommand;
    DWORD               ListIndex;
    DWORD               Status;
    DWORD               FramesProcessed;
} MONITOR_STATUS_EVENT;
typedef MONITOR_STATUS_EVENT* PMONITOR_STATUS_EVENT;
// ****************************************************************************

typedef struct
{
    OLECHAR*    pwszName;
    FILETIME    FileTime;
    DWORD       pXMCS;
    BOOL        bCurrent;
} MCS_CLIENT;
typedef MCS_CLIENT* PMCS_CLIENT;
// ****************************************************************************

#ifdef __cplusplus
}
#endif

#endif