Attribute VB_Name = "modClient"
Option Explicit
'-------------------------------------------------------------------------
'The project is the Client component of the Application Performance Explorer
'This client is designed to be instanciated by and configured by the APE
'Manager.  It can generate Service Request by calling the QueueManager.
'Or it can call the Worker to produce synchronous work.  In either of these
'sinarios the frequency can vary, and the type and size of data it passes
'can vary.
'
'Key Files:
'   frmClnt.frm     The only form in the app
'   Client.cls      Single-use, creatable, public class that provides
'                   OLE interface for Manager to instanciate and configure
'   clsCalbk.cls    Not creatable, but public class that is passed to the
'                   QueueMgr to receive call backs
'   clsCntSv.cls    Class used to store data on expected callbacks
'   clsDrtTl.cls    Class providing a runtest method for running direct
'                   instanciation tests
'   clsPosFm.cls    Tool form saving form position to registry
'   clsQueTl.cls    Class providing a runtest method for running Queue
'                   manager tests
'-------------------------------------------------------------------------

'Declares
#If UNICODE Then
    Declare Function GetTempFileName Lib "Kernel32" Alias "GetTempFileNameW" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
    Declare Function GetTempPath Lib "Kernel32" Alias "GetTempPathW" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
    Public Declare Function GetComputerName Lib "Kernel32" Alias "GetComputerNameW" (ByVal lpBuffer As String, nSize As Long) As Long
#Else
    Declare Function GetTempFileName Lib "Kernel32" Alias "GetTempFileNameA" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
    Declare Function GetTempPath Lib "Kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
    Public Declare Function GetComputerName Lib "Kernel32" Alias "GetComputerNameA" (ByVal lpBuffer As String, nSize As Long) As Long
#End If
Public Declare Function GetTickCount Lib "Kernel32" () As Long
Public Declare Sub Sleep Lib "Kernel32" (ByVal dwMilliseconds As Long)

'Caption String Constants
Public Const giFORM_CAPTION  As Integer = 101                   'Form Caption
Public Const giCALLS_MADE_CAPTION  As Integer = 102
Public Const giCALLS_RETURNED_CAPTION  As Integer = 103

'Log String Constants
Public Const giCOMPONENT_NAME  As Integer = 2
Public Const giCALLBACK_RECEIVED  As Integer = 3
Public Const giCALLBACK_ERROR_RECEIVED  As Integer = 4
Public Const giQUEUE_SERVICE  As Integer = 5
Public Const giQUEUE_SERVICE_ERROR  As Integer = 7
Public Const giQUEUE_SERVICE_COLLISION_RETRY  As Integer = 9
Public Const giWAIT_PERIOD_ERROR  As Integer = 12
Public Const giSTART_TEST  As Integer = 13
Public Const giSTOP_TEST  As Integer = 14
Public Const giTEST_STARTED  As Integer = 16
Public Const giTEST_COMPLETE  As Integer = 17
Public Const giSERVICES_POSTED  As Integer = 18
Public Const giCALLBACKS_COMPLETE As Integer = 19
Public Const giINITIALIZING_TEST As Integer = 20
Public Const giDIRECT_SERVICE  As Integer = 21
Public Const giWRITING_TEMP_FILE  As Integer = 23
Public Const giUSING_NO_AUTHENTICATION  As Integer = 24
Public Const giDISK_FULL As Integer = 26
Public Const giPOOL_MGR_REJECTION_WAITS_EXHAUSTED As Integer = 27
Public Const giERROR_CREATE_MTS_OBJECT As Integer = 28

Public Const giFONT_CHARSET_INDEX As Integer = 30
Public Const giFONT_NAME_INDEX As Integer = 31
Public Const giFONT_SIZE_INDEX  As Integer = 32

Public Const giERROR_PREFIX  As Integer = 50                ' "Error:  "

' MTS Transaction-related text
Public Const giSUCCEEDED_TRANSACTIONS_CAPTION As Integer = 110
Public Const giABORTED_TRANSACTIONS_CAPTION As Integer = 111
Public Const giBEGIN_MTS_TRANSACTION As Integer = 112
Public Const giEND_MTS_TRANSACTION_FAILED As Integer = 113
Public Const giEND_MTS_TRANSACTION_SUCCEEDED As Integer = 114
Public Const giMTS_FORM_CAPTION As Integer = 115

Public Const giRACREG_ERROR_CODE_OFFSET As Integer = 200               'Add offset to racreg32 error codes
                                                            'to make corresponding resource string key
'Application Error Constants
Public Const giCOLLISION_ERROR  As Integer = 32767          'OLE collision retries exausted
Public Const giREQUIRED_PARAMETER_IS_MISSING As Integer = 32765
Public Const giPOOLMGR_RETURNED_NOTHING As Integer = 32766
Public Const giCONNECTION_SETTING_FAILED As Integer = 32750     'An error was returned by RacReg32
Public Const giNO_NAMED_PIPES_UNDER_WIN95  As Integer = 32739     ' Named pipes cannot be created under Win95
'Queue Manager errors
Public Const giQUEUE_MGR_IS_BUSY As Integer = 32749

