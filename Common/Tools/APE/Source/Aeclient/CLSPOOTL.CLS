VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPoolTestTool"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class provides a RunTest method to be called to run a Pool
'Management model test.
'-------------------------------------------------------------------------

Public Sub RunTest()
    '-------------------------------------------------------------------------
    'Purpose:   Executes a loop for glNumberOfCalls each time calling
    '           AEWorker.Worker.DoActivity.  Before each call a Worker
    '           is Requested from AEPoolMgr.Pool after each call the
    '           Worker is released and PoolMgr is called again to
    '           notify of release.  This method actually runs
    '           a test according to set properties
    'Assumes:   All Client properties have been set.
    'Effects:
    '           Calls CompleteTest when finished calling Worker
    '   [gbRunning]
    '           Is true during procedure
    '   [glFirstServiceTick]
    '           becomes the tick count of when the test is started
    '   [glLastCallbackTick]
    '           becomes the tick count of when the last call is made
    '   [glCallsMade]
    '           is incremented every time the Worker is called
    ' Exceptions:
    '   If only an MTS transaction is being performed, the MTS, not APE's
    '   Pool Manager, provides the pool management services.
    '-------------------------------------------------------------------------
    
    'Called by tmrStartTest so that the StartTest method can release
    'the calling program.
    
    Const lMAX_COUNT = 2147483647
    Dim s As String         'Error message
    Dim sServiceID As String  'Service Request ID
    Dim lTicks As Long      'Tick Count
    Dim lEndTick As Long    'DoEvents loop until this Tick Count
    Dim lCallNumber As Long 'Number of calls to Worker
    Dim lNumberOfCalls As Long      'Test duration in number of calls
    Dim iDurationMode As Integer    'Test duration mode
    Dim lDurationTicksEnd As Long   'Tick that test should end on
    Dim bPostingServices As Boolean 'In main loop of procedure
    Dim iRetry As Integer           'Number of call reties made by error handling resume
    Dim vSendData As Variant        'Data to send with Service request
    Dim bRandomSendData As Boolean  'If true vSendData needs generated before each new request
    Dim sSendCommand As String      'Command string to be sent with Service Request
    Dim bRandomCommand As Boolean   'If true sSendCommand needs generated before each new request
    Dim lCallWait As Long           'Number of ticks to wait between calls
    Dim bRandomWait As Boolean      'If true lCallWait needs generated before each new request
    Dim bSendSomething As Boolean    'If true data needs passed with request
    Dim bReceiveSomething As Boolean 'If true data is expected back from request
    Dim oWorker As APEInterfaces.IWorker  'Local reference to the Worker
    Dim oPool As APEInterfaces.IPool
    Dim bLog As Boolean             'If true log records
    Dim bShow As Boolean            'If true update display
    Dim iPoolWaitRetryCount As Integer  'Number of times retry is need for each call loop
    Dim bReleaseWorker As Boolean       ' If True, the worker needs to be released before leaving the procedure
    
    bReleaseWorker = False
    
    On Error GoTo RunTestError
    'If there is reentry by a timer click exit sub
    If gbRunning Then Exit Sub
    gbRunning = True
    
    ' If only an MTS transaction is being performed, use MTS as pool manager
    If (giServiceTask = (giMASK_USE_DB_TASK Or giMASK_WRITE_MTS_TRANSACTION)) Then
        RunMTSTest
        Exit Sub
    End If
    
    'Set the local variables to direct the testing
    Set oPool = CreateObject("AEPoolMgr.Pool")
    
    bRandomSendData = GetTestData(bSendSomething, bReceiveSomething, vSendData)
    lCallWait = GetValueFromRange(gudtWaitPeriod, bRandomWait)
    sSendCommand = GetServiceCommand(bRandomCommand)
    bLog = gbLog
    bShow = gbShow
    
    s = LoadResString(giTEST_STARTED)
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus s
    glFirstServiceTick = GetTickCount()
    glLastCallbackTick = glFirstServiceTick     ' If 0 calls are completed, the time spent will be 0 ticks
    
    'Test duration variables
    iDurationMode = giTestDurationMode
    If iDurationMode = giTEST_DURATION_CALLS Then
        lNumberOfCalls = glNumberOfCalls
    ElseIf iDurationMode = giTEST_DURATION_TICKS Then
        lDurationTicksEnd = glFirstServiceTick + glTestDurationInTicks
    End If
    
    bPostingServices = True
    Do While Not gbStopping
        'Check if new data needs generated because of randomization
        If bRandomSendData Then bRandomSendData = GetTestData(bSendSomething, bReceiveSomething, vSendData)
        If bRandomWait Then lCallWait = GetValueFromRange(gudtWaitPeriod, bRandomWait)
        If bRandomCommand Then sSendCommand = GetServiceCommand(bRandomCommand)
        
        'Increment number of calls made
        lCallNumber = glCallsMade + 1
        'Get a Worker from the PoolMgr
        'Post the service to a worker
        'Post a synchronous service
        sServiceID = glClientID & "." & lCallNumber
        iRetry = 0
        iPoolWaitRetryCount = 0
