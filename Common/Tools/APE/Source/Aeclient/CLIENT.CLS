VERSION 1.0 CLASS
BEGIN
  MultiUse = 0   'False
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Client"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Client"
Option Explicit

Implements APEInterfaces.IClient

'Private class level variables
Private mbFirstClientOnMachine As Boolean   'If true, this is the first Client application
                                            'started on this machine
'*****************
'Public Properties
'*****************

Public Property Set IClient_Explorer(ByVal oExplorer As APEInterfaces.IManagerCallback)
Attribute IClient_Explorer.VB_Description = "Set the Manager object that the Client will use to notify test completion."
    '-------------------------------------------------------------------------
    'Purpose:   To give the client a reference to AEManager.Explorer
    'IN:
    '   [oExplorer]
    '           must be valid reference to a AEManager.Explorer class object
    'Effects:
    '   [goExplorer]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    Set goExplorer = oExplorer
End Property

Public Property Get IClient_MachineName() As String
Attribute IClient_MachineName.VB_Description = "Returns the computer name that the Client is instanciated on."
    'Get the local computer name
    Dim l As Long
    Dim s As String
    s = Space$(255)
    l = GetComputerName(s, 255)
    l = InStr(s, vbNullChar)
    s = Left$(s, l - 1)
    IClient_MachineName = s
End Property

Public Property Let IClient_ConnectionAddress(ByVal sAddress As String)
Attribute IClient_ConnectionAddress.VB_Description = "Set the network address for the location of the APE server."
    '-------------------------------------------------------------------------
    'Purpose:   The netaddress used for remote connections
    'Effects:
    '   [gsConnectionAddress]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gsConnectionAddress = sAddress
End Property

Public Property Get IClient_ConnectionAddress() As String
    IClient_ConnectionAddress = gsConnectionAddress
End Property

Public Property Let IClient_ConnectionProtocol(ByVal sProtocol As String)
Attribute IClient_ConnectionProtocol.VB_Description = "Sets the protocol to be used for Remote Automation connections."
    '-------------------------------------------------------------------------
    'Purpose:   The RPC protocol to use for all remote connections.
    'Effects:
    '   [gsConnectionProtocol]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gsConnectionProtocol = sProtocol
End Property

Public Property Get IClient_ConnectionProtocol() As String
    IClient_ConnectionProtocol = gsConnectionProtocol
End Property

Public Property Let IClient_ConnectionAuthentication(ByVal lAuthentication As Long)
Attribute IClient_ConnectionAuthentication.VB_Description = "Sets the authentication level to be used for Remote Automation connections."
    '-------------------------------------------------------------------------
    'Purpose:   The RPC authenticaion to enforce for all remote connections.
    'Effects:
    '   [gsConnectionAuthentication]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    glConnectionAuthentication = lAuthentication
End Property

Public Property Get IClient_ConnectionAuthentication() As Long
    IClient_ConnectionAuthentication = glConnectionAuthentication
End Property

Public Property Let IClient_ConnectionRemote(ByVal bRemote As Boolean)
Attribute IClient_ConnectionRemote.VB_Description = "Determines if the Client will connect to a remote APE server or to a local APE server."
    '-------------------------------------------------------------------------
    'Purpose:   If true server is remote and ConnectionAddress, ConnectionProtocol,
    '           ConnectionNetOLE, and ConnectionAuthentication apply
    'Effects:
    '   [gsConnectionRemote]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbConnectionRemote = bRemote
End Property

Public Property Get IClient_ConnectionRemote() As Boolean
    IClient_ConnectionRemote = gbConnectionRemote
End Property

Public Property Let IClient_ConnectionNetOLE(ByVal bNetOLE As Boolean)
Attribute IClient_ConnectionNetOLE.VB_Description = "Determines if the Client will use DCOM to connect to the APE server."
    '-------------------------------------------------------------------------
    'Purpose:   If true use NetOLE (DCOM) for remote connection, instead of
    '           Remote Automation
    'Effects:
    '   [gsConnectionNetOLE]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbConnectionNetOLE = bNetOLE
End Property

Public Property Get IClient_ConnectionNetOLE() As Boolean
    IClient_ConnectionNetOLE = gbConnectionNetOLE
End Property

