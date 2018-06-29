VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsCallback"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "Callback object passed to AEQueueMgr.Queue for the return of Service Request results."
Option Explicit
    '-------------------------------------------------------------------------
    'Used to for callback objects to be sent with Service Requests to QueueMgr.
    '-------------------------------------------------------------------------
Implements APEInterfaces.IClientCallback

Private Sub IClientCallback_CallBack(ByVal sServiceID As String, ByVal vServiceReturn As Variant, ByVal sServiceError As String)
    '-------------------------------------------------------------------------
    'Purpose:   Used by the Expediter to notify a Client when an Service is complete.
    'IN:
    '   [sServiceID]
    '           Service Request ID
    '   [vServiceReturn]
    '           Data returned by Service Request
    '   [sServiceError]
    '           Error information for errors that occured processing Service Request.
    '           Information is delimited by a semi-colon and a space in the following
    '           format:  "number; source; description"
    'Effects:
    '   Calls CallbackHandler procedure
    '-------------------------------------------------------------------------
    CallBackHandler sServiceID, vServiceReturn, sServiceError
End Sub