'Other Constants
Public Const giCALL_SENT_AND_RECEIVED_MAX_DIFFERENCE As Integer = 200   'If the number of calls that the
                                                                'client has made is this much greater than
                                                                'the number of calls received back then
                                                                'pause making calls until callbacks catch up
Public Const giREDIM_CHUNK_SIZE  As Integer = 100               'Size of redimension chunks of log array
Public Const giNO_RECORD  As Integer = -1                       'Flag value meaning no records
Public Const giMAX_ALLOWED_RETRIES  As Integer = 500            'Max allowed OLE automation call retries
Public Const giRETRY_WAIT_MIN  As Integer = 1000                'Retry Wait is measure in DoEvent cyles
Public Const giRETRY_WAIT_MAX  As Integer = 5000
Public Const giROWS_RETURNED_PER_GET_RECORDS As Integer = 500   'Max number of records returned for
                                                                'each call of GetRecords
Public Const RPC_C_AUTHN_LEVEL_NONE  As Integer = 1             'Remote Automation Authentication level constant
Public Const giPOOL_WAIT_RETRY_MIN As Integer = 1000            'The minum milliseconds to wait if the Pool Manager
                                                                'rejects request for a Worker
Public Const giQUEUE_WAIT_RETRY_MIN As Integer = 3000           'The minimum to wait in milliseconds if the Queue
                                                                'raises an error that it is to busy to process
                                                                'a Service Request
Public Const glMAX_LONG As Long = 2147483647
Public Const giDEFAULT_TIMER_INTERVAL As Integer = 100
Public Const giSLEEP_INCREMENT As Integer = 500                     ' Time (ms) to sleep on each iteration of a delay loop

'Type
Public Type RANDOM_DATA_GROUP
    Random As Boolean
    SpecificValue As Long
    UpperValue As Long
    LowerValue As Long
End Type

'Global Variables and Objects
Public goTestTool As Object             'Object of a class having RunTest method
                                        'actually runs the test.  Different classes
                                        'are used for different types of tests
Public gcServices As Collection         'Collection of clsCllietnService class objects
                                        'stores expected callback information
Public gaLog() As Variant               'Array that stores log records
Public glCallsMade As Long              'Number of calls made in test
Public glCallsReturned As Long          'Number of callbacks made in a test
Public glInstances As Long              'Count of intances of Client class
Public glLogThresholdRecs As Long       'Log threshold in record count
Public goRegClass As RacReg.RegClass    'RacReg used to change connection settings
Public glLastAddedRecord As Long        'Last added log record array index
Public glFirstServiceTick As Long       'Milliseconds of test start
Public glLastCallbackTick As Long       'Milliseconds of end of test
Public gsTempFile As String             'Temporary log file name

'Flags
Public gbTestInProcess As Boolean           'If true, test is in process
Public gbStopping As Boolean                'If true, stopping test, procedures check it
Public gbShutDown As Boolean                'If true, shutting down client
Public gbRunCompleteProcedure As Boolean    'Timer will run CompleteTest
Public gbRunning As Boolean                 'In a RunTest method
Public gbGetWrittenLogCalled As Boolean     'GetWritten log was called

'Public Property Variables
Public gsServiceCommand As String               'Command string to pass to Queue.Add
Public gbUseDefaultService As Boolean           'If true use default service object
Public gudtWaitPeriod As RANDOM_DATA_GROUP      'How long to wait between calls
Public glNumberOfCalls As Long                  'Number of Calls to make in test
Public glTestDurationInTicks As Long            'Number of Milliseconds for Test to last
Public giTestDurationMode As Integer            'Mode of determining test duration
Public gudtSendNumRows As RANDOM_DATA_GROUP     'Number of rows of data to send with Service request
Public gudtSendRowSize As RANDOM_DATA_GROUP     'Number of bytes of data to put in each row of data
Public glSendContainerType As Long              'Type of data to send with Service request
Public gudtReceiveNumRows As RANDOM_DATA_GROUP  'Number of rows to request back from Service request
Public gudtReceiveRowSize As RANDOM_DATA_GROUP  'Size of each row in bytes to request back
Public glReceiveContainerType As Long           'Container type to request back from Service request
Public gudtTaskDuration As RANDOM_DATA_GROUP    'Length of time a Service request should use the processor
Public gudtSleepPeriod As RANDOM_DATA_GROUP    'Length of time a Service request should sleep
Public giServiceTask As Integer                'Code for whether Service should use processor cycles during
Public gsDatabaseQuery As String        'Query for the Service to use to execute a database request
Public giUseProcPercent As Integer              'Percentage of requests that services should use processor
Public gvServiceConfiguration As Variant     'Service configuration information