Public Property Let IClient_ID(ByVal lID As Long)
Attribute IClient_ID.VB_Description = "Sets and returns the Client ID for Client management."
    '-------------------------------------------------------------------------
    'Purpose:   Unique ID for the client in this test.  ID is used to seperate
    '           Clients log records and differentiate title bars
    'Effects:
    '   [glClientID]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    glClientID = lID
End Property

Public Property Get IClient_ID() As Long
    IClient_ID = glClientID
End Property

Public Property Let IClient_Model(ByVal lModel As Long)
Attribute IClient_Model.VB_Description = "Determines what test model the Client will perform."
    '-------------------------------------------------------------------------
    'Purpose:   'What model to use for this test.
    '               0 or giMODEL_QUEUE - Queue Management
    '               2 or gimodel_direct - Direct Instanciation
    'Effects:
    '   [glModel]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    glModel = lModel
End Property

Public Property Get IClient_Model() As Long
    IClient_Model = glModel
End Property

Public Property Let IClient_Show(ByVal bShow As Boolean)
Attribute IClient_Show.VB_Description = "Determines if the Client will show a form."
    '-------------------------------------------------------------------------
    'Purpose:   If true, show the Client's U/I
    'Effects:
    '   [gbShow]
    '           Set equal to parameter
    '   [frmClient.Visible]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    frmClient.Visible = bShow
    gbShow = bShow
    If bShow Then
        'Update values on U/I
        With frmClient
            .lblCallsMade.Caption = 0
            .lblCallsReturned.Caption = 0
            .lblCallsMade.Refresh
            .lblCallsReturned.Refresh
        End With
    End If
End Property

Public Property Get IClient_Show() As Boolean
    IClient_Show = gbShow
End Property

Public Property Let IClient_Log(ByVal bLog As Boolean)
Attribute IClient_Log.VB_Description = "Determines if the Client logs its events and errors."
    '-------------------------------------------------------------------------
    'Purpose:   If true, log events in the Client
    'Effects:
    '   [gbLog]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbLog = bLog
End Property

Public Property Get IClient_Log() As Boolean
    IClient_Log = gbLog
End Property

Public Property Let IClient_CallbackMode(ByVal lCallbackMode As APECallbackNotificationConstants)
Attribute IClient_CallbackMode.VB_Description = "Determines what Callback mode that will be used."
    '-------------------------------------------------------------------------
    'Purpose:   Determines if and how client receives results from
    '           services requested from QueueManager
    '           see "Callback mode keys" in modAEConstants
    'Effects:
    '   [glCallbackMode]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    Select Case lCallbackMode
        Case giUSE_DEFAULT_CALLBACK, giUSE_PASSED_CALLBACK, giRETURN_BY_SYNC_EVENT
            glCallbackMode = lCallbackMode
        Case Else
            'Default callback mode
            glCallbackMode = giUSE_PASSED_CALLBACK
    End Select
End Property

Public Property Get IClient_CallbackMode() As APECallbackNotificationConstants
    IClient_CallbackMode = glCallbackMode
End Property

'How many Kb should the log collection be allowed to take
'before it is cached to a temporary file?
'If zero, the log is not cached to a file.
Public Property Let IClient_LogThreshold(ByVal lKB As Long)
Attribute IClient_LogThreshold.VB_Description = "Sets the log threshold in kilobytes that determines when log records are written to a file and purged from memory."
    '-------------------------------------------------------------------------
    'Purpose:   Client uses the LogThreshold property to determine how many
    '           kilobytes should be held in memory before writing to a file
    '           and emptying log record array.
    'Effects:   [glLogThreshold]
    '               Becomes equal to the passed parameter
    '           [glLogThresholdRecs]
    '               Becomes an estimated number of records equivalent
    '-------------------------------------------------------------------------
    On Error Resume Next
    glLogThreshold = lKB
    glLogThresholdRecs = lKB * giLOG_RECORD_KILOBYTES
End Property

Public Property Get IClient_LogThreshold() As Long
    IClient_LogThreshold = glLogThreshold
End Property

Public Property Let IClient_PreLoadServices(ByVal bPreLoad As Boolean)
Attribute IClient_PreLoadServices.VB_Description = "Determines if LoadServiceObject will be called on a directly instantiated AEWorker.Worker object before beginning the test."
    '-------------------------------------------------------------------------
    'Purpose:   If true, call the Worker's PreLoadService method before
    '           starting test
    'Effects:
    '   [gbPreloadServices]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbPreloadServices = bPreLoad
