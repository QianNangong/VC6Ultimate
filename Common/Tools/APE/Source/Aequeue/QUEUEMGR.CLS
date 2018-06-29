VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "QueueMgr"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Job Manager"
Option Explicit
'-------------------------------------------------------------------------
'This public multi-use class provides the OLE interface for the APE Manager
'or another application designed to be the Manager
' It implements the IQueueManager interface
'-------------------------------------------------------------------------

Implements APEInterfaces.IQueueManager

'***********************
'Public Properties
'***********************
Private Property Get IQueueManager_IsIdle() As Boolean
    '-------------------------------------------------------------------------
    'Purpose:   Returns True if the Queue is idle.
    '-------------------------------------------------------------------------
    If gcQueue.Count = 0 Then
        Dim oWorker As clsWorker
        IQueueManager_IsIdle = True
        For Each oWorker In gcWorkers
            If oWorker.Busy Then
                IQueueManager_IsIdle = False
                Exit For
            End If
        Next
    Else
        IQueueManager_IsIdle = False
    End If
End Property

Public Property Let IQueueManager_MaxQueueSize(ByVal lMax As Long)
Attribute IQueueManager_MaxQueueSize.VB_Description = "Sets or returns the value that controls the size of the Queue that causes Service Requests to be rejected."
    '-------------------------------------------------------------------------
    'Purpose:   MaxQueueSize property determines the maximum amount
    '           of ServiceRequests that will be allowed to back up
    'Effects:   [glMax] becomes value of parameter
    '-------------------------------------------------------------------------
    glMaxQueueSize = lMax
End Property

Public Property Get IQueueManager_MaxQueueSize() As Long
    IQueueManager_MaxQueueSize = glMaxQueueSize
End Property

Public Property Let IQueueManager_ShowQueue(ByVal bShow As Boolean)
Attribute IQueueManager_ShowQueue.VB_Description = "Determines whether the AEQueueMgr shows a form."
    '-------------------------------------------------------------------------
    'Purpose:   Show property determines whether or not a form
    '           is displayed while QueueMgr is loaded
    'Effects:   [gbShow] becomes value of parameter
    '           If parameter is true frmQueueMgr is show, else form
    '           is Unloaded.
    '-------------------------------------------------------------------------
    If Not gbShow = bShow Then
        gbShow = bShow
        If bShow Then
            With frmQueueMgr
                .Show
                .lblCount.Caption = glAddCallCount
                .lblPeak.Caption = glPeakQueueSize
                .lblQueue.Caption = gcQueue.Count
                .lblWorkerCount.Caption = gcWorkers.Count
            End With
        Else
            Unload frmQueueMgr
        End If
    End If
    
End Property

Public Property Get IQueueManager_ShowQueue() As Boolean
    IQueueManager_ShowQueue = gbShow
End Property

Public Property Let IQueueManager_LogQueue(ByVal bLog As Boolean)
Attribute IQueueManager_LogQueue.VB_Description = "Determines if the AEQueueMgr logs its events and errors to the AELogger.Logger object."
    '-------------------------------------------------------------------------
    'Purpose:   If log is true create logger class object and log Services
    'Effects:   [gbLog] becomes value of parameter
    '           [goLogger] is set to a new AELogger.Logger object if parameter
    '                      is true.  If false goLogger is destroyed
    '-------------------------------------------------------------------------
    On Error GoTo LogQueueError
    If Not gbLog = bLog Then
        gbLog = bLog
        If bLog Then
            Set goLogger = CreateObject("AELogger.Logger")
        Else
            Set goLogger = Nothing
        End If
    End If
    Exit Property
LogQueueError:
    Select Case Err.Number
        Case ERR_CANT_FIND_KEY_IN_REGISTRY
            'AEInstancer.Instancer is a work around for error
            '-2147221166 which occurrs every time a client
            'object creates an instance of a remote server,
            'destroys it, registers it local, and tries to
            'create a local instance.  The client can not
            'create an object registered locally after it created
            'an instance while it was registered remotely
            'until it shuts down and restarts.  Therefore,
            'it works to call another process to create the
            'local instance and pass it back.
            Dim oInstancer As APEInterfaces.IInstancer
            Set oInstancer = CreateObject("AEInstancer.Instancer")
            Set goLogger = oInstancer.Object("AELogger.Logger")
            Set oInstancer = Nothing
            Resume Next
        Case Else
            Err.Raise Err.Number, Err.Source, Err.Description
    End Select