Public gbShow As Boolean                'If true, show frmClient during test
Public gbLog As Boolean                 'If true log events during test
Public glCallbackMode As Long           'Determines if and how client receives results from
                                        'services requested from QueueManager
                                        'see "Callback mode keys" in modAEConstants
Public gbLogWorker As Boolean           'If true, have directly instanciated worker log
Public gbPreloadServices As Boolean     'If true, have directly instanciated worker preload
                                        'needed service object
Public gbPersistentServices As Boolean  'If true, have directly instanciated worker retain
                                        'references to Service objects
Public gbEarlyBindServices As Boolean   'If true, have directly instanciated workers use
                                        'earlybound service objects
Public glModel As Long                  'APE framework model to use during test
Public glClientID As Long               'Client ID Manager uses to manager Client object
Public gsConnectionAddress As String    'Net address of APE server objects to use
Public gsConnectionProtocol As String   'Protocol to connect with
Public glConnectionAuthentication As String 'Authentiation level to use
Public gbConnectionRemote As Boolean    'If true, connect to a remote server not local
Public gbConnectionNetOLE As Boolean    'If true, use NetOLE (DCOM) instead of Remote Automation
Public goExplorer As APEInterfaces.IManagerCallback 'Explorer object passed to client from Manager
                                           'Client calls manager back with this
Public glLogThreshold As Long           'Log threshodl in kilobytes

Public Sub CompleteTest()
    '-------------------------------------------------------------------------
    'Purpose:   Release objects used during test, and call Manager with
    '           notification the test.
    'Effects:
    '   [gbTestInProcess]
    '           becomes false
    '   [goTesttool]    destroyed
    '   [goExplorer]    destroyed
    '   [gcServices]    destroyed
    '-------------------------------------------------------------------------
    Dim s As String
    Static stbInCompleteTest As Boolean 'If true already in this procedure
    
    'Exit if reentry caused by timer click
    'while calling goExplorer
    
    If stbInCompleteTest Then Exit Sub
    stbInCompleteTest = True
    On Error GoTo CompleteTestError
    s = LoadResString(giTEST_COMPLETE)
    If gbLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus s
    If Not goExplorer Is Nothing Then goExplorer.Done ape_ctClient
    Set goTestTool = Nothing
    Set gcServices = Nothing
    stbInCompleteTest = False
    gbTestInProcess = False
    Exit Sub
CompleteTestError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim iRetry As Integer
            Dim il As Integer
            Dim ir As Integer
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(giQUEUE_SERVICE_COLLISION_RETRY), GetTickCount(), False
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                Resume
            Else
                'We reached our max retries
                AddLogRecord gsNULL_SERVICE_ID, LoadResString(giCOLLISION_ERROR), GetTickCount(), False
                Resume Next
            End If
        Case Else
            s = LoadResString(giQUEUE_SERVICE_ERROR) & CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
            stbInCompleteTest = False
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Sub
    End Select
End Sub


Public Sub gStopTest()
    '-------------------------------------------------------------------------
    'Purpose:   To stop cancel the current test
    'Assumes:   If gbRunning is true, a method procedure or a callback method
    '           are being processed.  We can exit this procedure and one of those
    '           methods will check the gbStopping flag and call gStopTest again
    '           If gbShutDown is true, then this procedure was called by the
    '           Terminate event of the Client class on the release of its last
    '           reference
    'Effects:
    '   [gbTestInProcess]
    '                   becomes false
    '   [goTesttool]    destroyed
    '   [goExplorer]    destroyed
    '   [gcServices]    destroyed
    '   [goRegClass]
    '           If gbShutDown is true destroy goRegClass
    '   [frmClient]
    '           If gbShutDown is true unload
    '-------------------------------------------------------------------------
    Dim oCA As clsClientService
    Dim s As String
    On Error GoTo gStopTestError
    
    gbStopping = True
    s = LoadResString(giSTOP_TEST)
    If gbLog Then AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus s
    'Make sure we are not in the middle of queueing an Service.
    'If we are, get out.  QueueService will check the gbStopping flag
    'and call the gStopTest method again when it's done.
    If gbRunning Then Exit Sub
    Set goTestTool = Nothing
    Set gcServices = Nothing
    gbTestInProcess = False
    'See if this was called by Terminate if it was unload form
    If gbShutDown Then
        Set goRegClass = Nothing
        Unload frmClient
    End If
    Exit Sub
gStopTestError:
    Select Case Err.Number
        Case Else
            LogError Err
            If glInstances > 0 Then Err.Raise Err.Number, Err.Source, Err.Description
            Resume Next
    End Select
End Sub


Public Sub AddServiceRecord(sID As String, sCommand As String, lTicks As Long)
    '-------------------------------------------------------------------------
    'Purpose:   Put a new Service Request in the Service collection.
    'In:
    '   [sID]   Service Request ID
    '   [sCommand]
    '           Service Request Command sent to QueueMgr
    '   [lTicks]
    '           Tick count at time of call to QueueMgr
    'Effects:
    '   [gcServices]
    '           Adds a clsClientService class object to collection
    '-------------------------------------------------------------------------
    Dim oCA As clsClientService     'Object with properties designed to store
                                    'Service request information
    
    Set oCA = New clsClientService
    With oCA
        .sID = sID
        .sCommand = sCommand
        .lStartTicks = lTicks
    End With
    gcServices.Add oCA, oCA.sID