RunTest_GetWorkerRetry:
        Set oWorker = oPool.GetWorker
        'Pool Manager may reject request for worker
        'If it does wait sometime and retry
        If oWorker Is Nothing Then GoTo RunTest_WaitForPool
        bReleaseWorker = True
        iRetry = 0
        iPoolWaitRetryCount = 0
        'Display CallsMade
        If bShow Then
            With frmClient
                .lblCallsMade = lCallNumber
                .lblCallsMade.Refresh
            End With
        End If
        If bSendSomething Then
            oWorker.DoService sServiceID, sSendCommand, vSendData
        Else
            oWorker.DoService sServiceID, sSendCommand
        End If
        glLastCallbackTick = GetTickCount
        Set oWorker = Nothing
        oPool.ReleaseWorker
        bReleaseWorker = False
        
        'Display CallsReturned
        If bShow Then
            With frmClient
                .lblCallsReturned = lCallNumber
                .lblCallsReturned.Refresh
            End With
        End If
        'If gbStopping Then Exit Do
        'Go into an idle loop util the next call.
        If lCallWait > 0 Then
            lEndTick = GetTickCount + lCallWait
            Do While GetTickCount() < lEndTick And Not gbStopping
                DoEvents
            Loop
        End If
        glCallsMade = lCallNumber
        glCallsReturned = lCallNumber
        
        'See if it is time to stop the test
        If iDurationMode = giTEST_DURATION_CALLS Then
            If lCallNumber >= lNumberOfCalls Then Exit Do
        ElseIf iDurationMode = giTEST_DURATION_TICKS Then
            If GetTickCount >= lDurationTicksEnd Then Exit Do
        End If
    Loop
StopTestNow:
    bPostingServices = False
    gbRunning = False
    Set oWorker = Nothing
    If gbStopping Then
        'Someone hit the stop button on the Explorer.
        gStopTest
        GoTo CleanupAndExit
    End If
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    CompleteTest
    GoTo CleanupAndExit
RunTest_WaitForPool:
    If iPoolWaitRetryCount <= giMAX_ALLOWED_RETRIES Then
        iPoolWaitRetryCount = iPoolWaitRetryCount + 1
        lEndTick = GetTickCount + lCallWait + giPOOL_WAIT_RETRY_MIN
        Do While GetTickCount() < lEndTick And Not gbStopping
            DoEvents
        Loop
        GoTo RunTest_GetWorkerRetry
    Else
        'We reached our max retries
        s = LoadResString(giPOOL_MGR_REJECTION_WAITS_EXHAUSTED)
        If bLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        DisplayStatus s
        StopOnError s
        Exit Sub
    End If
    Exit Sub