End Property

Public Property Get IQueueManager_LogQueue() As Boolean
    IQueueManager_LogQueue = gbLog
End Property

'********************
'Worker Properties
'********************
Public Property Let IQueueManager_LogWorkers(ByVal bLog As Boolean)
Attribute IQueueManager_LogWorkers.VB_Description = "Sets the value that is used to set the Log property of AEWorker.Worker objects."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Log property of all the Workers
    'Effects:
    '   [gbLogWorkers]
    '           becomes equal to the passed parameter
    'Assumes:   There is a collection of one or more valid Worker objects
    '-------------------------------------------------------------------------
    'If the property setting actually
    'changes the current property pass
    'the property change to all the Workers
    Dim oWork As clsWorker
    If Not bLog = gbLogWorkers Then
        For Each oWork In gcWorkers
            oWork.Worker.Log = bLog
        Next oWork
        gbLogWorkers = bLog
    End If
End Property

Public Property Get IQueueManager_LogWorkers() As Boolean
    IQueueManager_LogWorkers = gbLogWorkers
End Property

Public Property Let IQueueManager_PersistentServices(ByVal bPersistent As Boolean)
Attribute IQueueManager_PersistentServices.VB_Description = "Sets the value that is used to set the PersistentServices property of AEWorker.Worker objects."
    '-------------------------------------------------------------------------
    'Purpose:   To set the PersistentServices property of all the Workers
    'Effects:
    '   [gbPersistentServices]
    '           becomes equal to the passed parameter
    'Assumes:   There is a collection of one or more valid Worker objects
    '-------------------------------------------------------------------------
    'If the property setting actually
    'changes the current property pass
    'the property change to all the Workers
    Dim oWork As clsWorker
    If Not bPersistent = gbPersistentServices Then
        For Each oWork In gcWorkers
            oWork.Worker.PersistentServices = bPersistent
        Next oWork
        gbPersistentServices = bPersistent
    End If
End Property
    
Public Property Get IQueueManager_PersistentServices() As Boolean
    IQueueManager_PersistentServices = gbPersistentServices
End Property

Public Property Let IQueueManager_EarlyBindServices(ByVal bEarlyBind As Boolean)
Attribute IQueueManager_EarlyBindServices.VB_Description = "Sets the value that is used to set the EarlyBindServices property of AEWorker.Worker objects."
    '-------------------------------------------------------------------------
    'Purpose:   To set the EarlyBindServices property of all the Workers
    'Effects:
    '   [gbEarlyBindServices]
    '           becomes equal to the passed parameter
    'Assumes:   There is a collection of one or more valid Worker objects
    '-------------------------------------------------------------------------
    'If the property setting actually
    'changes the current property pass
    'the property change to all the Workers
    Dim oWork As clsWorker
    If Not bEarlyBind = gbEarlyBindServices Then
        For Each oWork In gcWorkers
            oWork.Worker.EarlyBindServices = bEarlyBind
        Next oWork
        gbEarlyBindServices = bEarlyBind
    End If
End Property

Public Property Get IQueueManager_EarlyBindServices() As Boolean
    IQueueManager_EarlyBindServices = gbEarlyBindServices
End Property

'********************
'Expediter Properties
'********************

Public Property Let IQueueManager_ShowExpediter(ByVal bShow As Boolean)
Attribute IQueueManager_ShowExpediter.VB_Description = "Sets the Show property of the AEExpediter.Expediter object."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Show property of the Expediter
    'Effects:
    '   [gbShowExpediter]
    '           becomes equal to the passed parameter
    'Assumes:
    '   [goExpediter]
    '           Is a valid AEExpediter.Expediter object
    '-------------------------------------------------------------------------
    'If the property setting actually
    'changes the current property pass
    'the property change to the expediter
    If Not bShow = gbShowExpediter Then
        goExpediter.Show = bShow
        gbShowExpediter = bShow
    End If
