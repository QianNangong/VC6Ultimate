VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsClientService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class is used a structure for information about expected
'Service Request callbacks.  Objects of this class are
'added to the gcService collection
'-------------------------------------------------------------------------

Public sID As String          'Service Request ID
Public sCommand As String   'Service Request Command
Public lStartTicks As Long  'Tick Count of when call was made

