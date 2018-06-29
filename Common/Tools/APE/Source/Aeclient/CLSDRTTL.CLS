VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDirectTestTool"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class provides a RunTest method to be called to run a Direct
'Instanciation model test.
'-------------------------------------------------------------------------

Public Sub RunTest()
    '-------------------------------------------------------------------------
    'Purpose:   Executes a loop for glNumberOfCalls each time calling
    '           AEWorker.Worker.DoActivity.  This method actually runs
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
    Dim bLog As Boolean             'If true log records
    Dim bShow As Boolean            'If true update display
    
    On Error GoTo RunTestError
    'If there is reentry by a timer click exit sub
    If gbRunning Then Exit Sub
    gbRunning = True
    
    'Set the local variables to direct the testing
    Set oWorker = CreateObject("AEWorker.Worker")
    'Pass configuration settings to the Worker
    With oWorker
        .SetProperties gbLogWorker, gbEarlyBindServices, gbPersistentServices, glClientID   'The Worker ID is the same as the Clients' ID in direct case
        If gbPreloadServices Then
            .LoadServiceObject IIf(gbUseDefaultService, gsSERVICE_LIB_CLASS, gsServiceCommand), gvServiceConfiguration
        End If
    End With
    
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
KeepPostingServices:
    Do While Not gbStopping
        'Check if new data needs generated because of randomization
        If bRandomSendData Then bRandomSendData = GetTestData(bSendSomething, bReceiveSomething, vSendData)
        If bRandomWait Then lCallWait = GetValueFromRange(gudtWaitPeriod, bRandomWait)
        If bRandomCommand Then sSendCommand = GetServiceCommand(bRandomCommand)
        
        'Increment number of calls made
        lCallNumber = glCallsMade + 1
        'Post the service to a worker
        'Post a synchronous service
        sServiceID = glClientID & "." & lCallNumber
        iRetry = 0
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
        Exit Sub
    End If
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    CompleteTest
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
            Set oWorker = oInstancer.object("AEWorker.Worker")
            Set oInstancer = Nothing
            Resume Next
        Case RPC_S_UNKNOWN_AUTHN_TYPE
            Dim iResult As Integer
            'Tried to connect to a server that does not support
            'specified authentication level.  Display message and
            'switch to no authentication and try again
            s = LoadResString(giUSING_NO_AUTHENTICATION)
            DisplayStatus s
            AddLogRecord gsNULL_SERVICE_ID, s, 0, False
            glConnectionAuthentication = RPC_C_AUTHN_LEVEL_NONE
            iResult = goRegClass.SetAutoServerSettings(True, "AEWorker.Worker", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
            Resume
        Case ERR_OVER_FLOW
            s = CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            lCallNumber = 0
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        Case giRPC_ERROR_ACCESSING_COLLECTION
            Set oWorker = Nothing
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
                Exit Sub
            Else
                Resume Next
            End If
    End Select
End Sub
