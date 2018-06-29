/*=============================================================================

    Copyright (c) 1998  Microsoft Corporation

    Module Name:

        clfslsn.h

    Abstract:

        Header file containing the publicly defined LSN data structure for the
        common log file system.

    Author:

        Dexter Bradshaw    [DexterB]   09-Dec-1998


    Revision History:

=============================================================================*/

#ifndef _CLFS_LSN_H_
#define _CLFS_LSN_H_

//
// CLS_LSN
//

typedef struct _CLS_LSN
{

    ULONGLONG               Internal;      

} CLS_LSN, *PCLS_LSN, **PPCLS_LSN;

//
// Alias CLS prefixed types with CLFS prefixes.
//

typedef CLS_LSN CLFS_LSN;
typedef CLFS_LSN *PCLFS_LSN, **PPCLFS_LSN;

#endif