End Property

Public Property Get IQueueManager_ShowExpediter() As Boolean
    IQueueManager_ShowExpediter = gbShowExpediter
End Property

Public Property Let IQueueManager_LogExpediter(ByVal bLog As Boolean)
Attribute IQueueManager_LogExpediter.VB_Description = "Set the Log property of the AEExpediter.Expediter object."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Log property of the Expediter
    'Effects:
    '   [gbLogExpediter]
    '           becomes equal to the passed parameter
    'Assumes:
    '   [goExpediter]
    '           Is a valid AEExpediter.Expediter object
    '-------------------------------------------------------------------------
    'If the property setting actually
    'changes the current property pass
    'the property change to the expediter
    If Not bLog = gbLogExpediter Then
        goExpediter.Log = bLog
        gbLogExpediter = bLog
    End If
End Property

Public Property Get IQueueManager_LogExpediter() As Boolean
    IQueueManager_LogExpediter = gbLogExpediter
End Property

'****************************
'Public Methods
'****************************

Public Sub IQueueManager_SetProperties(ByVal bShow As Boolean, Optional ByVal bLog As Variant)
Attribute IQueueManager_SetProperties.VB_Description = "Sets all of the AEQueueMgr.QueueMgr related properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the QueueMgr properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           ShowQueue, LogQueue, WorkerQuantity
    '-------------------------------------------------------------------------
    With Me
        .IQueueManager_ShowQueue = bShow
        If Not IsMissing(bLog) Then .IQueueManager_LogQueue = bLog
    End With
End Sub

Public Sub IQueueManager_SetWorkerProperties(ByVal bLog As Boolean, Optional ByVal bEarlyBindServices As Variant, _
        Optional ByVal bPersistentServices As Variant)
Attribute IQueueManager_SetWorkerProperties.VB_Description = "Sets all of the AEWorker.Worker related properties on one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Worker properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           ShowWorkers, LogWorkers, EarlyBindServices, PersistentServices
    '-------------------------------------------------------------------------
    Dim oWork As clsWorker
    gbLogWorkers = bLog
    If Not IsMissing(bEarlyBindServices) Then gbEarlyBindServices = bEarlyBindServices
    If Not IsMissing(bPersistentServices) Then IQueueManager_PersistentServices = bPersistentServices
    For Each oWork In gcWorkers
        oWork.Worker.SetProperties gbLogWorkers, gbEarlyBindServices, gbPersistentServices
    Next oWork
End Sub

Public Sub IQueueManager_SetExpediterProperties(ByVal bShow As Boolean, Optional ByVal bLog As Variant)
Attribute IQueueManager_SetExpediterProperties.VB_Description = "Sets all of the AEExpediter.Expediter related properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Expediter properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           ShowExpediter, LogExpediter
    '-------------------------------------------------------------------------
    gbShowExpediter = bShow
    If Not IsMissing(bLog) Then gbLogExpediter = bLog
    goExpediter.SetProperties gbShowExpediter, gbLogExpediter
    
End Sub

Public Sub IQueueManager_SetConnectionProperties(ByVal bUseDCOM As Boolean, Optional ByVal sProtocol As Variant, _
                                    Optional ByVal lAuthentication As Variant)