End Sub

Public Sub WriteLog()
    '-------------------------------------------------------------------------
    'Purpose:   Writes the current log records to a temp file and
    '           removes the records from memory
    'Assumes:   If gbGetWrittenLogCalled is true, any records currently in the
    '           temporary file are no longer needed, but the file may still be
    '           open.
    'Effects:
    '           All records currently in gaLog are written to a temporary file
    '           and removed from the array
    '   [gbGetWrittenLogCalled]
    '           becomes false
    '   [glLastAddedRecord]
    '           becomes giNO_RECORD
    '   [gaLog] becomes redimension to store new records
    '-------------------------------------------------------------------------
    'Don't save the Component name because the component
    'is always the same
    Dim sServiceID As String
    Dim sComment As String
    Dim lMilliseconds As Long
    Dim lFile As Long
    Dim l As Long
    On Error GoTo WriteLogError
    If glLastAddedRecord > giNO_RECORD Then
        If gbLog Then
            AddLogRecord gsNULL_SERVICE_ID, LoadResString(giWRITING_TEMP_FILE), GetTickCount, False
        End If
        
        'Check to see if the contents of the temp file
        'need deleted first, the reason it is not delete
        'when the flag is flipped is to give one the chance
        'of rescueing it if the Manager fails to retreive
        'the records from it
        If gbGetWrittenLogCalled Then
            Close   'Close in case last GetWrittenLogs cancelled
            Kill gsTempFile
            gbGetWrittenLogCalled = False
        End If
            
        lFile = FreeFile
        Open gsTempFile For Append As lFile
            For l = 0 To glLastAddedRecord
                sServiceID = gaLog(giSERVICE_ELEMENT, l)
                sComment = gaLog(giCOMMENT_ELEMENT, l)
                lMilliseconds = gaLog(giMILLI_SECONDS_ELEMENT, l)
                Write #lFile, sServiceID, sComment, lMilliseconds
                'Reset logrecord counter no after writing the first record
                'so that records are not added after the count that is being
                'written and therefore, lost.  This also protects from
                'Addlogrecord trying to write a record greater than
                'giREDIM_CHUNK_SIZE write after gaLog is redimensioned
                If l = 0 Then glLastAddedRecord = giNO_RECORD
            Next
        Close #lFile
        'Remove LogRecords from memory
        'Preserve is used because there is a potential
        'for a log record to be added after the above line
        'but before the following one
        ReDim Preserve gaLog(giLOG_ARRAY_DIMENSION_ONE, giREDIM_CHUNK_SIZE)
    End If
    Exit Sub
WriteLogError:
    Select Case Err.Number
        Case ERR_DISK_FULL
            'Turn off logging erase array
            'leave present file for later retrieval
            DisplayStatus LoadResString(giDISK_FULL)
            Close lFile
            Erase gaLog
            gbLog = False
            Exit Sub
        Case ERR_FILE_NOT_FOUND
            'There is no temp file to kill
            Resume Next
        Case Else
            Close lFile
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Sub
    End Select
End Sub


Public Sub GetWrittenLog()
    '-------------------------------------------------------------------------
    'Purpose:   Checks to see if there is log records written to a temp file
    '           If there are it inputs it and adds it to the gaLog array
    '           If it reaches the chunk size for passing log records it will
    '           exit the loop, leaving the file open. It is necessary to keep
    '           calling this function until no records or added.  Do not call
    '           this function more than once until the array that was filled
    '           was erased.  The external process that is calling a method that
    '           calls this procedure should be responsible for calling until
    '           all records have been attained.
    'Effects:
    '           [gbGetWrittenLogCalled] becomes true
    '           Temp file may be left open if all records are not read
    '           AddlogRecord is called for each record read
    'Assumption:
    '           If gbGetWrittenLogCalled is true then the temp file is already
    '           open, ready for the next record to be read.
    '           If the EOF is not reached before the glROWS_RETURNED_PER_GET_RECORDS
    '           is reached then the external process that called Logger.GetRecords
    '           will call it again, to get the rest of the records
    '-------------------------------------------------------------------------
    
    Static stlFile As Long      'File number
    Dim sPath As String         'Path and file name of temporary file
    Dim sServiceID As String      'Service Request ID
    Dim sComment As String      'Comment in log record
    Dim lMilliseconds As Long   'Milliseconds in log record
    Dim lAddedCount As Long     'Count of how many records have been read and added to memory
    
    On Error GoTo GetWrittenLogError
    sPath = gsTempFile
    
    'Open file if it is not open yet
    If Not gbGetWrittenLogCalled Then
        'Write records in memory first to order the records
        'with any records that may have already been written
        WriteLog
        gbGetWrittenLogCalled = True
        stlFile = FreeFile
        Open sPath For Input As stlFile
    End If
    
    Do Until EOF(stlFile)
        'Component was not saved to temp file because
        'the component name is always the same in this file
        Input #stlFile, sServiceID, sComment, lMilliseconds
        AddLogRecord sServiceID, sComment, lMilliseconds, True
        lAddedCount = lAddedCount + 1
        'Exit here if max record size was reached
        If lAddedCount = giROWS_RETURNED_PER_GET_RECORDS Then Exit Sub
    Loop
    Close
    Exit Sub
