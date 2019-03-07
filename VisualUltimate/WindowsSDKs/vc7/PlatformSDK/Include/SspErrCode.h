//-----------------------------------------------------------------------------
// Ssperrcode.h
//                                                  
//
//
//  Copyright (c)1998 - 1999 Microsoft Corporation, All Rights Reserved
//-----------------------------------------------------------------------------

/************************************************************************************************\
|*                                                                                              *|
|* Each error has been classified by several features which are severity, facility, action      *|
|* object, and reason. Severity is the recoverability of the error, whether it was only the     *|
|* call to the provider that failed or whether it's a symptom of a larger failure. Facility     *|
|* is the facility that was being accessed when the failure occurred. Action is the action that *|
|* failed. Object is the object that the action was being preformed on. And reason is the       *|
|* reason for failure (which may or may not be known). In addition to the error code, the       *|
|* error object (see SspError.h) also allows two other pieces to information to be returned,    *|
|* a variant that describes the object (in some cases it may be the object) and a variant that  *|
|* describes the reason. In many cases this will be an additional error code reported by a      *|
|* a particular module.                                                                         *|
|*                                                                                              *|
\************************************************************************************************/


// Error code bitfields:
//
//  31-30: Severity (2 bits)
//  29-28: Reserved (2 bits)
//  27-22: Facility (6 bits)
//  21-16: Action   (6 bits)
//  15- 8: Object   (8 bits)
//   7- 0: Reason   (8 bits)

#if !defined(__ssperrcode_H)
#define      __ssperrcode_H
#pragma once


#define MKERR(S,F,A,O,R)  ((0x03UL & (S))<<30 | (0x3FUL & (F))<<22 | (0x3FUL & (A))<<16 |\
(0xFFUL & (O))<<8  | (0xFF & (R)))

namespace SspEC
{
    const DWORD dwSMASK = MKERR(0xFF,0,0,0,0);
    const DWORD dwFMASK = MKERR(0,0xFF,0,0,0);
    const DWORD dwAMASK = MKERR(0,0,0xFF,0,0);
    const DWORD dwOMASK = MKERR(0,0,0,0xFF,0);
    const DWORD dwRMASK = MKERR(0,0,0,0,0xFF);
	
	
    enum e_severity
    {
            S_NONE,
			S_FUNC,     // functional, can continue
			S_MINOR,    // minor system error, can use other functions of SMS
			S_MAJOR     // totally hosed, shut down
    };
	
    enum e_facility 
    {
            F_NONE, 
			F_INT,      // internal
			F_NET,      // network
			F_FILE,     // file
			F_COM,      // COM
			F_SQL,      // SQL
			F_CIMOM,    // CIMOM
			F_BC,       // base classes
			F_MEM,      // memory
			F_SEC,      // security
			F_REG       // registry
    };
	
    enum e_action
    {
            A_NONE,
			A_CREAT,    // create
			A_OPEN,     // open
			A_DEL,      // delete
			A_READ,     // read
			A_WRITE,    // write
			A_PERST,    //  persist     (create and write)
			A_INST,     // instantiate (open and read)
			A_VAL,      // validate
			A_EXEC,     // execute
			A_LOCK      // lock/unlock
    };
	
    enum e_object
    {
        O_NONE,
			O_FACILITY,
			O_DESCRIPTOR,
			O_DATAFILE,
			O_DIRECTORY,
			O_CONNECTION,
			O_TRANSACTION,
			O_PARAMETER,
			O_PROPERTY,
			O_METHOD,
			O_INSTANCE,
			O_DATA,
			O_RIGHTS,
			O_ROW,
			O_MEMORY,
			O_QUERY,
			O_COLLECTION,
			O_USER,
			O_FILELINE,
			O_RESOURCE
    };
	
