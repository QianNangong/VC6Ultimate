VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "CallBackRef"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'Purpose:   This class forms a data structure to store Service request
'           information.  New CallBacksRef objects can be added to a
'           collection to store the data
'-------------------------------------------------------------------------
Private mvResult As Variant     ' Stores the data to be returned

Public ServiceID As String        'Service Request ID
Public Object As APEInterfaces.IClientCallback  'Callback object that will be called
Public SyncObject As EventReturn
Public Error As String          'Error description that occurred in Worker
                                'while processing task.
Public UseSyncEvent As Boolean
Public CallAttempts As Long     'The number of failed attempts to call the
                                'Callback method of the Object property

Public Property Get Result() As Variant
    Select Case VarType(mvResult)
        Case vbEmpty, vbNull
            Result = Null
        Case vbObject, vbError, vbDataObject
            Set Result = mvResult
        Case Else
            Result = mvResult
    End Select
End Property

Public Property Let Result(ByVal vNewValue As Variant)
    mvResult = vNewValue
End Property

Public Property Set Result(ByVal vNewValue As Variant)
    Set mvResult = vNewValue
End Property