GetWrittenLogError:
    Select Case Err.Number
        Case ERR_FILE_NOT_FOUND
            'There are no written records so exit
            Exit Sub
        Case ERR_BAD_FILE_NAME
            'We have already reached the end of the file
            'and it has been closed
            Exit Sub
        Case Else
            Close
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Sub
    End Select
End Sub


Public Function GetTempFile() As String
    '-------------------------------------------------------------------------
    'Purpose:   Gets a temp file name from the system
    'Return:    a valid temporary file name
    '-------------------------------------------------------------------------
    Dim lSize As Long
    Dim sPath As String
    Dim sName As String
    Dim lResult As Long
    
    sPath = Space(255)
    lResult = GetTempPath(255, sPath)
    sPath = Left$(sPath, lResult)
    sName = Space(255)
    lResult = GetTempFileName(sPath, "AEC", 0, sName)
    lResult = InStr(sName, vbNullChar)
    sName = Left$(sName, lResult - 1)
    
    GetTempFile = sName
End Function

Public Sub DisplayString(s As String)
    '-------------------------------------------------------------------------
    'Purpose:   Adds the passed text to to the list box.  Only used if conditional
    '           compile ccShowList is true.
    'Assumes:   If gbShow is true, form is visible
    '           If ccShowList is true, lstLog is visible and positioned
    '-------------------------------------------------------------------------
    If gbShow Then
        With frmClient.lstLog
            If .ListCount = giLIST_BOX_MAX Then .Clear
            .AddItem s, 0
        End With
    End If
End Sub

Public Sub DisplayStatus(s As String)
    '-------------------------------------------------------------------------
    'Purpose:   If gbShow is true, displays passed string on forms status box
    'Assumes:   If gbShow is true, form is loaded and visible
    '-------------------------------------------------------------------------
    If gbShow Then
        AlignTextToBottom frmClient.lblStatus, s
    End If
End Sub


'Puts a new log record into the private log array and updates the listbox
'if the the UI is visible.  The logs will besent to the manager later.
Public Sub AddLogRecord(sServiceID As String, sComment As String, lMilliseconds As Long, bIgnoreThreshod As Boolean)
    '-------------------------------------------------------------------------
    'Purpose:   Called to add a record to the gaLog.
    'In:        [sServiceID]    Service ID that will be added
    '           [sComment]      Comment that will be added
    '           [lMilliseconds] Milliseconds that will be added
    '           [bIgnoreThreshold]
    '                           If true, procedure ignores the Threshold property
    '                           It will not write the records to a file and
    '                           remove them from the array
    'Effects:   [gaLog]         May be redimensioned (preserve) to increase
    '                           its size
    '           [glLastAddedRecord]
    '                           will be increased by one
    '-------------------------------------------------------------------------
    Dim lU As Long      'Ubound of array
    Dim lIndex As Long  'array index to put records in

    On Error GoTo AddLogRecordError
AddLogRecordTop:
        
    'Check if the array needs dimensioned
    If glLastAddedRecord = giNO_RECORD Then
        ReDim gaLog(giLOG_ARRAY_DIMENSION_ONE, giREDIM_CHUNK_SIZE)
        glLastAddedRecord = 0
        lIndex = glLastAddedRecord
    Else
        lU = UBound(gaLog, 2)
        glLastAddedRecord = glLastAddedRecord + 1
        lIndex = glLastAddedRecord
        If glLastAddedRecord > lU Then
            'Redim gaRecords to increase size
            lU = lU + giREDIM_CHUNK_SIZE
            ReDim Preserve gaLog(giLOG_ARRAY_DIMENSION_ONE, lU)
        End If
    End If
    gaLog(giCOMPONENT_ELEMENT, lIndex) = LoadResString(giCOMPONENT_NAME) & Str$(glClientID)
    gaLog(giSERVICE_ELEMENT, lIndex) = sServiceID
    gaLog(giCOMMENT_ELEMENT, lIndex) = sComment
    gaLog(giMILLI_SECONDS_ELEMENT, lIndex) = lMilliseconds
    If Not bIgnoreThreshod And glLogThresholdRecs > 0 And glLogThresholdRecs = glLastAddedRecord Then
       'Write the log file
       WriteLog
    End If
    #If ccShowList Then
        DisplayString sServiceID & gsSEPERATOR & sComment: DoEvents
    #End If
    Exit Sub
