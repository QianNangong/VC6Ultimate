Attribute VB_Name = "modWin32Errors"
Option Explicit
'-------------------------------------------------------------------------
'This Module provides Windows Error constants
'-------------------------------------------------------------------------

Public Const ERR_ACCESS_DENIED As Integer = 5
Public Const RPC_E_CALL_REJECTED = &H80010001
Public Const RPC_E_SERVER_DIED_DNE = &H80010012
Public Const RPC_S_INVALID_RPC_PROTSEQ As Integer = 1704
Public Const RPC_S_PROTSEQ_NOT_SUPPORTED As Integer = 1703
Public Const ERR_CANT_FIND_KEY_IN_REGISTRY = &H80040152     'Occurs when a client app tries to Create an object
                                                            'that was previously created while registered remotely
                                                            'then registered locally
Public Const ERR_CALL_FAILED_DIDNOT_EXECUTE = &H80010012
Public Const ERR_NO_MORE_ENDPOINTS = &H800706D9             'There are no more endpoints available from the endpoint mapper.
Public Const RPC_S_UNKNOWN_AUTHN_TYPE = &H800706CD          'Error occurs when trying to connect using an authentication type
                                                            'not supported by the server
Public Const REGDB_E_IIDNOTREG = &H80040155                 'Interface not registered
Public Const RPC_PROTOCOL_SEQUENCE_NOT_FOUND = &H800706D0             'The RPC protocol sequence was not found