    enum e_reason
    {
            R_NONE,          // no further information
			R_WINAPI,        // Win API failure, additional info is GetLastError()
			R_SQL,           // failure in call to SQL server, additional info is failure code
			R_COM,           // failure of call to COM method, additional info is HRESULT
			R_PDFERROR,      // PDF problem
			R_DISTSRC,       // problem with software distribution
			R_COLLERR,       // collection error
			R_TOKITSQL,      // Site control file SQL error
			R_TOKENITEM,     // Site control file error
			R_OFFERSRC,      // Advertisement error
			R_MFCFE,         // MFC CFileException, additional info is in cause code
			R_SITE,          // Site file error
			R_READONLY,      // attempt to write to read only object
			R_TIMEOUT,       // action timed out
			R_NOTFOUND,      // object not found - error object returns not found status when set
			R_ALREADYEXISTS, // object already exists
			R_BADHANDLE,     // invalid pointer or handle
			R_BADDATA,       // data is corrupt
			R_BADPARAM,      // bad parameter passed
			R_NETERR,        // network error
			R_NOTSUPPORTED,  // not supported - error object returns not supported status when set
			R_BADPROP,       // bad property
			R_INVALIDMTD,    // invalid method - error object returns invalid method status when set
			R_STATMSG,       // status message error
			R_FIELDNO,       // reason is field number starting at 0
			R_QUERYSRC       // problem with query object
    };
	
    inline DWORD MkErr(e_facility F, e_action A, e_object O, e_severity S=S_NONE, e_reason R=R_NONE)
    {
        return MKERR(S,F,A,O,R);
    }
	
    //**** Predefined error codes, these are the values we're actually using ****/
    //                     some are used more than others
    //
    //**** NOTE: Reason bits may be set in the returned error code even if they are not
    //           set below. Unless the reason bits are set in the code below, 
    //           mask off the reason bits before comparing error codes.
	
    const DWORD E_NONE(            MkErr(F_NONE, A_NONE,  O_NONE, S_NONE, R_NONE) );
	
    // can't connect to the registry
    const DWORD E_REG_CREAT_CONN( MkErr(F_REG,  A_CREAT, O_CONNECTION, S_MAJOR)  );
    // can't get data from registry
    const DWORD E_REG_OPEN_DATA(   MkErr(F_REG,  A_OPEN,  O_DATA,       S_MAJOR)  );
	
    // can't connect to CIMOM
    const DWORD E_CIMOM_CREAT_CONN( MkErr(F_REG, A_CREAT, O_CONNECTION, S_MAJOR)  );
	
    // CoCreateInstance failed
    const DWORD E_COM_CREAT_INST( MkErr(F_COM, A_CREAT, O_INSTANCE, S_MAJOR) );
	
    const DWORD E_FILE_CREATE_DIR( MkErr(F_FILE, A_CREAT, O_DIRECTORY,S_MINOR)  );
    const DWORD E_FILE_PERST_FILE( MkErr(F_FILE, A_PERST, O_DATAFILE,S_MINOR)   );
    const DWORD E_FILE_INST_FILE(  MkErr(F_FILE, A_INST,  O_DATAFILE,S_MINOR)   );
    const DWORD E_FILE_OPEN_FILE(  MkErr(F_FILE, A_OPEN,  O_DATAFILE,S_MINOR)   );
    const DWORD E_FILE_WRITE_FILE( MkErr(F_FILE, A_WRITE, O_DATAFILE,S_MINOR)   );
    const DWORD E_FILE_CREAT_FILE( MkErr(F_FILE, A_CREAT, O_DATAFILE,S_MINOR)   );
	
    // can't create network connection
    const DWORD E_NET_CREAT_CONN(  MkErr(F_NET,  A_CREAT, O_CONNECTION, S_MAJOR) );
    // can't read from network connections
    const DWORD E_NET_READ_CONN(   MkErr(F_NET,  A_READ,  O_CONNECTION, S_MAJOR) );
	
    // can't allocate memory
    const DWORD E_MEM_CREAT_MEM(   MkErr(F_MEM, A_CREAT,    O_MEMORY, S_MAJOR)     );
	
    // can't validate parameter, bad parameter passed
    const DWORD E_INT_VAL_PARAM(   MkErr(F_INT,  A_VAL,   O_PARAMETER, S_FUNC)  );
    // can't validate data, internal data error
    const DWORD E_INT_VAL_DATA(    MkErr(F_INT,  A_VAL,   O_DATA, S_MINOR)       );
    // invalid method call
    const DWORD E_INT_VAL_METHOD(  MkErr(F_INT, A_VAL,   O_METHOD, S_FUNC, R_INVALIDMTD)     );
    // unsupported method
    const DWORD E_INT_VAL_METHOD_NS(  MkErr(F_INT,  A_VAL,   O_METHOD, S_FUNC, R_NOTSUPPORTED)     );
    // can't create object instance
    const DWORD E_INT_CREAT_INST(  MkErr(F_INT,  A_CREAT, O_INSTANCE, S_MAJOR)   );
    // can't get/find property in object
    const DWORD E_INT_OPEN_PROP(   MkErr(F_INT,  A_OPEN,  O_PROPERTY, S_MINOR)   );
    // instance passed in with bad properties
    const DWORD E_INT_VAL_PARAM_BP( MkErr(F_INT, A_VAL, O_PARAMETER, S_FUNC, R_BADPROP) );
	