End Property

Public Property Get IClient_PreLoadServices() As Boolean
    IClient_PreLoadServices = gbPreloadServices
End Property

Public Property Let IClient_PersistentServices(ByVal bPersistent As Boolean)
Attribute IClient_PersistentServices.VB_Description = "Sets the value that is used to set the PersistentServices property of a directly instantiated AEWorker.Worker object."
    '-------------------------------------------------------------------------
    'Purpose:   Sets the Worker's PersistentServices property
    'Effects:
    '   [gbPersistentServices]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbPersistentServices = bPersistent
End Property

Public Property Get IClient_PersistentServices() As Boolean
    IClient_PersistentServices = gbPersistentServices
End Property

Public Property Let IClient_LogWorker(ByVal bLog As Boolean)
Attribute IClient_LogWorker.VB_Description = "Sets the value that is used to set the Log property of a directly instantiated AEWorker.Worker object."
    '-------------------------------------------------------------------------
    'Purpose:   Sets the Worker's Log property
    'Effects:
    '   [gbLogWorker]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbLogWorker = bLog
End Property

Public Property Get IClient_LogWorker() As Boolean
    IClient_Log = gbLogWorker
End Property

Public Property Let IClient_EarlyBindServices(ByVal bEarlyBind As Boolean)
Attribute IClient_EarlyBindServices.VB_Description = "Sets the value that is used to set the EarlyBindServices property of a directly instantiated AEWorker.Worker object."
    '-------------------------------------------------------------------------
    'Purpose:   Sets the Worker's EarlyBindServices property
    'Effects:
    '   [gbEarlyBindServices]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbEarlyBindServices = bEarlyBind
End Property

Public Property Get IClient_EarlyBindServices() As Boolean
    IClient_EarlyBindServices = gbEarlyBindServices
End Property

'************************
'Public Methods
'************************

Function IClient_GetStatistics() As Variant
Attribute IClient_GetStatistics.VB_Description = "Returns a variant array of test statistics."
    '-------------------------------------------------------------------------
    'Purpose:   Get the all summary status from the client.
    'Return:    Returns a single dimension long array in which
    '           element 0 = number of calls, 1 = Begin Milliseonds,
    '           and 2 = End Milliseconds
    '-------------------------------------------------------------------------
    'Returns statistical data for Explorer computation
    Dim lReturn(giSTAT_ARRAY_DIMENSION) As Long
    lReturn(giNUM_CALLS_ELEMENT) = glCallsReturned
    lReturn(giBEGIN_TICKS_ELEMENT) = glFirstServiceTick
    lReturn(giEND_TICKS_ELEMENT) = glLastCallbackTick
    IClient_GetStatistics = lReturn()
End Function

Public Function IClient_GetRecords() As Variant
Attribute IClient_GetRecords.VB_Description = "Returns a variant array of log records."
    '-------------------------------------------------------------------------
    'Purpose:   Use to retrieve all of the log records created by the client
    '           Keep calling until, it does not return a variant array
    'Return:    Returns a two dimension array in which
    '           the first four elements of the first dimension
    '           are Component(string), ServiceID(Long),Comment(string),
    '           and Milliseconds(long) respectively
    '           the second dimension represents the number of log records
    '           User Defined Types can not be returned from public
    '           procedures of public classes
    'Effects:   [gaLog]
    '               Redimensioned after calling GetRecords to not have empty
    '               records at the end
    '           [glLastAddedRecord]
    '               becomes equal to giNO_RECORDS
    '-------------------------------------------------------------------------
    
    GetWrittenLog
    'Trim the array to only send the filled elements
    If glLastAddedRecord >= 0 Then
        If UBound(gaLog, 2) <> glLastAddedRecord Then ReDim Preserve gaLog(giLOG_ARRAY_DIMENSION_ONE, glLastAddedRecord)
        IClient_GetRecords = gaLog()
        'Setting the glLastAddedRecord flag to giNO_RECORD will cause
        'Write log to ignore records on the next call
        glLastAddedRecord = giNO_RECORD
    Else
        IClient_GetRecords = Null
    End If
End Function