Attribute IQueueManager_SetConnectionProperties.VB_Description = "Sets the connection parameters to be used when creating remote AEWorker.Worker objects."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Connection Settings that the QueueMgr will use
    '           to connect to remote Workers
    'In:
    '   [bUseDCOM]
    '           If true workers will be created using DCOM instead of
    '           Remote Automation.
    '   [sProtocol]
    '           Protocol sequence to use when connecting to remote objects
    '   [lAuthentication]
    '           Authentication level to use
    'Effects:
    '   [gbUseDCOM]
    '           becomes equal to bUseDCOM parameter
    '   [gsProtocol]
    '           becomes equal to sProtocol parameter
    '   [glAuthentication]
    '           becomes equal to lAuthentication parameter
    '-------------------------------------------------------------------------
    Dim iVarType As Integer     'Variant type code of lAuthentication
    gbUseDCOM = bUseDCOM
    If Not IsMissing(sProtocol) Then
        If VarType(sProtocol) = vbString Then gsProtocol = sProtocol
    End If
    If Not IsMissing(lAuthentication) Then
        iVarType = VarType(lAuthentication)
        If iVarType = vbLong Or iVarType = vbInteger Or iVarType = vbDouble Or iVarType = vbSingle Then
            glAuthentication = lAuthentication
        End If
    End If
End Sub

Public Function IQueueManager_CreateWorkers(ByVal bRemoteWorkers As Boolean, Optional ByVal lWorkerQuantity As Variant, _
                                    Optional ByVal lWorkersPerMachine As Variant, Optional ByVal vaMachineList As Variant, _
                                    Optional ByVal bUseLocalMachine As Variant) As String
