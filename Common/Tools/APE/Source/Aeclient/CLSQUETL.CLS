VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQueueTestTool"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class provides a RunTest method to be called to run a Queue Manager
'model test
'-------------------------------------------------------------------------
Private WithEvents moEventReturn As AEExpediter.EventReturn  'Expediter may raise an event
Attribute moEventReturn.VB_VarHelpID = -1
                                                             'to return results

Public Sub RunTest()
    '-------------------------------------------------------------------------
    'Purpose:   Executes a loop for glNumberOfCalls each time calling
    '           AEQueueMgr.Queue.Add.  This method actually runs
    '           a test according to set properties
    'Assumes:   All Client properties have been set.
    'Effects:
    '           Calls CompleteTest when finished calling QueueMgr if no
    '           callbacks are expected
    '           Calls AddServiceRecord procedure after each call to QueueMgr
    '           if callbacks are expected
    '   [gbRunning]
    '           Is true during procedure
    '   [glFirstServiceTick]
    '           becomes the tick count of when the test is started
    '   [glLastCallbackTick]
    '           becomes the tick count of when the last call is made
    '   [glCallsMade]
    '           is incremented every time the QueueMgr is called
    '   [glCallsReturned]
    '           is incremented every time the QueueMgr is called if no
    '           callback is expected
    '-------------------------------------------------------------------------
    Const lMAX_COUNT = 2147483647
    Dim s As String         'Error message to log and display
    Dim sServiceID As String  'Service Request ID
    Dim lTicks As Long      'Tick Count in milliseconds
    Dim lEndTick As Long    'DoEvents loop until this tick count
    Dim lCallNumber As Long 'Number of calls
    Dim lNumberOfCalls As Long      'Test duration in number of calls
    Dim iDurationMode As Integer    'Test duration mode
    Dim lDurationTicksEnd As Long   'Tick that test should end on
    Dim iRetry As Integer   'Number of call retries made because call rejection
    Dim bPostingServices As Boolean 'If true, in main loop of procedure
    Dim vSendData As Variant        'Data to send with Service Request
    Dim bRandomSendData As Boolean  'If true vSendData needs generated before each new request
    Dim sSendCommand As String      'Command string to be sent with Service Request
    Dim bRandomCommand As Boolean   'If true sSendCommand needs generated before each new request
    Dim lCallWait As Long           'Number of ticks to wait between calls
    Dim bRandomWait As Boolean      'If true lCallWait needs generated before each new request
    Dim bSendSomething As Boolean    'If true data needs passed with request
    Dim bReceiveSomething As Boolean 'If true something is expeted back
    Dim oCallback As clsCallback    'Callback object to pass with requests
    Dim bLog As Boolean             'If true log records
    Dim bShow As Boolean            'If true update display
    Dim iCallbackMode As Integer    'Determines if and how results are returned from QueueMgr
    Dim oQueue As APEInterfaces.IQueue  'Queue object to post service requests to

    On Error GoTo RunTestError
    
    'If there is reentry by a timer click exit sub
    If gbRunning Then Exit Sub
    gbRunning = True
    
    'Set the local variables to direct the testing
    Set oQueue = CreateObject("AEQueueMgr.Queue")
    Set oCallback = New clsCallback
    bRandomSendData = GetTestData(bSendSomething, bReceiveSomething, vSendData)
    lCallWait = GetValueFromRange(gudtWaitPeriod, bRandomWait)
    sSendCommand = GetServiceCommand(bRandomCommand)
    bLog = gbLog
    bShow = gbShow
    iCallbackMode = glCallbackMode
    
    'Set the DefaultCallback property if it will be needed
    'Setting the default callback even when the client will be passing
    'a callback every call improves performance by keeping RemAuto and DCOM
    'form tearing down the stub and proxy for the callback object
    'when the expediter's reference count of the callback object is zero
    'Having one reference always on the server side keeps the stub and proxy
    'from being torn done, which removes the need for the stub and proxy to have
    'to be continually recreated during the test
    If iCallbackMode = giUSE_DEFAULT_CALLBACK Or giUSE_PASSED_CALLBACK Then Set oQueue.DefaultCallBack = oCallback
    'Set the withevents object if it will be needed
    If iCallbackMode = giRETURN_BY_SYNC_EVENT Then Set moEventReturn = oQueue.GetEventObject
    
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
        'Queue the Service
        'Post this Service to the queue
        'Queue an asynchronous Service
        sServiceID = glClientID & "." & lCallNumber
        iRetry = 0
        lTicks = GetTickCount
        
        'Display CallsMade
        If bShow Then
            With frmClient.lblCallsMade
                .Caption = lCallNumber
                .Refresh
            End With
        End If
        
        If bReceiveSomething Then
            Dim bProcessed As Boolean
            'We are expecting a callback.
            Select Case iCallbackMode
                Case giUSE_DEFAULT_CALLBACK, giRETURN_BY_SYNC_EVENT
                    bProcessed = oQueue.Add(sSendCommand, sServiceID, iCallbackMode, vSendData)
                Case giUSE_PASSED_CALLBACK
                    bProcessed = oQueue.Add(sSendCommand, sServiceID, iCallbackMode, vSendData, oCallback)
            End Select
            'If not bProcessed then QueueMgr did not process Service request
            'because it was stopped.
            If Not bProcessed Then Exit Do
            AddServiceRecord sServiceID, sSendCommand, GetTickCount()
        ElseIf bSendSomething Then
            'Sending data but nothing comming back.
            'Dont receive a callback.
            oQueue.Add sSendCommand, sServiceID, giNO_CALLBACK, vSendData
            glLastCallbackTick = GetTickCount
            'Increment the CallsReturned global
            glCallsReturned = glCallsReturned + 1
            If bShow Then
                With frmClient.lblCallsReturned
                    .Caption = glCallsReturned
                    .Refresh
                End With
            End If
        Else
            'Just make the call, nothing else.
            oQueue.Add sSendCommand, sServiceID, giNO_CALLBACK
            glLastCallbackTick = GetTickCount
            'Increment the CallsReturned global
            glCallsReturned = glCallsReturned + 1
            If bShow Then
                With frmClient.lblCallsReturned
                    .Caption = glCallsReturned
                    .Refresh
                End With
            End If
        End If
        If bLog Then AddLogRecord sServiceID, LoadResString(giQUEUE_SERVICE) & gsSEPERATOR & sSendCommand, lTicks, False
        
        'If gbStopping Then Exit Do
        'Go into an idle loop util the next call.
        'Also go into idle loop if difference between
        'calls sent and calls received is greater than giCALL_SENT_AND_RECEIVED_MAX_DIFFERENCE
        If lCallWait > 0 Or (lCallNumber - glCallsReturned) > giCALL_SENT_AND_RECEIVED_MAX_DIFFERENCE Then
            lEndTick = GetTickCount + lCallWait
            Do While ((GetTickCount() < lEndTick) Or ((lCallNumber - glCallsReturned) > giCALL_SENT_AND_RECEIVED_MAX_DIFFERENCE)) And Not gbStopping
                DoEvents
            Loop
        End If
        glCallsMade = lCallNumber
        
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
    If gbStopping Then
        'Someone hit the stop button on the Explorer.
        gStopTest
        Exit Sub
    End If
    If bLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    If Not bReceiveSomething Or glCallsReturned = glCallsMade Then
        'Not expecting callbacks. The test is done.
        CompleteTest
    End If
    Set oCallback = Nothing
    Set oQueue = Nothing
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
        Case giQUEUE_MGR_IS_BUSY + vbObjectError
            lEndTick = GetTickCount + lCallWait + giQUEUE_WAIT_RETRY_MIN
            AddLogRecord sServiceID, Err.Description, GetTickCount, False
            Do While GetTickCount() < lEndTick And Not gbStopping
                DoEvents
            Loop
            Resume
        Case ERR_OBJECT_VARIABLE_NOT_SET
            'QueueMgr was not successfully created
            'stop client
            'If gbStopping is true the error occurred
            'because StopOnError was already called when
            'handling a callback
            If Not gbStopping Then
                s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
                DisplayStatus Err.Description
                AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
                StopOnError s
            End If
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
            Set oQueue = oInstancer.object("AEQueueMgr.Queue")
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
            iResult = goRegClass.SetAutoServerSettings(True, "AEQueueMgr.Queue", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
            Resume
        Case ERR_OVER_FLOW
            s = CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            If lCallNumber = glMAX_LONG Then lCallNumber = 0
            If glCallsReturned = glMAX_LONG Then glCallsReturned = 0
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        Case RPC_PROTOCOL_SEQUENCE_NOT_FOUND
            'Most probably because of an attempt to create a Named Pipe under Win95
            If frmClient.SysInfo.OSPlatform = 1 And gbConnectionNetOLE = False And gbConnectionRemote = True _
                    And gsConnectionProtocol = "ncacn_np" Then
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

Private Sub moEventReturn_ServiceResult(ByVal sServiceID As String, ByVal vServiceReturn As Variant, ByVal sServiceError As String)
    '-------------------------------------------------------------------------
    'Purpose:   Event raised by Expediter class object to return results
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