    // can't write an instance of a read only class
    const DWORD E_INT_WRITE_INST_RO(  MkErr(F_INT,  A_WRITE, O_INSTANCE, S_FUNC, R_READONLY) );
    const DWORD E_INT_WRITE_INST(  MkErr(F_INT,  A_WRITE, O_INSTANCE, S_FUNC));
    // can't create instance that already exists
    const DWORD E_INT_CREAT_INST_AE(  MkErr(F_INT, A_CREAT, O_INSTANCE, S_FUNC, R_ALREADYEXISTS)  );
    // can't update instance that doesn't exists
    const DWORD E_INT_WRITE_INST_NF(  MkErr(F_INT, A_WRITE, O_INSTANCE, S_FUNC, R_NOTFOUND)  );
    const DWORD E_INT_OPEN_INST( MkErr(F_INT, A_OPEN, O_INSTANCE, S_MINOR, R_NOTFOUND));
    const DWORD E_INT_DEL_INST ( MkErr(F_INT, A_DEL,  O_INSTANCE, S_MINOR));
    const DWORD E_INT_PERST_INST ( MkErr(F_INT, A_PERST, O_INSTANCE, S_MINOR));
    
	
	// can't update collections
    const DWORD E_INT_WRITE_COLL( MkErr(F_INT, A_WRITE, O_COLLECTION, S_MINOR) );
    // can't verify collection (couldn't verify member class name)
    const DWORD E_INT_VAL_COLL( MkErr(F_INT, A_VAL, O_COLLECTION, S_FUNC));
    // could not lock the collection
    const DWORD E_INT_LOCK_COLL(MkErr(F_INT, A_LOCK, O_COLLECTION, S_FUNC));
	
    // bad WQL query/ can't parse query
    const DWORD E_INT_VAL_QUERY( MkErr(F_INT, A_VAL, O_QUERY, S_FUNC) );
    // unsupported query
    const DWORD E_INT_VAL_QUERY_NS( MkErr(F_INT, A_VAL, O_QUERY, S_FUNC, R_NOTSUPPORTED) );
	
	
    // can't get SQL connection
    const DWORD E_SQL_CREAT_CONN(  MkErr(F_SQL, A_CREAT,    O_CONNECTION, S_MAJOR)  );
    // general SQL problem
    const DWORD E_SQL_VAL_CONN(    MkErr(F_SQL,  A_VAL,   O_CONNECTION, S_MAJOR)  );
    // can't open transaction
    const DWORD E_SQL_CREAT_TRANS( MkErr(F_SQL,  A_CREAT, O_TRANSACTION, S_MAJOR) );
    // can't commit transaction
    const DWORD E_SQL_WRITE_TRANS( MkErr(F_SQL,  A_WRITE, O_TRANSACTION, S_MAJOR) );
    // can't exec stored procedure
    const DWORD E_SQL_EXEC_METHOD( MkErr(F_SQL,  A_EXEC,  O_METHOD,     S_MAJOR) );
    // can't exec query
    const DWORD E_SQL_EXEC_QUERY(  MkErr(F_SQL,  A_EXEC,  O_QUERY,      S_MINOR) );
    // can't find/retrieve row
    const DWORD E_SQL_OPEN_ROW(    MkErr(F_SQL,  A_OPEN,  O_ROW,        S_MINOR) );
	// can't retrieve data
    const DWORD E_SQL_READ_ROW(   MkErr(F_SQL,  A_WRITE, O_ROW,        S_MAJOR) );
    // can't insert row
	const DWORD E_SQL_PERST_ROW(   MkErr(F_SQL,  A_PERST, O_ROW,        S_MINOR) );
    // can't delete row
    const DWORD E_SQL_DEL_ROW(     MkErr(F_SQL,  A_DEL,   O_ROW,        S_MINOR) );
    // can't update row
    const DWORD E_SQL_WRITE_ROW(   MkErr(F_SQL,  A_WRITE, O_ROW,        S_MINOR) );
	