Public Sub IClient_StartTest(Optional ByVal lStartDelay As Long = -1&)
Attribute IClient_StartTest.VB_Description = "Starts a test."
    '-------------------------------------------------------------------------
    'Purpose:   Tells the client to start its Test
    'IN:
    '   [lStartDelay]
    '           If present it will be used as the timer interval so the start test
    '           can be delayed.  If missing, a default will be used.
    'Assumes:   All properties have already been set
    'Effects:
    '   [gbRunCompleteProcedure]
    '           becomes false
    '   [tmrStartTest]
    '           becomes enabled
    '-------------------------------------------------------------------------
    Dim s As String
    
    If gbTestInProcess Then Exit Sub
    s = LoadResString(giSTART_TEST)
    If gbLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus s
    ' Display or hide MTS Transaction status dialog
    If glModel = giMODEL_POOL And gvServiceConfiguration(ape_conShowMTSTransactions) _
             And (giServiceTask = (giMASK_USE_DB_TASK Or giMASK_WRITE_MTS_TRANSACTION)) Then
        With frmService
            .Show vbModeless, frmClient
            .Reset
        End With
    Else
        Unload frmService
    End If
    'Start timer and release the calling program.  When trmStarTest
    'get's its first event it will set its inteval to 0 and call
    'RunTest.
    gbRunCompleteProcedure = False
    gbStopping = False
    With frmClient.tmrStartTest
        If lStartDelay <= 0 Then lStartDelay = giDEFAULT_TIMER_INTERVAL
        .Interval = lStartDelay
        .Enabled = True
    End With
    Exit Sub
End Sub


Public Sub IClient_StopTest()
Attribute IClient_StopTest.VB_Description = "Ends a test."
    '-------------------------------------------------------------------------
    'Purpose:   Tells the client to Stop its Test
    '-------------------------------------------------------------------------
    gStopTest
End Sub

Public Sub IClient_SetSendData(ByVal lContainerType As APEDatasetTypeConstants, ByVal lRowSize As Long, _
                            Optional ByVal bRandomizeRowSize As Variant, Optional ByVal lRowSizeMin As Variant, _
                            Optional ByVal lRowSizeMax As Variant, _
                            Optional ByVal lNumRows As Variant, Optional ByVal bRandomizeNumRows As Variant, _
                            Optional ByVal lNumRowsMin As Variant, Optional ByVal lNumRowsMax As Variant)
Attribute IClient_SetSendData.VB_Description = "Determines the type and size of data that will be passed with Service Requests."
    '-------------------------------------------------------------------------
    'Purpose:   Set all of the parameter for data being passed
    '           in with the Service Request from the client.
    'In:
    '   [lContainerType]
    '           A code specifying the type of data to send with the Service
    '           Request.  See modAECon.bas for constants
    '   [lRowSize]
    '           The size of the row in bytes
    '   [bRandomizeRowSize]
    '           If true Client will pick a random RowSize for every Service
    '           Request.  lRowSizeMin will become the Lower bound of the range
    '           and lRowSizeMax will become the upper bound.
    '   [lRowSizeMin]
    '           Required if bRandomizeRowSize is true
    '   [lRowSizemax]
    '           Required if bRandomizeRowSize is true
    '   [lNumRows]
    '           The number of rows of data to send with the Service Request
    '   [bRandomizeNumRows
    '           If true Client will pick a random NumRows for every Service
    '           Request.  lNumRowsMin will become the Lower bound of the range
    '           and lNumRowsMax will become the upper bound.
    '   [lNumRowsMin]
    '           Required if bRandomizeNumRows is true
    '   [lNumRowsMax]
    '           Required if bRandomizeNumRows is true
    'Effects:
    '   [gudtSendNumRows]
    '           becomes value of lNumRows
    '   [gudtSendRowSize]
    '           becomes value of lRowSize
    '   [glSendContainerType]
    '           becomes value of lContainerType
    '-------------------------------------------------------------------------
    glSendContainerType = lContainerType
    With gudtSendRowSize
        .SpecificValue = lRowSize
        If IsMissing(bRandomizeRowSize) Then .Random = False Else .Random = CBool(bRandomizeRowSize)
        If .Random Then
            If IsMissing(lRowSizeMin) Or IsMissing(lRowSizeMax) Then
                GoTo SetSendData_InvalidParameter
            Else
                .LowerValue = lRowSizeMin
                .UpperValue = lRowSizeMax
            End If
        End If
    End With
                
    With gudtSendNumRows
        If Not IsMissing(lNumRows) Then .SpecificValue = lNumRows
        If IsMissing(bRandomizeNumRows) Then .Random = False Else .Random = CBool(bRandomizeNumRows)
        If .Random Then
            If IsMissing(lNumRowsMin) Or IsMissing(lNumRowsMax) Then
                GoTo SetSendData_InvalidParameter
            Else
                .LowerValue = lNumRowsMin
                .UpperValue = lNumRowsMax
            End If
        End If
    End With
    Exit Sub