AddLogRecordError:
    Select Case Err.Number
        Case ERR_SUBSCRIPT_OUT_OF_RANGE
            'Synchronicity issues caused this
            'Got the glLastAddedRecord write before it got changed
            'but tried to put record in array right after it got redim'ed
            Dim bTried
            'If already tried raise error
            If bTried Then Err.Raise Err.Number, Err.Source, Err.Description
            bTried = True
            'Try the at the top again, getting a new glLastAddedRecord
            GoTo AddLogRecordTop
        Case Else
            DisplayStatus Err.Description
            Exit Sub
    End Select
End Sub

Public Sub LogError(ByVal oErr As ErrObject)
    '-------------------------------------------------------------------------
    'Purpose:   Display error description on forms Status box if the form is
    '           visible
    'In:        [oErr]
    '               Valid error object
    '-------------------------------------------------------------------------
    Dim s As String
    s = LoadResString(giERROR_PREFIX) & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description
    AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
    DisplayStatus oErr.Description
End Sub

Function GetValueFromRange(udtRangeData As RANDOM_DATA_GROUP, bRandomValueRequired As Boolean) As Long
    Dim lReturn As Long
    
    With udtRangeData
        If .Random Then
            Randomize
            lReturn = CLng((.UpperValue - .LowerValue + 1) * Rnd + .LowerValue)
        Else
            lReturn = .SpecificValue
        End If
        If Not bRandomValueRequired Then bRandomValueRequired = .Random
    End With
    GetValueFromRange = lReturn
End Function

Function GetServiceCommand(bRandomCommandRequired As Boolean) As String
    Dim sSendCommand As String
    Dim iRandom As Integer
    
    bRandomCommandRequired = False
    'Get ServiceCommand to use
    If gbUseDefaultService Then
        sSendCommand = gsSERVICE_LIB_CLASS & "." & giServiceTask
    Else
        sSendCommand = gsServiceCommand
    End If
    GetServiceCommand = sSendCommand
End Function


Function GetTestData(bSendSomething As Boolean, bReceiveSomething As Boolean, vSendData As Variant) As Boolean
    Dim s As String
    Dim i As Integer
    Dim lSendNumRows As Long
    Dim lSendRowSize As Long
    Dim lReceiveNumRows As Long
    Dim lReceiveRowSize As Long
    Dim cData As Collection
    Dim aData() As Variant
    Dim lSendContainerType As Long
    Dim lReceiveContainerType As Long
    Dim bRandomDataRequired As Boolean
    Dim lTaskDuration As Long, lSleepPeriod As Long
    
    lReceiveContainerType = glReceiveContainerType
    lSendContainerType = glSendContainerType
    
    'Get Data that will be worked with
    lSendNumRows = GetValueFromRange(gudtSendNumRows, bRandomDataRequired)
    lSendRowSize = GetValueFromRange(gudtSendRowSize, bRandomDataRequired)
    lReceiveNumRows = GetValueFromRange(gudtReceiveNumRows, bRandomDataRequired)
    lReceiveRowSize = GetValueFromRange(gudtReceiveRowSize, bRandomDataRequired)
    lTaskDuration = GetValueFromRange(gudtTaskDuration, bRandomDataRequired)
    lSleepPeriod = GetValueFromRange(gudtSleepPeriod, bRandomDataRequired)
    
    'Check if we are sending or receiving any data
    'Clear the data structures
    bSendSomething = True
    bReceiveSomething = False
 
    Set cData = New Collection
    ReDim aData(0) As Variant
    'Anything to send to the Service?
    If (lSendNumRows = 0 Or lSendRowSize = 0) And (lReceiveNumRows = 0 Or lReceiveRowSize = 0) And _
            lTaskDuration = 0 And lSleepPeriod = 0 Then
        'Nothing to send to the Service
        ' bSendSomething = False    We need to send the service configuration information with each call
    Else
        bSendSomething = True
        'Fill the data class send data for passing to the Service
        s = Space(lSendRowSize)
        Select Case lSendContainerType
            Case giCONTAINER_TYPE_VARRAY
                ReDim Preserve aData(giRECORD_DATA_BEGIN + lSendNumRows - 1) As Variant
                For i = giRECORD_DATA_BEGIN To giRECORD_DATA_BEGIN + lSendNumRows - 1
                    aData(i) = s
                Next i
            Case giCONTAINER_TYPE_VCOLLECTION
                For i = 1 To lSendNumRows
                    cData.Add s
                Next i
         End Select
    End If
    'Anything to receive back from the Service?
    If (lReceiveNumRows = 0 Or lReceiveRowSize = 0 Or lReceiveContainerType = giCONTAINER_TYPE_NULL) Then
        bReceiveSomething = False
        lReceiveNumRows = 0
        lReceiveRowSize = 0
        lReceiveContainerType = giCONTAINER_TYPE_NULL
    Else
        bReceiveSomething = True
    End If
    'Some data may actually be sent if something is expected back or a
    'Milliseconds to be used is specified, but only enough data to instruct
    'the Service on what to do.
    If bReceiveSomething Or bSendSomething Then
        'Fill the global data class receive parameters for passing to the Service
        Select Case lSendContainerType
        Case giCONTAINER_TYPE_VARRAY
            'Make sure we have records in our array to fill
            If UBound(aData) < giRECORD_DATA_BEGIN - 1 Then
                ReDim aData(giRECORD_DATA_BEGIN - 1) As Variant
            End If
            aData(giRECORD_NUMROWS) = lReceiveNumRows
            aData(giRECORD_ROWSIZE) = lReceiveRowSize
            aData(giRECORD_TASK_DURATION) = lTaskDuration
            aData(giRECORD_SLEEP_PERIOD) = lSleepPeriod
            aData(giRECORD_CONTAINER_TYPE) = lReceiveContainerType
            aData(giRECORD_DATABASE_QUERY) = gsDatabaseQuery
            aData(giRECORD_SERVICE_CONFIGURATION) = gvServiceConfiguration
        Case giCONTAINER_TYPE_VCOLLECTION
            cData.Add lReceiveNumRows, CStr(giRECORD_NUMROWS)
            cData.Add lReceiveRowSize, CStr(giRECORD_ROWSIZE)
            cData.Add lTaskDuration, CStr(giRECORD_TASK_DURATION)
            cData.Add lSleepPeriod, CStr(giRECORD_SLEEP_PERIOD)
            cData.Add lReceiveContainerType, CStr(giRECORD_CONTAINER_TYPE)
            cData.Add gsDatabaseQuery, CStr(giRECORD_DATABASE_QUERY)
            cData.Add gvServiceConfiguration, CStr(giRECORD_SERVICE_CONFIGURATION)
        End Select
    End If
    
    'Set return value and out parameters
    Select Case lSendContainerType
    Case giCONTAINER_TYPE_VARRAY
        vSendData = aData()
    Case giCONTAINER_TYPE_VCOLLECTION
        Set vSendData = cData
    End Select
    GetTestData = bRandomDataRequired