    // can't validate security rights
    const DWORD E_SEC_VAL_RIGHT(   MkErr(F_SEC,  A_VAL,   O_RIGHTS, S_FUNC)     );
    // can't initialize security descriptor
    const DWORD E_SEC_CREAT_DESC(   MkErr(F_SEC,  A_INST,  O_DESCRIPTOR, S_MAJOR) );
    // can't get rights for object
    const DWORD E_SEC_OPEN_RIGHTS(  MkErr(F_SEC, A_OPEN,  O_RIGHTS, S_MINOR) );
    // invalid user login
    const DWORD E_SEC_VAL_USER(     MkErr(F_SEC, A_VAL,   O_USER, S_MINOR)   );
    // Unable to get security source connection
    const DWORD E_SEC_CREAT_CONN(   MkErr(F_SEC, A_CREAT, O_CONNECTION, S_MAJOR));
    // Uncable to create instance of base class
    const DWORD E_BC_CREAT_INST(    MkErr(F_BC, A_CREAT, O_INSTANCE, S_MAJOR));
	// Request to read Resource/Group data with missing or incorrect collection limiting
    const DWORD E_SEC_READ_COLLLIMIT(   MkErr(F_SEC, A_READ, O_RESOURCE, S_MINOR));
	
	
    // can't read/parse line of text file
    const DWORD E_INT_READ_LINE( MkErr(F_INT,A_READ,O_FILELINE,S_FUNC) );
	
	
};


/********** PDF Error Code and Warnings  **********/
// when the reason bits are set to R_PDFERROR, SMS_ExtendedStatus will be one of these error codes
#define PDF_NO_ERROR                0
#define PDF_ERROR                   1
#define PDF_ERROR_FILENOTFOUND      2
#define PDF_ERROR_NOTPDF            3   // The file is not a PDF file
#define PDF_ERROR_VERSION           4   // The file is not of a recognized PDF version
#define PDF_ERROR_FILEIO            7   // Couldn't read the file
#define PDF_ERROR_ICON_FILEIO       8   // Couldn't read the file icon
#define PDF_ERROR_MISSINGNAME       9   // The PDF file is missing the package Name key
#define PDF_ERROR_MISSINGLANG       11  // The PDF file is missing the package Language key
#define PDF_ERROR_MISSINGPUB        12  // The PDF file is missing the package Publisher key
#define PDF_ERROR_NOPROGS           13  // The PDF file has no programs defined
#define PDF_ERROR_MISSINGPROGINFO   14  // The PDF is missing required program keys (1.0 (SMS 1.2) only)
#define PDF_ERROR_PROGRMISSINGNAME  16  // A program is missing the Name key
#define PDF_ERROR_PROGMISSINGCMDLN  17  // A program is missing the CommandLine key
#define PDF_ERROR_BADTYPE           18  // The PDF is of the wrong type (it's a client component PDF)
#define PDF_ERROR_DATABASE          19  // Database error accessing the supported platforms table.
#define PDF_ERROR_DUPPROGRAM        20  // There are two programs with the same name.
#define PDF_ERROR_LAST              21

// warning bits returned by SMS_PDF_Package.LoadPDF()
#define BIT(N)                      (1UL<<(N))
#define PDF_WARN_RUN                BIT(0)  // invalid Run info specified
#define PDF_WARN_RESTART            BIT(1)  // invalid Restart info specified
#define PDF_WARN_CANRUNWHEN         BIT(2)  // invalid AfterRunning info specified
#define PDF_WARN_ASSIGNMENT         BIT(3)  // invalid Assignment info specified
#define PDF_WARN_BADDEPNDPROG       BIT(4)  // invalid DependentProgram info specified
#define PDF_WARN_BADDL              BIT(5)  // invalid SpecifyDrive info specified
#define PDF_WARN_BADDISKSPREQ       BIT(6)  // invalid EstimatedDiskSpace info specified
#define PDF_WARN_NOSUPCLINFO        BIT(7)  // no SupportedClients info specified
#define PDF_WARN_BADSUPCLINFO       BIT(8)  // invalid SupportedClients info specified
#define PDF_WARN_VER1PDF            BIT(9)  // version 1.0 file used
#define PDF_WARN_REMPROGNOKEY       BIT(10) // RemoveProgram is set but no UninstallKey given



#endif