SetSendData_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetReceiveData(ByVal lContainerType As APEDatasetTypeConstants, ByVal lRowSize As Long, _
                            Optional ByVal bRandomizeRowSize As Variant, Optional ByVal lRowSizeMin As Variant, _
                            Optional ByVal lRowSizeMax As Variant, _
                            Optional ByVal lNumRows As Variant, Optional ByVal bRandomizeNumRows As Variant, _
                            Optional ByVal lNumRowsMin As Variant, Optional ByVal lNumRowsMax As Variant)
Attribute IClient_SetReceiveData.VB_Description = "Determines the type and size of data that will be returned as Service Request results. "
    '-------------------------------------------------------------------------
    'Purpose:   Set all of the parameter for data being passed
    '           to the client as results of the Service Request.
    'In:
    '   [lContainerType]
    '           A code specifying the type of data to return from the Service
    '           Request.  See modAECon.bas for constants
    '   [lRowSize]
    '           The size of the row in bytes
    '   [bRandomizeRowSize]
    '           If true Client will pick a random RowSize for every Service
    '           Request.  lRowSizeMin will become the Lower bound of the range
    '           and lRowSizeMax will become the upper bound.
    '   [lRowSizeMin]
    '           Required if bRandomizeRowSize is true
    '   [lRowSizemax]
    '           Required if bRandomizeRowSize is true
    '   [lNumRows]
    '           The number of rows of data to return from the Service Request
    '   [bRandomizeNumRows
    '           If true Client will pick a random NumRows for every Service
    '           Request.  lNumRowsMin will become the Lower bound of the range
    '           and lNumRowsMax will become the upper bound.
    '   [lNumRowsMin]
    '           Required if bRandomize NumRows is true
    '   [lNumRowsMax]
    '           Required if bRandomizeNumRows is true
    'Effects:
    '   [gudtSendNumRows]
    '           becomes value of lNumRows
    '   [gudtSendRowSize]
    '           becomes value of lRowSize
    '   [glSendContainerType]
    '           becomes value of lContainerType
    '-------------------------------------------------------------------------
    glReceiveContainerType = lContainerType
    With gudtReceiveRowSize
        .SpecificValue = lRowSize
        If IsMissing(bRandomizeRowSize) Then .Random = False Else .Random = CBool(bRandomizeRowSize)
        If .Random Then
            If IsMissing(lRowSizeMin) Or IsMissing(lRowSizeMax) Then
                GoTo SetReceiveData_InvalidParameter
            Else
                .LowerValue = lRowSizeMin
                .UpperValue = lRowSizeMax
            End If
        End If
    End With
                
    With gudtReceiveNumRows
        If Not IsMissing(lNumRows) Then .SpecificValue = lNumRows
        If IsMissing(bRandomizeNumRows) Then .Random = False Else .Random = CBool(bRandomizeNumRows)
        If .Random Then
            If IsMissing(lNumRowsMin) Or IsMissing(lNumRowsMax) Then
                GoTo SetReceiveData_InvalidParameter
            Else
                .LowerValue = lNumRowsMin
                .UpperValue = lNumRowsMax
            End If
        End If
    End With
    Exit Sub
SetReceiveData_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetServiceConfiguration(ByVal vServiceConfiguration As Variant)
    gvServiceConfiguration = vServiceConfiguration
End Sub

Public Sub IClient_SetProperties(ByVal bShow As Boolean, Optional ByVal bLog As Variant, Optional ByVal lID As Variant, Optional ByVal lModel As Variant, _
                        Optional ByVal lLogThreshold As Variant, Optional ByVal iCallbackMode As Variant)