Attribute IQueueManager_CreateWorkers.VB_Description = "Creates AEWorker.Worker objects.  Returns a string that describes any errors that occurred."
    '-------------------------------------------------------------------------
    'Purpose:   Sets the settings for remote workers.  These settings provide
    '           The QueueMgr the information needed to create Workers on several
    '           remote machines rather than just the local one.
    'IN:
    '   [bRemoteWorkers]
    '           If true, the QueueMgr will create Workers on remote machines.
    '           If false, the QueueMgr will only create Workers on the local machine.
    '   [lWorkerQuantity]
    '           The total number of Workers to be created.
    '   [lWorkersPerMachine]
    '           A variant long specifing the maximum allowed number of Workers
    '           to create on a single machine.
    '   [vaMachineList]
    '           A string array, providing the list of machine names
    '           to create the workers on.  If this is not a valid
    '           array of strings it will be treated like no machine
    '           names were specified
    '   [bUseLocalMachine]
    '           If true, include local machine in list of remote machine names
    'Return:    String to display to user and print to log file.  Will contain
    '           any error information and the total number of workers created
    '-------------------------------------------------------------------------
                                    
    Static stbUseDCOM As Boolean     'Last DCom automation setting used
    Static stsProtocol As String     'Last Automation protocol setting used
    Static stlAuthentication As Long 'Last Automation Authentication setting used
    Dim sResult As String   'Result of SetWorkersOnMachine function
    Dim sErrors As String   'String with error descriptions to return for
                            'display to user
    Dim oWorkerMachine As clsWorkerMachines 'Object in gcWorkerMachines collection
                                            'that stores how many workers are instanciated
                                            'on a particular machine
    Dim lUB As Long         'Ubound of passed array
    Dim bListExists As Boolean  'True if a array of machine names exists
    Dim bInList As Boolean  'If true the Machine Name is in the passed array
    Dim i As Integer        'For...Next loop counter
    Dim lAdd As Long        'Number of Workers to add on machine
    Dim lNumOnMach As Long  'Number of workers on a machine
    Dim iVarType As Integer 'Variant data type of a parameter
    
    On Error GoTo CreateWorkersError
    
    'Validate the parameters
    'validate lWorkerQuantity
    iVarType = VarType(lWorkerQuantity)
    If Not (iVarType = vbLong Or iVarType = vbInteger Or iVarType = vbSingle Or iVarType = vbDouble) Then
        Err.Raise giINVALID_PARAMETER, , LoadResString(giINVALID_PARAMETER)
    ElseIf lWorkerQuantity <= 0 Then
        'Make sure at least one worker is created
        lWorkerQuantity = 1
    End If
    If bRemoteWorkers Then
        'validate vaMachineList
        iVarType = VarType(vaMachineList)
        If (iVarType = vbArray + vbString) Or (iVarType = vbArray + vbVariant) Then
            On Error Resume Next
            lUB = UBound(vaMachineList)
            If Err.Number <> ERR_SUBSCRIPT_OUT_OF_RANGE Then
                bListExists = True
            End If
            On Error GoTo CreateWorkersError
        End If
        'validate lworkerspermachine
        iVarType = VarType(lWorkersPerMachine)
        If Not (iVarType = vbLong Or iVarType = vbInteger Or iVarType = vbSingle Or iVarType = vbDouble) Then
            Err.Raise giINVALID_PARAMETER, , LoadResString(giINVALID_PARAMETER)
        ElseIf lWorkersPerMachine <= 0 Then
            lWorkersPerMachine = 1
        End If
        'validate bUseLocalMachine
        On Error Resume Next
        bUseLocalMachine = CBool(bUseLocalMachine)
        If Err.Number = ERR_TYPE_MISMATCH Then
            On Error GoTo CreateWorkersError
            Err.Raise giINVALID_PARAMETER, , LoadResString(giINVALID_PARAMETER)
        Else
            On Error GoTo CreateWorkersError
        End If
    End If
    
    'First destroy all workers that can not be used any more
    'If connection settings have been changed or if bRemoteWorkers
    'is false all Workers on remote machines must be destroyed
    If (Not bRemoteWorkers) Or (stbUseDCOM <> gbUseDCOM) Or (stsProtocol <> gsProtocol) Or (stlAuthentication <> glAuthentication) Then
        'Reset the Last Connection setting static variables
        stbUseDCOM = gbUseDCOM
        stsProtocol = gsProtocol
        stlAuthentication = glAuthentication
        'Destroy all remote Workers
        For Each oWorkerMachine In gcWorkerMachines
            If oWorkerMachine.Remote Then
                sResult = SetWorkersOnMachine(True, oWorkerMachine.MachineName, 0)
                sErrors = sErrors & sResult
            End If
        Next
    Else
        'If we did not destroy all workers on remote machines
        'destroy workers that are on machines that are not
        'in the passed list of remote worker machines
        
        'Check if the machine names currently in gcWorkerMachines
        'are in the passed array
        For Each oWorkerMachine In gcWorkerMachines
            If oWorkerMachine.Remote Then
                bInList = False
                If bListExists Then
                    For i = 0 To lUB
                        If vaMachineList(i) = oWorkerMachine.MachineName Then
                            bInList = True
                            Exit For
                        End If
                    Next
                End If
                If Not bInList Then
                    sResult = SetWorkersOnMachine(True, oWorkerMachine.MachineName, 0)
                    sErrors = sErrors & sResult
                End If
            End If
        Next
    End If
    
    'See if Workers on local machine need destroyed
    If bRemoteWorkers Then
        If Not bUseLocalMachine Then
            sResult = SetWorkersOnMachine(False, "", 0)
            sErrors = sErrors & sResult
        End If
    End If
    
    'Create Workers
    If Not bRemoteWorkers Then
        'Just create all workers on local machine
        sResult = SetWorkersOnMachine(False, "", CLng(lWorkerQuantity))
        sErrors = sErrors & sResult
    Else
        'Now loop through machine name list and add workers
        'to each machine until giWorkerCount equals
        'lWorkerQuantity or the end of the machine list is
        'reached
        If giWorkerCount <= lWorkerQuantity Then
            'First create workers on local machine
            If bUseLocalMachine Then
                'Get the number of workers currently on this machine
                lNumOnMach = gcWorkerMachines.Item(1).WorkerKeys.Count
                'Set number of Workers to be on current machine
                lAdd = lWorkersPerMachine
                If lAdd > (lWorkerQuantity + lNumOnMach) - giWorkerCount Then lAdd = (lWorkerQuantity + lNumOnMach) - giWorkerCount
                sResult = SetWorkersOnMachine(False, "", lAdd)
                sErrors = sErrors & sResult
            End If
            
            If bListExists Then
                Do Until (i > lUB Or giWorkerCount = lWorkerQuantity)
                    On Error Resume Next
                    'Get the number of workers currently on this machine
                    Set oWorkerMachine = gcWorkerMachines.Item(vaMachineList(i))
                    If Err.Number = ERR_INVALID_PROCEDURE_CALL Then
                        lNumOnMach = 0
                    Else
                        lNumOnMach = oWorkerMachine.WorkerKeys.Count
                    End If
                    On Error GoTo CreateWorkersError
                    'Set number of Workers to be on current machine
                    lAdd = lWorkersPerMachine
                    If lAdd > (lWorkerQuantity + lNumOnMach) - giWorkerCount Then lAdd = (lWorkerQuantity + lNumOnMach) - giWorkerCount
                    sResult = SetWorkersOnMachine(True, CStr(vaMachineList(i)), lAdd)
                    sErrors = sErrors & sResult
                    i = i + 1
                Loop
            End If
        Else
            'There may be too many workers, so destroy workers to
            'make the right count
            If bListExists Then
                i = lUB
                Do While i >= 0
                    On Error Resume Next
                    'Get the number of workers currently on this machine
                    Set oWorkerMachine = gcWorkerMachines.Item(vaMachineList(i))
                    If Err.Number = ERR_INVALID_PROCEDURE_CALL Then
                        lNumOnMach = 0
                    Else
                        lNumOnMach = oWorkerMachine.WorkerKeys.Count
                    End If
                    On Error GoTo CreateWorkersError
                    If lNumOnMach > 0 Then
                        lAdd = 0
                        If lNumOnMach > (giWorkerCount - lWorkerQuantity) Then lAdd = lNumOnMach - (giWorkerCount - lWorkerQuantity)
                        sResult = SetWorkersOnMachine(True, CStr(vaMachineList(i)), lAdd)
                        sErrors = sErrors & sResult
                    End If
                    i = i - 1
                Loop
            End If
            
            'if there are still too many workers
            'reduce the number of workers on the local machine
            If giWorkerCount > lWorkerQuantity Then
                lNumOnMach = gcWorkerMachines.Item(1).WorkerKeys.Count
                lAdd = 0
                If lNumOnMach > (giWorkerCount - lWorkerQuantity) Then lAdd = lNumOnMach - (giWorkerCount - lWorkerQuantity)
                sResult = SetWorkersOnMachine(False, "", lAdd)
                sErrors = sErrors & sResult
            End If
        End If
    End If
    
    'Check if any workers were created and raise error if none were created
    If giWorkerCount < lWorkerQuantity Then
        If giWorkerCount = 0 Then
            Err.Raise giNO_WORKERS_CREATED, , sErrors & vbCrLf & LoadResString(giNO_WORKERS_CREATED)
        Else
            sErrors = sErrors & vbCrLf & ReplaceString(LoadResString(giONLY_N_WORKERS_CREATED), gsNUMBER_TOKEN, CStr(giWorkerCount))
        End If
    Else
        sErrors = sErrors & vbCrLf & LoadResString(giALL_WORKERS_CREATED)
    End If
    
    IQueueManager_CreateWorkers = sErrors
    Exit Function