RunTestError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim il As Integer
            Dim ir As Integer
            'First check if stopping test
            If gbStopping Then GoTo StopTestNow
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(giQUEUE_SERVICE_COLLISION_RETRY), GetTickCount(), False
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                If gbStopping Then Resume Next Else Resume
            Else
                'We reached our max retries
                s = LoadResString(giCOLLISION_ERROR)
                AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
                DisplayStatus s
                StopOnError s
                GoTo CleanupAndExit
            End If
        Case ERR_OBJECT_VARIABLE_NOT_SET
            'Worker was not successfully created
            s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            StopOnError s
            Exit Sub
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
            Set oWorker = oInstancer.object("AEWorker.Worker")
            Set oInstancer = Nothing
            Resume Next
        Case RPC_S_UNKNOWN_AUTHN_TYPE
            'Tried to connect to a server that does not support
            'specified authentication level.  Display message and
            'switch to no authentication and try again
            Dim iResult As Integer
            s = LoadResString(giUSING_NO_AUTHENTICATION)
            DisplayStatus s
            AddLogRecord gsNULL_SERVICE_ID, s, 0, False
            glConnectionAuthentication = RPC_C_AUTHN_LEVEL_NONE
            iResult = goRegClass.SetAutoServerSettings(True, "AEPoolMgr.Pool", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
            Resume
        Case ERR_OVER_FLOW
            s = CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            lCallNumber = 0
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        Case giRPC_ERROR_ACCESSING_COLLECTION
            Set oWorker = Nothing
            oPool.ReleaseWorker
            bReleaseWorker = False
            s = LoadResString(giRPC_ERROR_ACCESSING_COLLECTION)
            DisplayStatus s
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            StopOnError s
            Exit Sub
        Case RPC_PROTOCOL_SEQUENCE_NOT_FOUND
            'Most probably because of an attempt to create a Named Pipe under Win95
            If frmClient.SysInfo.OSPlatform = 1 And gbConnectionNetOLE = False And gbConnectionRemote = True _
                    And gsConnectionProtocol = "ncacn_np" Then
                Set oWorker = Nothing
                oPool.ReleaseWorker
                bReleaseWorker = False
                s = LoadResString(giNO_NAMED_PIPES_UNDER_WIN95)
                AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
                DisplayStatus s
                StopOnError s
                Exit Sub
            End If
        Case Else
            s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            If bPostingServices Then
                StopOnError s
                GoTo CleanupAndExit
            Else
                Resume Next
            End If
    End Select

CleanupAndExit:
    On Error Resume Next
    If bReleaseWorker And Not oPool Is Nothing Then
        oPool.ReleaseWorker
    End If
End Sub

Public Sub RunMTSTest()
    ' Similar to RunTest, but specifically for using MTS, not APE's Pool Manager, for pooling objects.
    ' This occurs when an MTS transaction (and no CPU task) is being performed.
    Const lMAX_COUNT = 2147483647
    
    Const miMinQueryRetryDelay As Integer = 20      ' Min delay (ms) between retries of a query that failed due to a locking contention
    Const miMaxQueryRetryDelay As Integer = 100      ' Max delay (ms) between retries of a query that failed due to a locking contention
    
    Dim s As String         'Error message
    Dim lTicks As Long      'Tick Count
    Dim lEndTick As Long    'DoEvents loop until this Tick Count
    Dim lCallNumber As Long 'Number of calls to Worker
    Dim lNumberOfCalls As Long      'Test duration in number of calls
    Dim iDurationMode As Integer    'Test duration mode
    Dim lDurationTicksEnd As Long   'Tick that test should end on
    Dim bPostingServices As Boolean 'In main loop of procedure
    Dim iRetry As Integer           'Number of call reties made by error handling resume
    Dim lCallWait As Long           'Number of ticks to wait between calls
    Dim bRandomWait As Boolean      'If true lCallWait needs generated before each new request
    Dim bLog As Boolean             'If true log records
    Dim bShow As Boolean            'If true update display
    Dim oMoveMoney As APEInterfaces.IMTSMoveMoney
    Dim sConnect As String          ' Connect string
    Dim eConnectOptions As ape_DbConnectionOptions     ' Database connection option
    Dim bLogMTSTransactions  As Boolean     ' If True, log MTS events
    Dim bShowMTSTransactions  As Boolean     ' If True, show MTS events
    Dim iTransferRetries As Integer         ' Number of attempts at performing the transfer
    
    On Error GoTo RunMTSTestError
    bLog = gbLog
    bShow = gbShow
    
    ' Set up connect string and database connection options
    sConnect = gvServiceConfiguration(ape_conConnectionString)
    eConnectOptions = gvServiceConfiguration(ape_conConnectionOption)
    bLogMTSTransactions = gvServiceConfiguration(ape_conLogMTSTransactions)
    bShowMTSTransactions = gvServiceConfiguration(ape_conShowMTSTransactions)
    
    s = LoadResString(giTEST_STARTED)
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus s
    Randomize
    glFirstServiceTick = GetTickCount()
    glLastCallbackTick = glFirstServiceTick     ' If 0 calls are completed, the time spent will be 0 ticks
    
    'Test duration variables
    iDurationMode = giTestDurationMode
    If iDurationMode = giTEST_DURATION_CALLS Then
        lNumberOfCalls = glNumberOfCalls
    ElseIf iDurationMode = giTEST_DURATION_TICKS Then
        lDurationTicksEnd = glFirstServiceTick + glTestDurationInTicks
    End If
    
    bPostingServices = True
    Do While Not gbStopping
        'Increment number of calls made
        lCallNumber = glCallsMade + 1
        'Get a Worker from the PoolMgr
        'Post the service to a worker
        'Post a synchronous service
        iRetry = 0
        
        ' Create the appropriate MoveMoney object
        Set oMoveMoney = CreateObject("AEMTSSvc.MoveMoney")
        
        iRetry = 0
        If bLogMTSTransactions Then
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(giBEGIN_MTS_TRANSACTION), GetTickCount(), False
        End If
        On Error Resume Next
        Const iMAX_ACCOUNT_NO = 1000        ' Highest account number (1 is presumed to be the lowest)
        Dim lFromAccount As Long, lToAccount As Long
        lFromAccount = 1 + Int(iMAX_ACCOUNT_NO * Rnd)
        lToAccount = 1 + Int(iMAX_ACCOUNT_NO * Rnd)
        iTransferRetries = 0
        
        Dim bRetry As Boolean
        Do
            bRetry = False
            oMoveMoney.Transfer sConnect, eConnectOptions, lFromAccount, lToAccount, 1
            glLastCallbackTick = GetTickCount
            Dim lError As Long
            lError = Err.Number
            On Error GoTo RunMTSTestError
            ' If the error is due to a locking contention, try again
            If lError <> 0 And iTransferRetries < giMAX_ALLOWED_RETRIES Then
                Select Case eConnectOptions
                    Case ape_idcADO
                        bRetry = (lError = -2147467259)
                    Case ape_idcDAO
                        If lError = 3146 Then
                            ' First make sure the cause really is a locking contention
                            Dim DAOErr As DAO.Error
                            For Each DAOErr In DBEngine.Errors
                                If DAOErr.Number = 1205 Then    ' 1205 = SQL Server record locking contention
                                    bRetry = True
                                End If
                            Next
                        End If
                    Case ape_idcRDO
                        If lError = 40002 Then
                            ' First make sure the cause really is a locking contention
                            Dim RDOErr As RDO.rdoError
                            For Each RDOErr In rdoEngine.rdoErrors
                                If RDOErr.Number = 1205 Then    ' 1205 = SQL Server record locking contention
                                    bRetry = True
                                End If
                            Next
                        End If
                    Case ape_idcODBC
                        bRetry = (lError = ErrorResourceDeadlock)
                End Select
                If bRetry Then
                    iTransferRetries = iTransferRetries + 1
                    Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
                    DoEvents
                End If
            End If
        Loop While bRetry
        If bLogMTSTransactions Then
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(IIf(lError = 0, giEND_MTS_TRANSACTION_SUCCEEDED, _
                    giEND_MTS_TRANSACTION_FAILED)), GetTickCount(), False
        End If
        If bShowMTSTransactions Then
            frmService.MTSResults (lError = 0)
        End If
        Set oMoveMoney = Nothing
        
        'Display CallsMade
        If bShow Then
            With frmClient
                .lblCallsMade = lCallNumber
                .lblCallsReturned = lCallNumber
                .lblCallsMade.Refresh
                .lblCallsReturned.Refresh
            End With
        End If
        'If gbStopping Then Exit Do
        'Go into an idle loop util the next call.
        If lCallWait > 0 Then
            lEndTick = GetTickCount + lCallWait
            Do While GetTickCount() < lEndTick And Not gbStopping
                DoEvents
            Loop
        End If
        glCallsMade = lCallNumber
        glCallsReturned = lCallNumber
        
        'See if it is time to stop the test
        If iDurationMode = giTEST_DURATION_CALLS Then
            If lCallNumber >= lNumberOfCalls Then Exit Do
        ElseIf iDurationMode = giTEST_DURATION_TICKS Then
            If GetTickCount >= lDurationTicksEnd Then Exit Do
        End If
    Loop
StopMTSTestNow:
    bPostingServices = False
    gbRunning = False
    Set oMoveMoney = Nothing
    If gbStopping Then
        'Someone hit the stop button on the Explorer.
        gStopTest
        Exit Sub
    End If
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    CompleteTest
    Exit Sub
RunMTSTestError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim il As Integer
            Dim ir As Integer
            'First check if stopping test
            If gbStopping Then GoTo StopMTSTestNow
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(giQUEUE_SERVICE_COLLISION_RETRY), GetTickCount(), False
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                If gbStopping Then Resume Next Else Resume
            Else
                'We reached our max retries
                s = LoadResString(giCOLLISION_ERROR)
                AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
                DisplayStatus s
                StopOnError s
                Exit Sub
            End If
        Case ERR_OBJECT_VARIABLE_NOT_SET
            'Worker was not successfully created
            s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            StopOnError s
            Exit Sub
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
            Set oMoveMoney = oInstancer.object("AEMTSService.MoveMoney")
            Set oInstancer = Nothing
            Resume Next
        Case ERR_OVER_FLOW
            s = CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            lCallNumber = 0
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        Case ERR_CANT_CREATE_OBJECT    ' CreateObject failed
            s = LoadResString(giERROR_CREATE_MTS_OBJECT)
            DisplayStatus s
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            StopOnError s
            Exit Sub
        Case Else
            s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            If bPostingServices Then
                StopOnError s
                Exit Sub
            Else
                Resume Next
            End If
    End Select

End Sub


    