Attribute IClient_SetProperties.VB_Description = "Sets the Client related properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Client properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           Show, Log, Model, NumberOfCalls, WaitPeriod, ServiceCommand,
    '           ServiceMilliseconds, UseProcessor, LogThreshold, UseDefaultCallback
    '-------------------------------------------------------------------------
    Me.IClient_Show = bShow
    DisplayStatus LoadResString(giINITIALIZING_TEST)
    If Not IsMissing(bLog) Then gbLog = bLog
    If Not IsMissing(lID) Then Me.IClient_ID = lID
    If Not IsMissing(lModel) Then glModel = lModel
    If Not IsMissing(lLogThreshold) Then Me.IClient_LogThreshold = lLogThreshold
    If Not IsMissing(iCallbackMode) Then Me.IClient_CallbackMode = iCallbackMode
End Sub

Public Sub IClient_SetTestDuration(Optional ByVal lNumberOfCalls As Variant, _
                           Optional ByVal lNumberOfMilliseconds As Variant)
Attribute IClient_SetTestDuration.VB_Description = "Sets how long a test will last in number of calls or number of milliseconds."
    '-------------------------------------------------------------------------
    'Purpose:   The the parameters effecting the TestDuration
    'In:        If no parameters are present then the test will continue
    '           until interupted by the Stop test method.
    '   [lNumberOfCalls]
    '           If present, the test duration will last for a number of
    '           calls specified by this parameter
    '   [lNumberOfMilliseconds]
    '           If present and lNumberOfCalls is missing, the test duration
    '           will last for the number of milliseconds specified by this
    '           parameter.
    '-------------------------------------------------------------------------
    If Not IsMissing(lNumberOfCalls) Then
        giTestDurationMode = giTEST_DURATION_CALLS
        glNumberOfCalls = lNumberOfCalls
    ElseIf Not IsMissing(lNumberOfMilliseconds) Then
        giTestDurationMode = giTEST_DURATION_TICKS
        glTestDurationInTicks = lNumberOfMilliseconds
    Else
        giTestDurationMode = giTEST_DURATION_CONTINUE
    End If
End Sub

Public Sub IClient_SetWaitPeriod(ByVal lMilliseconds As Long, Optional ByVal bRandom As Variant, _
                            Optional ByVal lMillisecondsMin As Variant, _
                            Optional ByVal lMillisecondsMax As Variant)
Attribute IClient_SetWaitPeriod.VB_Description = "Sets how long the Client will wait between submitting Service Requests in milliseconds."
    '-------------------------------------------------------------------------
    'Purpose:   Specifies how many Milliseconds to wait between each call
    'Effects:
    '   [gudtWaitPeriod]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    With gudtWaitPeriod
        .SpecificValue = lMilliseconds
        If IsMissing(bRandom) Then .Random = False Else .Random = CBool(bRandom)
        If .Random Then
            If IsMissing(lMillisecondsMin) Or IsMissing(lMillisecondsMax) Then
                GoTo SetWaitPeriod_InvalidParameter
            Else
                .LowerValue = lMillisecondsMin
                .UpperValue = lMillisecondsMax
            End If
        End If
    End With
    Exit Sub
SetWaitPeriod_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetTaskDuration(ByVal lMilliseconds As Long, Optional ByVal bRandom As Variant, _
                            Optional ByVal lMillisecondsMin As Variant, _
                            Optional ByVal lMillisecondsMax As Variant)
Attribute IClient_SetTaskDuration.VB_Description = "Sets how long the default service object's task will execute in milliseconds."
    '-------------------------------------------------------------------------
    'Purpose:   Specifies how many milliseconds the Service should use the processor on each call
    'Effects:
    '   [gudtTaskDuration]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    With gudtTaskDuration
        .SpecificValue = lMilliseconds
        If IsMissing(bRandom) Then .Random = False Else .Random = CBool(bRandom)
        If .Random Then
            If IsMissing(lMillisecondsMin) Or IsMissing(lMillisecondsMax) Then
                GoTo SetTaskDuration_InvalidParameter
            Else
                .LowerValue = lMillisecondsMin
                .UpperValue = lMillisecondsMax
            End If
        End If
    End With
    Exit Sub