CreateWorkersError:
    Select Case Err.Number
        Case Is > giERROR_THRESHOLD
            Err.Raise Err.Number + vbObjectError, Err.Source, Err.Description
        Case Else
            Err.Raise Err.Number, Err.Source, Err.Description
    End Select
End Function

Public Function IQueueManager_GetRemoteLoggerCollection() As Collection
Attribute IQueueManager_GetRemoteLoggerCollection.VB_Description = "Returns a collection of remote AELogger.Logger objects that were created by remote AEWorker.Worker objects."
    '-------------------------------------------------------------------------
    'Purpose:   Returnse the collection of loggers created on the same
    '           machines as remote Workers
    'Assumes:
    '   [gcWorkerMachines]
    '           a valid collection of clsWorkerMachines object
    '   [clsWorkerMachines]
    '           If .Remote is true .WorkerKeys.Count is > 0
    '-------------------------------------------------------------------------
    Dim cRemoteLoggers As Collection        'Collection to return
    Dim oWorkerMachine As clsWorkerMachines 'Object representing each Worker machine
    Dim oLogger As APEInterfaces.ILogger          'Valid logger object or nothing
    
    Set cRemoteLoggers = New Collection
    
    For Each oWorkerMachine In gcWorkerMachines
        With oWorkerMachine
            If .Remote Then
                Dim oWork As clsWorker
                Set oWork = gcWorkers.Item(CStr(.WorkerKeys(1)))
                Set oLogger = oWork.Worker.GetLogger
                If Not oLogger Is Nothing Then
                    cRemoteLoggers.Add oLogger
                End If
            End If
        End With
    Next
    
    If cRemoteLoggers.Count = 0 Then Set cRemoteLoggers = Nothing
    Set IQueueManager_GetRemoteLoggerCollection = cRemoteLoggers
