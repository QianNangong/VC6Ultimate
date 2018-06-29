VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "MoveMoney"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE MTS Transaction Service (MoveMoney)"
Option Explicit

Implements APEInterfaces.IMTSMoveMoney

Public Sub Transfer(sConnect As String, eConnectOptions As ape_DbConnectionOptions, lFromAccount As Long, lToAccount As Long, lAmount As Long)
    '-------------------------------------------------------------------------
    'Purpose:
    '   Provides an interface for late binding.  Late binding is only provided
    '   for test comparison.  Other custom services should only use the implemented
    '   interface.
    '-------------------------------------------------------------------------
    IMTSMoveMoney_Transfer sConnect, eConnectOptions, lFromAccount, lToAccount, lAmount
End Sub

Private Sub IMTSMoveMoney_Transfer(sConnect As String, eConnectOptions As APEInterfaces.ape_DbConnectionOptions, lFromAccount As Long, lToAccount As Long, lAmount As Long)
    ' get our object context
    Dim ctxObject As ObjectContext
    Set ctxObject = GetObjectContext()
 
    On Error GoTo TransferError
 
    ' create the account object using our context
    Dim objAccount As APEInterfaces.IMTSAccount
    Set objAccount = ctxObject.CreateInstance("AEMTSSvc.Account")
    
    If objAccount Is Nothing Then
        Err.Raise errAccountCreateFailed, , LoadResString(ERROR_COULD_NOT_CREATE_ACCOUNT_OBJECT)
    End If
    
    ' do the credit
    objAccount.Post sConnect, eConnectOptions, lToAccount, lAmount
    ' then do the debit
    objAccount.Post sConnect, eConnectOptions, lFromAccount, -lAmount
    ctxObject.SetComplete
    Exit Sub
 
TransferError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    ctxObject.SetAbort
    Err.Raise lErrorNumber, , sErrorDescription
End Sub