SetTaskDuration_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetSleepPeriod(ByVal lMilliseconds As Long, Optional ByVal bRandom As Variant, _
                            Optional ByVal lMillisecondsMin As Variant, _
                            Optional ByVal lMillisecondsMax As Variant)
    '-------------------------------------------------------------------------
    'Purpose:   Specifies how many milliseconds the Service should sleep on each call
    'Effects:
    '   [gudtSleepPeriod]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    With gudtSleepPeriod
        .SpecificValue = lMilliseconds
        If IsMissing(bRandom) Then .Random = False Else .Random = CBool(bRandom)
        If .Random Then
            If IsMissing(lMillisecondsMin) Or IsMissing(lMillisecondsMax) Then
                GoTo SetSleepPeriod_InvalidParameter
            Else
                .LowerValue = lMillisecondsMin
                .UpperValue = lMillisecondsMax
            End If
        End If
    End With
    Exit Sub
SetSleepPeriod_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetServiceTask(ByVal iServiceTask As Integer)
Attribute IClient_SetServiceTask.VB_Description = "Sets the task that the default service will execute."
    '-------------------------------------------------------------------------
    'Purpose:   To instruct Client what task to require from AEService.Service
    'Effects:
    '   [giServiceTask]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    giServiceTask = iServiceTask
End Sub

Public Sub IClient_SetDatabaseQuery(ByVal sQuery As String)
    '-------------------------------------------------------------------------
    'Purpose:   Specifies the query used for a database task
    'Effects:
    '   [gsDatabaseQuery]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gsDatabaseQuery = sQuery
End Sub

Public Sub IClient_SetServiceCommand(ByVal bUseDefaultService As Boolean, Optional ByVal sName As Variant)
Attribute IClient_SetServiceCommand.VB_Description = "Determines if the default Service object or a custom service object will be used."
    '-------------------------------------------------------------------------
    'Purpose:   Specifies what ProgID to and command to use for Service
    '           requests
    'IN:
    '   [bUseDefaultService]
    '           If true use default service, else use require following parameter
    '           as service command
    '   [sName]
    '           Required if bUseDefaultService is False
    '           Ex: "Library.Class.Method"
    'Effects:
    '   [gsServiceCommand]
    '           Set equal to parameter
    '-------------------------------------------------------------------------
    gbUseDefaultService = bUseDefaultService
    If Not bUseDefaultService Then
        If IsMissing(sName) Then
            GoTo SetServiceCommand_InvalidParameter
        ElseIf VarType(sName) <> vbString Then
            GoTo SetServiceCommand_InvalidParameter
        Else
            gsServiceCommand = sName
        End If
    End If
    Exit Sub
SetServiceCommand_InvalidParameter:
    Err.Raise giREQUIRED_PARAMETER_IS_MISSING + vbObjectError, , LoadResString(giREQUIRED_PARAMETER_IS_MISSING)
End Sub

Public Sub IClient_SetWorkerProperties(ByVal bLog As Boolean, Optional ByVal bEarlyBindServices As Variant, _
        Optional ByVal bPersistentServices As Variant, Optional ByVal bPreloadServices As Variant)
Attribute IClient_SetWorkerProperties.VB_Description = "Sets all Worker related properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Worker properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           ShowWorker, LogWorker, EarlyBindServices, PersistentServices
    '           PreloadServices
    '-------------------------------------------------------------------------
    gbLogWorker = bLog
    If Not IsMissing(bEarlyBindServices) Then gbEarlyBindServices = bEarlyBindServices
    If Not IsMissing(bPersistentServices) Then IClient_PersistentServices = bPersistentServices
    If Not IsMissing(bPreloadServices) Then gbPreloadServices = bPreloadServices

End Sub

Public Sub IClient_SetConnectionProperties(ByVal bRemote As Boolean, Optional ByVal bNetOLE As Variant, _
        Optional ByVal sAddress As Variant, Optional ByVal sProtocol As Variant, _
        Optional ByVal lAuthentication As Variant)
