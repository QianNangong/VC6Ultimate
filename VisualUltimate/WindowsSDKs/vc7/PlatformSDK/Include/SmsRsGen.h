//////////////////////////////////////////////////////////////
// Copyright (c)1999 Microsoft Corporation, All Rights Reserved
//
// Version 1.0
//
// SMSRsGen.h - Header file for the SMS Resource Generator
//				DLL functions
//
//
//////////////////////////////////////////////////////////////

#ifndef __SMSRSGEN_H
#define __SMSRSGEN_H

#include <windows.h>

// *** 'Property Flags' ***
// Used to specify any special purposes for a property
// Note: All architectures must have at least one Property with the ADDPROP_NAME flag
//		 This property will be copied into the Name property of the SMS_R_<Architecture> instances
#define ADDPROP_GUID		0x00000002 // Defines this property as being a GUID
#define ADDPROP_GROUPING	0x00000004 // *Reserved*
#define ADDPROP_KEY			0x00000008 // Defines this property as being a Key value that must be unique
#define ADDPROP_ARRAY		0x00000010 // *Reserved*
#define ADDPROP_AGENT		0x00000020 // *Reserved*
#define ADDPROP_NAME		0x00000044 // Specifies this property as the actual 'Name' property in the resource
#define ADDPROP_NAME2		0x00000084 // Specifies this property as the actual 'Comment' property in the resource


// **********************************
// NewDDR()
//
// Clears the DDR Data and begins creating a new DDR File
//
// Parameters 
//		sArchitecture	- The Name of the Architecture to Use\Create
//		sAgentName		- The Name of the Agent that is creating the DDR (USER_DISCOVERY, etc)
//		sSiteCode		- The SiteCode for the Site that the DDR is to be delivered to
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRNew(char *sArchitecture, char *sAgentName, char *sSiteCode);

// **********************************
// DDRAddInteger()
//
// Adds an Integer Property to the DDR
//
// Parameters
//		sName	- The Name\Title of the Property
//		lValue	- The actual numeric data assigned to the Property
//		dwFlags	- The flags for this property, (see 'Property Flags' in the header file)
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRAddInteger(char *sName, long lValue, DWORD dwFlags);

// **********************************
// DDRAddString()
//
// Adds a String Property to the DDR
//
// Parameters
//		sName		- The Name\Title of the Property
//		sValue		- The actual string data assigned to the Property
//		nSQLWidth	- The width of the SQL Table generated (usualy 32, 64, or 256)
//		dwFlags		- The flags for this property, (see 'Property Flags' in the header file)
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRAddString(char *sName, char *sValue, int nSQLWidth, DWORD dwFlags);

// **********************************
// DDRAddIntegerArray()
//
// Adds an Array of Integers Property to the DDR
//
// Parameters
//		sName	- The Name\Title of the Property
//		lArray	- An array numeric data to be assigned to the Property
//		lSize	- The size of the lValue array
//		dwFlags	- The flags for this property, (see 'Property Flags' in the header file)
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRAddIntegerArray(char *sName, long lArray[], long lSize, DWORD dwFlags);

// **********************************
// AddStringArray()
//
// Adds an Array of Strings Property to the DDR
//
// Parameters
//		sName		- The Name\Title of the Property
//		sArray		- An Array of Null Terminated Strings to be assigned to the Property
//		nArraySize	- The number of strings in the string array
//		nSQLWidth	- The width of the SQL Table SMS Generates (usually 32, 64, or 256)
//		dwFlags		- The flags for this property, (see 'Property Flags' in the header file)
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRAddStringArray(char *sName, char *sArray[], int nArraySize, int nSQLWidth, DWORD dwFlags);

// **********************************
// WriteDDR()
//
// Writes the DDR to the specified file
//
// Parameters
//		sFileName	- The Name of the DDR file to be written (ex\ 'MyArch.DDR')
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRWrite(char *sFileName);

// **********************************
// SendDDRToSMS()
//
// Writes the DDR File to either the CAP inbox (if the machine is an SMS client),
//  or the Server's DDM Inbox (if the machine is an SMS site server).
//
// **********************************
extern "C" __declspec(dllimport) HRESULT DDRSendToSMS(void);

#endif