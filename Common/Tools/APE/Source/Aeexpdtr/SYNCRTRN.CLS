VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "EventReturn"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Attribute VB_Description = "APE Expediter Event Return Interface"
Option Explicit

'******************
'Events
'******************
Public Event ServiceResult(ByVal sServiceID As String, ByVal vServiceReturn As Variant, ByVal sServiceError As String)
Attribute ServiceResult.VB_Description = "Returns a Service Request result"

'*******************
'Friend methods
'*******************
Friend Sub RaiseServiceResult(sServiceID As String, vServiceReturn As Variant, sServiceError As String)
    RaiseEvent ServiceResult(sServiceID, vServiceReturn, sServiceError)
End Sub