Attribute IClient_SetConnectionProperties.VB_Description = "Sets the connection properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Connection Settings that the Client will use to
    '           connect to a remote Worker
    'In:
    '   [bRemote]
    '           If true connect to a remote Worker instead of a local one
    '   [bNetOLE]
    '           If true use NetOLE (DCOM) instead of Remote Automation
    '   [sAddress]
    '           Machine name to connect to
    '   [sProtocol]
    '           Protocol sequence to use when connecting to remote objects
    '   [lAuthentication]
    '           Authentication level to use
    'Effects:   The following globals are set to the value of the corresponding
    '           parameters:
    '           gbConnectionRemote, gbConnectionNetOLE, gsConnectionAddress
    '           gsConnectionProtocol, glConnectionAuthentication
    '-------------------------------------------------------------------------
    gbConnectionRemote = bRemote
    If Not IsMissing(bNetOLE) Then gbConnectionNetOLE = bNetOLE
    If Not IsMissing(sAddress) Then gsConnectionAddress = sAddress
    If Not IsMissing(sProtocol) Then gsConnectionProtocol = sProtocol
    If Not IsMissing(lAuthentication) Then glConnectionAuthentication = lAuthentication
        
End Sub

'******************
'Private Procedures
'******************

Private Sub RestoreLocalConnSettings()
    '-------------------------------------------------------------------------
    'Purpose:       If this AEClient was the first client created on the local
    '               machine, restores the Connections Settings of the Worker and
    '               the QueueMgr to local.  Settings need to be restored to
    '               local incase machine is used as a server in another session.
    '-------------------------------------------------------------------------
    Dim iResult As Integer
    'Called by Class_Terminate
    
    If mbFirstClientOnMachine Then
        iResult = goRegClass.SetAutoServerSettings(False, "AEWorker.Worker")
        iResult = goRegClass.SetAutoServerSettings(False, "AEQueueMgr.Queue")
        iResult = goRegClass.SetAutoServerSettings(False, "AEPoolMgr.Pool")
    End If
End Sub

Private Sub Class_Initialize()
    On Error GoTo Class_InitializeError
    '-------------------------------------------------------------------------
    'Purpose:   If this is the first instanciation
    '           Put the Client in a "Ready" state.  Load RacReg, set property
    '           defaults
    'Effects:
    '   [glInstances]
    '           increments it by one
    '-------------------------------------------------------------------------
    'Keep track of the number of instances
    'to responsd to the first instancing
    glInstances = glInstances + 1
    If glInstances = 1 Then
        If Not App.PrevInstance Then mbFirstClientOnMachine = True
        'Make sure we don't get a timeout when starting OLE server across the net.
        App.OleServerBusyRaiseError = True
        App.OleServerBusyTimeout = 10000
        'Create Objects
        Set goRegClass = New RacReg.RegClass
        
        Set gcServices = New Collection
        glLastAddedRecord = giNO_RECORD
        'Get a temp file name
        gsTempFile = GetTempFile
        'Default Properties and variables
        glModel = giMODEL_QUEUE
        gbTestInProcess = False
        glSendContainerType = giCONTAINER_TYPE_VARRAY
        glReceiveContainerType = giCONTAINER_TYPE_VARRAY
        gbShow = True
        gbLog = True
        glModel = giMODEL_QUEUE
        glCallsMade = 0
        gbShow = True
        gbLog = True
        gbLogWorker = True
        glLogThreshold = 0
        'Set status flags
        gbStopping = False
    End If
    Exit Sub
Class_InitializeError:
    LogError Err
    Resume Next
End Sub

Private Sub Class_Terminate()
    '-------------------------------------------------------------------------
    'Purpose:   If the last reference to the Client is destroyed
    '           Close the Client
    'Effects:
    '           Restore Local connection settings
    '           Run gStopTest
    '           Delete Temporary file
    '   [glInstances]
    '           decrements it by one
    '-------------------------------------------------------------------------
    On Error GoTo Class_TerminateError
    glInstances = glInstances - 1
    If glInstances <= 0 Then
        'There is one internal reference to the Client class in the form module.  So,
        'we need to terminate when glInstances = 1 not 0.
        'Call gStopTest so that Services are cancelled
        'and set flag for shut down after Services are cancelled
        RestoreLocalConnSettings
        Close   'close in case getting logs was canceled
        Kill gsTempFile
        gbShutDown = True
        gStopTest
        Set goExplorer = Nothing
    End If
    Exit Sub
Class_TerminateError:
    Select Case Err.Number
        Case ERR_FILE_NOT_FOUND
            'There is no file to kill
            Resume Next
        Case Else
            LogError Err
            Resume Next
    End Select
End Sub


