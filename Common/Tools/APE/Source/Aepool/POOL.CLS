VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "Pool"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Pool Manager"
Option Explicit

Implements APEInterfaces.IPool

'Private variable to hold the WorkerID of the Worker which was
'passed to a client
Private mlWorkerID As Long
'A count of references is kept to keep track of
'how many references to this same worker it has.
'When the count is 0, moWorker is added back into
'the global collection and set to nothing.
Private mlReferenceCount As Long

Public Function IPool_GetWorker() As APEInterfaces.IWorker
Attribute IPool_GetWorker.VB_Description = "Returns a AEWorker.Worker object and reserves it for the calling applications use."
    'A client uses this method to get a reference to a worker
    'Find a worker that is not busy (passed to another client).
    'Mark the Worker as busy
    'Store the WorkerID in the Private Class Module level
    'mlWorkerID
    Dim oWork As clsWorker
    Dim bFoundWorker As Boolean
    On Error GoTo GetWorkerError
    LogEvent giGET_WORKER
    'If mlWorkerID does not equal zero then the client
    'that has reference to this instance of this class
    'already has a worker.  Therefore just pass a reference
    'to the same worker.
    If mlWorkerID <> 0 Then
        mlReferenceCount = mlReferenceCount + 1
        Set IPool_GetWorker = gcWorkers.Item(CStr(mlWorkerID)).Worker
        glRequestsSatisfied = glRequestsSatisfied + 1
        If gbShow Then
            With frmPoolMgr.lblSatisfied
                .Caption = CStr(glRequestsSatisfied)
                .Refresh
            End With
        End If
    Else
        'The client does not have any other references to a Worker
        'Find a worker that does not have any connections made
        'by other clients
        bFoundWorker = False
        For Each oWork In gcWorkers
            If Not oWork.Busy Then
                oWork.Busy = True
                mlWorkerID = oWork.ID
                mlReferenceCount = mlReferenceCount + 1
                bFoundWorker = True
                Set IPool_GetWorker = oWork.Worker
                'Update statistics
                glRequestsSatisfied = glRequestsSatisfied + 1
                If gbShow Then
                    With frmPoolMgr.lblSatisfied
                        .Caption = CStr(glRequestsSatisfied)
                        .Refresh
                    End With
                End If
                Exit For
            End If
        Next oWork
        If Not bFoundWorker Then
            'All workers are being used by other clients
            'set function equal to nothing
            Set IPool_GetWorker = Nothing
            glRequestsRejected = glRequestsRejected + 1
            If gbShow Then
                With frmPoolMgr.lblRejected
                    .Caption = CStr(glRequestsRejected)
                    .Refresh
                End With
            End If
        End If
    End If
    Exit Function
GetWorkerError:
    Select Case Err.Number
        Case ERR_OVER_FLOW
            LogError Err
            If glRequestsSatisfied = glMAX_LONG Then glRequestsSatisfied = 0
            If glRequestsRejected = glMAX_LONG Then glRequestsRejected = 0
            Resume Next
        Case Else
            LogError Err
            Err.Raise Err.Number, Err.Source, Err.Description
    End Select
End Function

Public Sub IPool_ReleaseWorker()
Attribute IPool_ReleaseWorker.VB_Description = "Notifies the AEPoolMgr that an AEWorker.Worker object received by GetWorker is no longer referenced by the calling application."
    'Called by a client when it destroys a reference to
    'a worker that it received by calling GetWorker
    'Check to see if the client has another reference to
    'the worker.  If it does not, mark the worker that was passed
    'to the client as not busy and set mlWorkerID = to 0.
    LogEvent giRELEASE_WORKER
    'If ReferenceCount is zero then this method is being called without
    'having an unreleased reference to the worker
    If mlReferenceCount = 0 Then Exit Sub
    mlReferenceCount = mlReferenceCount - 1
    'If ReferenceCount = 0 now then client has released
    'its only reference to a worker
    If mlReferenceCount = 0 Then
        gcWorkers.Item(CStr(mlWorkerID)).Busy = False
        mlWorkerID = 0
    End If
End Sub

Private Sub Class_Initialize()
    CountInitialize
End Sub

Private Sub Class_Terminate()
    CountTerminate
End Sub