End Function

Sub ConfigureTest()
    '-------------------------------------------------------------------------
    'Purpose:   Configure the Client to run a test according to its current
    '           properties.
    'Effects:   U/I is reset for a new test
    '           Remote Connection settings are made useing RacReg
    '   [glCallsMade]
    '           becomes 0
    '   [glCallsReturned]
    '           becomes 0
    '   [gbTestInProcess]
    '           becomes true
    '   [gbStopping]
    '           becomes false
    '   [gcServices]
    '           is destroyed and reinstanciated
    '   [goTestTool]
    '           is instanciated with the correct class having a RunTest method
    'Assumption:
    '           A test is not already in process
    '-------------------------------------------------------------------------
    'Configure test mode and connection settings
    Dim iResult As Integer
    
    'Set the global status flags
    'If there is reentry by a timer click exit sub
    If gbTestInProcess Then Exit Sub
    gbTestInProcess = True
    'Clear the Services collection
    Set gcServices = Nothing
    Set gcServices = New Collection
    'Set global variables
    glCallsMade = 0
    glCallsReturned = 0
    'Display the stautus defaults
    If gbShow Then
        With frmClient
            .lblCallsMade.Caption = 0
            .lblCallsReturned.Caption = 0
            .lblCallsMade.Refresh
            .lblCallsReturned.Refresh
        End With
    End If
    'Set the connection settings for AEWorker.Worker, AEQueueMgr.Queue, AEPoolMgr.Pool
    With goRegClass
        If gbConnectionRemote Then
            If gbConnectionNetOLE Then
                iResult = .SetNetOLEServerSettings(True, "AEQueueMgr.Queue", , gsConnectionAddress)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
                iResult = .SetNetOLEServerSettings(True, "AEWorker.Worker", , gsConnectionAddress)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
                iResult = .SetNetOLEServerSettings(True, "AEPoolMgr.Pool", , gsConnectionAddress)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
            Else
                iResult = .SetAutoServerSettings(True, "AEQueueMgr.Queue", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
                iResult = .SetAutoServerSettings(True, "AEWorker.Worker", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
                iResult = .SetAutoServerSettings(True, "AEPoolMgr.Pool", , gsConnectionAddress, gsConnectionProtocol, glConnectionAuthentication)
                If iResult <> 0 Then GoTo ConfigureTest_RacRegError
            End If
        Else
            iResult = .SetAutoServerSettings(False, "AEQueueMgr.Queue")
            If iResult <> 0 Then GoTo ConfigureTest_RacRegError
            iResult = .SetAutoServerSettings(False, "AEWorker.Worker")
            If iResult <> 0 Then GoTo ConfigureTest_RacRegError
            iResult = .SetAutoServerSettings(False, "AEPoolMgr.Pool")
            If iResult <> 0 Then GoTo ConfigureTest_RacRegError
        End If
    End With
    'Check our mode and create instances of the correct objects.
    Select Case glModel
        Case giMODEL_QUEUE
            Set goTestTool = New clsQueueTestTool
        Case giMODEL_DIRECT
            Set goTestTool = New clsDirectTestTool
        Case giMODEL_POOL
            Set goTestTool = New clsPoolTestTool
    End Select
    Exit Sub
ConfigureTest_RacRegError:
    Err.Raise giCONNECTION_SETTING_FAILED, , ReplaceString(LoadResString(giCONNECTION_SETTING_FAILED), gsNAME_TOKEN, LoadResString(giRACREG_ERROR_CODE_OFFSET + iResult))
End Sub

Sub StopOnError(sMessage As String)
    '-------------------------------------------------------------------------
    'Purpose:   Stop current test immediately
    'Effects:
    '           Calls goExplorer.Done
    '   [glLastCallbackTick]
    '           becomes value of GetTickCount
    '   [goTestTool]    is destroyed
    '   [gcServices]    is destroyed
    '   [goExplorer]    is destroyed
    '   [gbTestInProcess]
    '           becomes false
    '-------------------------------------------------------------------------
    On Error GoTo StopOnError_Error
    glLastCallbackTick = GetTickCount()
    gbRunning = False
    gbStopping = True   'This flags will cause callbacks to be ignored
    If gbLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    goExplorer.Done ape_ctClient, sMessage
    Set goTestTool = Nothing
    Set gcServices = Nothing
    gbTestInProcess = False
    Exit Sub
StopOnError_Error:
    If gbLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giSERVICES_POSTED), GetTickCount(), False
    LogError Err
    Resume Next
End Sub

Public Sub CallBackHandler(sServiceID As String, vServiceReturn As Variant, sServiceError As String)
    '-------------------------------------------------------------------------
    'Purpose:   Called by clsCallback Callback method or .
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
    '           May call CompleteTest procedure if all ServiceRequest have been returned
    '   [glCallsReturned]
    '           Increments by one
    '   [gcServices]
    '           Removes respective item
    '-------------------------------------------------------------------------
    Dim lTicks As Long                      'Milliseconds
    Dim oClientService As clsClientService  'Object storing Service Request information
                                            'one will be removed from gcServices
    Dim s As String
    
    On Error GoTo CallBackHandlerError
    'Grab the tics, keep a global copy of the last callback tick count for statistics.
    glLastCallbackTick = GetTickCount()
    'Exit sub if Stopping test
    If gbStopping Then Exit Sub
    'Lookup the Service
    If IsNumeric(sServiceID) Then
       'This is a valid Service.
        'Look up the ID in our collection.
        Set oClientService = gcServices.Item(sServiceID)
        'No error.  This Service is in our Service collection
        'Increment the CallsReturned global
        glCallsReturned = glCallsReturned + 1
        If gbShow Then
            With frmClient.lblCallsReturned
                .Caption = glCallsReturned
                .Refresh
            End With
        End If
        If gbLog Then AddLogRecord sServiceID, LoadResString(giCALLBACK_RECEIVED), glLastCallbackTick, False
        'Remove the Service from the collection
        gcServices.Remove (sServiceID)
    End If
    If Len(sServiceError) > 0 Then
        'It's an error message.  Log it.
        'And abort test
        s = LoadResString(giCALLBACK_ERROR_RECEIVED) & gsSEPERATOR & sServiceError
        If gbLog Then AddLogRecord sServiceID, s, lTicks, False
        StopOnError s
        
    End If
    'Are we through with the test yet?
    Dim bDone As Boolean
    Select Case giTestDurationMode
        Case giTEST_DURATION_CALLS
            bDone = (glCallsReturned = glNumberOfCalls)
        Case giTEST_DURATION_TICKS
            bDone = (glCallsReturned = glCallsMade) And Not gbRunning
        Case Else
            bDone = False
    End Select
    If bDone Then
        'All Services have been queud and callbacks received.
        If gbLog Then AddLogRecord gsNULL_SERVICE_ID, LoadResString(giCALLBACKS_COMPLETE), GetTickCount(), False
        'Release the Explorer before running CompleteTest
        gbRunCompleteProcedure = True
        frmClient.tmrStartTest.Enabled = True
    End If
    Exit Sub
CallBackHandlerError:
    Select Case Err.Number
        Case ERR_INVALID_PROCEDURE_CALL
            'The ServiceID was not found in the Services collection.
            LogError Err
        Case ERR_OVER_FLOW
            s = CStr(Err.Number) & gsSEPERATOR & Err.Source & gsSEPERATOR & Err.Description
            glCallsReturned = 0
            DisplayStatus Err.Description
            AddLogRecord gsNULL_SERVICE_ID, s, GetTickCount(), False
        Case Else
            'Do not raise an error back to the expediter
            LogError Err
    End Select
    Exit Sub
End Sub

