VERSION 1.0 CLASS
BEGIN
  MultiUse = 0   'False
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "WorkerProvider"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Worker Provider"
Option Explicit

Implements APEInterfaces.IWorkerProvider

Private moInitialErr As ErrObject    'Error that occurred during Class_Initialize

Public Function IWorkerProvider_GetWorker() As APEInterfaces.IWorker
Attribute IWorkerProvider_GetWorker.VB_Description = "Returns a new AEWorker.Worker instance."
    'Raise error if error occured during Class_Initialize
    If Not moInitialErr Is Nothing Then
        Err.Raise Err.Number, , Err.Description
    End If
    
    Set IWorkerProvider_GetWorker = CreateObject("AEWorker.Worker")
End Function

Private Sub Class_Initialize()
    'Make sure the AEWorker.Worker Class is registered
    'local because AEClient may have run on machine
    'previously.  It could leave AEWorker.Worker registered
    'remote if it did not unload properly.
    Dim oRacReg As RacReg.RegClass
    Dim iResult As Integer          'Error return code
    On Error GoTo Class_InitializeError
    
    Set oRacReg = New RacReg.RegClass
    iResult = oRacReg.SetAutoServerSettings(False, "AEWorker.Worker")
    If iResult > 0 Then
        'Error occurred in RacReg
        Err.Raise giRACREG_ERROR + vbObjectError, , ReplaceString(LoadResString(giRACREG_ERROR), gsNAME_TOKEN, LoadResString(iResult + giRACREG_ERROR_CODE_OFFSET))
    End If
    Exit Sub
Class_InitializeError:
    Set moInitialErr = Err
End Sub