End Function

Public Sub IQueueManager_LoadServiceObject(ByVal ServiceLibClass As String, ByVal vServiceConfiguration As Variant)
Attribute IQueueManager_LoadServiceObject.VB_Description = "Causes all created AEWorker.Worker objects to create an object whose ProgID matches ServiceLibClass."
    '-------------------------------------------------------------------------
    'Purpose:   Purpose is to call LoadServiceObject method in each
    '           instanciated worker.  It is ignored if gbPeristentServices
    '           is false
    'Assumes:
    '   [gcWorkers]
    '           Is a collection of valid AEWorker.Worker objects
    '-------------------------------------------------------------------------
    Dim oWork As clsWorker
    If gbPersistentServices Then
        For Each oWork In gcWorkers
            oWork.Worker.LoadServiceObject ServiceLibClass, vServiceConfiguration
        Next oWork
    End If
End Sub

Public Sub IQueueManager_StopTest()
Attribute IQueueManager_StopTest.VB_Description = "Causes the AEQueueMgr to stop processing Service Requests and to empty its queue."
    '-------------------------------------------------------------------------
    'Purpose:   Stops all Queue Managers processes
    'Effects:
    '           May call StopQueue
    '   [gbStopTest]
    '           Becomes true
    '-------------------------------------------------------------------------
    'Call this to halt the Queue Manager and the Expediter
    goExpediter.StopTest
    gbStopTest = True
    If Not gbBusyAdding And Not gbBusyGetServiceRequest Then StopQueue
    Exit Sub
End Sub

Public Sub IQueueManager_StartTest()
Attribute IQueueManager_StartTest.VB_Description = "Prepares the AEQueueMgr to process Service Requests after StopTest has been called."
    '-------------------------------------------------------------------------
    'Purpose:   Call this to allow processing of Services after calling StopTest
    'Effects:
    '           Resets U/I to look like QueueMgr just started
    '   [gbStopTest]
    '           Becomes False
    '-------------------------------------------------------------------------
    
    Dim oWork As clsWorker
    Dim iRetry As Integer
    
    On Error GoTo StartTestError
    
    'Reset stats
    gbStopTest = False
    
    With goExpediter
        iRetry = 0
        '.QueueMgrRef must be set before StartTest is called
        Set .QueueMgrRef = New clsQueueDelegator
        .StartTest
    End With
    If gbShow Then
        DisplayStatus ""
        glAddCallCount = 0
        glPeakQueueSize = 0
        With frmQueueMgr
            .lblCount.Caption = 0
            .lblPeak.Caption = 0
            .lblQueue.Caption = 0
            .lblWorkerCount.Caption = gcWorkers.Count
            .lblCount.Refresh
            .lblPeak.Refresh
            .lblQueue.Refresh
            .lblWorkerCount.Refresh
        End With
    End If
    Exit Sub
StartTestError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim il As Integer
            Dim ir As Integer
            'First check for stop test
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                LogEvent giCALL_REJECTED_RETRY, gsNULL_SERVICE_ID
                Resume
            Else
                'We reached our max retries
                LogError Err, gsNULL_SERVICE_ID
                Resume Next
            End If
        Case Else
            Err.Raise Err.Number, Err.Source, Err.Description
    End Select
End Sub

'********************
'Private Procedures
'********************

Private Sub Class_Initialize()
    CountInitialize
End Sub
    
Private Sub Class_Terminate()
    CountTerminate
End Sub

