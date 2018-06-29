Attribute VB_Name = "modQueueMgr"
Option Explicit
'-------------------------------------------------------------------------
'The project is the QueueMgr component of the Application Performance Explorer
'The QueueManager receives Service Requests from Client applications and
'places the requests in a Queue.  When it receives a request it passes
'a received callback object to the Expediter if needed.  Workers poll the
'QueueMgr taking Service Requests and accomplishing the service.  When
'the Worker takes a Service request it is removed from the Queue.
'The Queue Manager creates the Worker(s), the Logger, and the Expediter
'
'Key Files:
'   frmQueue.frm    Is the only and main form of the app
'   clsPosFm.cls    Is a tool to save the forms position to the registry
'   clsServc.cls    Is a class used to store Service Request Data
'   clsWorkr.cls    Is a class used to store a Worker object and its related
'                   data
'   QueueMgr.cls    Is a creatable multi-use class that provides the
'                   OLE interface for the APE Manager to call
'   clsQueDl.cls    Is a non-creatable public class that is instanciated and
'                   passed to the Workers and expediter for them to call
'   Queue.cls       Is a creatable multi-use class that provides the OLE
'                   interface for client applications to add service
'                   requests to the Queue
'-------------------------------------------------------------------------

'Declarations
Declare Function GetTickCount Lib "Kernel32" () As Long

'U/I Caption ResourceString keys
Public Const giFORM_CAPTI0N As Integer = 101
Public Const giCURRENT_QUEUE_CAPTION As Integer = 102
Public Const giPEAK_QUEUE_CAPTION As Integer = 103
Public Const giTOTAL_CALL_CAPTION As Integer = 104
Public Const giWORKER_COUNT_CAPTION As Integer = 105

'Constants
Public Const gbSHOW_FORM_DEFAULT As Boolean = False
Public Const gbLOG_DEFAULT As Boolean = False
Public Const gsPROTOCOL_DEFAULT As String = "ncacn_ip_tcp"
Public Const glAUTHENTICATION_DEFAULT As Long = 1
Public Const giWORKER_QUANTITY_DEFAULT As Integer = 1
Public Const gbWORKER_EARLYBIND_DEFAULT As Integer = True
Public Const gbPERSISTENT_QUEUE_DEFAULT As Boolean = False
Public Const glMAX_QUEUE_SIZE_DEFAULT As Long = 20000   'This was chosen as the ideal MaxQueue size on
                                                        'on a Pentium 100 with 32 meg, running NT4
                                                        'This allows the queue to get large enough for
                                                        'the user to see a performance hit, but not so
                                                        'large that recovery is difficult
Public Const giERROR_THRESHOLD As Integer = 32700
Public Const glMAX_ID As Long = 2147483647
Public Const giMAX_WORKERS As Integer = 30
Public Const giMAX_ALLOWED_RETRIES  As Integer = 500
Public Const giRETRY_WAIT_MIN  As Integer = 500       'Retry Wait is measure in DoEvent cycles
Public Const giRETRY_WAIT_MAX  As Integer = 2500
Public Const giRESULT_ARRAY_REDIM_CHUNK_SIZE = 20
Public Const giRESULT_ARRAY_MAX_SIZE = 200

Public Const giRACREG_ERROR_CODE_OFFSET = 200               'Add offset to racreg32 error codes
                                                            'to make corresponding resource string key

'Status codes for Status property of clsService
Public Const giCLIENT_IS_ADDING  As Integer = 0     'Client is currently in the Add method for the
                                                    'respective Service reaquest.  The request should
                                                    'not be delegated yet.
Public Const giWAITING_FOR_WORKER As Integer = 1    'Service request is ready to be taken by worker
Public Const giDELEGATED_TO_WORKER  As Integer = 2  'Worker is processing this service request
Public Const giHAVE_SERVICE_RESULTS As Integer = 3  'Worker has returned results for this Service
                                                    'request.  It is ready to be taken by Expediter

'User Defined Errors which also serve as string
'resource indexes
Public Const giQUEUE_MGR_IS_BUSY As Integer = 32749
Public Const giFIRST_GET_WITHEVENTS_OBJECT As Integer = 32763
Public Const giNO_WORKERS_CREATED As Integer = 32764
Public Const giINVALID_PARAMETER As Integer = 32765
Public Const giINVALID_CALLBACK As Integer = 32766
Public Const giCOULD_NOT_CREATE_EXPEDITER As Integer = 32762
Public Const giCONNECTION_SETTING_FAILED As Integer = 32750     'An error was returned by RacReg32

'String resourse strings for logging messages
Public Const giQUEUE_NAME As Integer = 2
Public Const giADD_RECEIVED As Integer = 3
Public Const giGETREQUEST_RECEIVED_NEW_SERVICE As Integer = 4
Public Const giGETREQUEST_RECEIVED_RETURNED_RESULTS As Integer = 5
Public Const giGETRESULTS_RECEIVED_RETURNED_RESULTS As Integer = 6

Public Const giSTOP_TEST_RECEIVED As Integer = 10
Public Const giCALL_REJECTED_RETRY As Integer = 11
Public Const giUSING_NO_AUTHENTICATION As Integer = 12
Public Const giONLY_N_WORKERS_CREATED As Integer = 13
Public Const giCOULD_NOT_CREATE_WORKER_ON_MACHINE As Integer = 14
Public Const giALL_WORKERS_CREATED As Integer = 15
Public Const giCOULD_NOT_CREATE_LOCAL_WORKER As Integer = 16
Public Const giERROR_PREFIX As Integer = 17

Public Const giFONT_CHARSET_INDEX As Integer = 30
Public Const giFONT_NAME_INDEX As Integer = 31
Public Const giFONT_SIZE_INDEX  As Integer = 32

'Global variables
Public glMaxQueueSize As Long  'Maximum allowed size of gcQueue
Public glAddCallCount As Long  'Total calls made to Queue.Add
Public glPeakQueueSize As Long 'Largest size of the collection of Service requests
Public gbLog As Boolean        'If True log QueueMgr Events
Public goExpediter As APEInterfaces.IExpediter  'Expediter class object
Public gcQueue As Collection                 'Collection of clsService class objects
                                             'which contain a data structure for holding
                                             'Service request.
Public gcWorkers As Collection               'Collection of clsWorker class objects
Public gcWorkerMachines As Collection        'Collection of clsWorkerMachines objects used
                                             'keep track of how many worker objects are on
                                             'each remote worker machine.
Public goLogger As APEInterfaces.ILogger           'Logger object
                                             
Public gbShow As Boolean                     'If True show frmQueueMgr
Public glInstances As Long                   'Count of number of instances
                                             'of this class
Public giWorkerCount As Integer              'Number of Worker instanciated, This can be different
                                             'than gcWorkers.Count if a Worker in the collection
                                             'is marked for removal it will not be included in giWorkerCount
Public gbLogWorkers As Boolean      'Flag to track status of
                                    'Worker property Log
Public gbPersistentServices As Boolean      'Flag keeps track of Worker
                                            'property PersistentServices
                                            'If true Workers keep reference to
                                            'all Service objects used else they
                                            'drop references after each use.
Public gbEarlyBindServices As Boolean       'Flag to track status of
                                            'Worker property EarlyBound
Public gbShowExpediter As Boolean   'Stores current Expediter property Show
Public gbLogExpediter As Boolean    'Stores current Expediter property Log
Public gbStopTest As Boolean        'Stop Test flag, checked by many procedures
                                    'that will discontinue their processes if true
Public gbBusyAdding As Boolean      'If true, in Queue.Add method
Public gbBusyGetServiceRequest As Boolean   'If true, in clsQueueDelegator.GetServiceRequest method
Public gbBusyGetServiceResults As Boolean   'If true, in clsQueueDelegator.GetServiceResults method
Public gbUnloading As Boolean               'Flag used by Class_terminate
Public gbHaveServiceResults As Boolean      'If true, there are Service Request results to return
                                            'to the Expediter when it polls
Public gsProtocol As String                 'Protocol sequence to use when connecting to Workers
Public glAuthentication As Long             'Authentication level to use when connecting to Workers
Public gbUseDCOM As Boolean                 'If true use DCOM to create workers instead of Remote Automation
Public gbFailedToCreateExpediter As Boolean

Public Sub CountInitialize()
    '-------------------------------------------------------------------------
    'Purpose:   Keep track of number instances of QueueMgr and Queue
    '           To be called by a public creatable class in its initialize
    '           event. To keep track of how many public creatable objects
    '           are initialized.  Initialize the QueueMgr application if
    '           this is the first time it is called.
    'Effects:
    '           If this is the first instanciation
    '           Put the QueueMgr in a "Ready" state.  Load expediter, and Workers
    '           Set default properties, Show form and load logger if necessary.
    '   [glInstances]
    '           increments by one
    '-------------------------------------------------------------------------
    Dim i As Integer
    Dim oWork As clsWorker                   'Object storing Workers and related informantion
    Dim oService As clsService               'Object storing service requests and results
    Dim oWorkerMachine As clsWorkerMachines  'Object that stores how many
                                             'Workers are on what machines
    Dim sProgID As String           'ProgID trying to be created
                                    'used for error handling
    Dim sReturn As String           'Return of SetWorkersOnMachine function
    Dim bCreatingExpediter As Boolean
    
    On Error GoTo CountInitializeError
    
    glInstances = glInstances + 1
    If glInstances = 1 Then
        App.OleServerBusyRaiseError = True
        App.OleServerBusyTimeout = 10000
        'Set default property values
        gbShow = gbSHOW_FORM_DEFAULT
        gbLog = gbLOG_DEFAULT
        gsProtocol = gsPROTOCOL_DEFAULT
        glAuthentication = glAUTHENTICATION_DEFAULT
        gbEarlyBindServices = gbWORKER_EARLYBIND_DEFAULT
        glMaxQueueSize = glMAX_QUEUE_SIZE_DEFAULT
        'Create Logger class object early so
        'potential errors could be logged
        sProgID = "AELogger.Logger"
        If gbLog Then Set goLogger = CreateObject("AELogger.Logger")
        'gbPersistentQueue = gbPERSISTENT_QUEUE_DEFAULT
        'Create Expediter class object
        sProgID = "AEExpediter.Expediter"
        bCreatingExpediter = True
        Set goExpediter = CreateObject("AEExpediter.Expediter")
        Set goExpediter.QueueMgrRef = New clsQueueDelegator
        bCreatingExpediter = False
        'Load frmQueueMgr because it has a timer
        Load frmQueueMgr
        'Create collection objects
        Set gcWorkers = New Collection
        Set gcQueue = New Collection
        Set gcWorkerMachines = New Collection
        'Add an item to represent number of workers on the local machine
        Set oWorkerMachine = New clsWorkerMachines
        gcWorkerMachines.Add oWorkerMachine
        'Load the default amount of workers and add
        'them to the gcWorkers Collection
        sReturn = SetWorkersOnMachine(False, "", giWORKER_QUANTITY_DEFAULT)
        
        'Only show the form if gbShow is true
        If gbShow Then
            frmQueueMgr.Show
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
        gbUnloading = False
        'call start test in the  Expediter so it
        'will start polling the QueueMgr
        goExpediter.StartTest
    End If
    Exit Sub
CountInitializeError:
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
            Select Case sProgID
                Case "AELogger.Logger"
                    Set goLogger = oInstancer.Object("AELogger.Logger")
                Case "AEExpediter.Expediter"
                    Set goExpediter = oInstancer.Object("AEExpediter.Expediter")
            End Select
            Set oInstancer = Nothing
            Resume Next
        Case Else
            If bCreatingExpediter Then gbFailedToCreateExpediter = True
            LogError Err, gsNULL_SERVICE_ID
            Resume Next
    End Select
End Sub

Public Sub CountTerminate()
    '-------------------------------------------------------------------------
    'Purpose:   Keep track of number instances of QueueMgr and Queue
    '           To be called by a public creatable class in its terminate
    '           event. To keep track of how many public creatable objects
    '           are initialized.  Terminate the QueueMgr application if
    '           this is the last time called.
    'Effects:
    '           Unload all objects, and unload form so that this application
    '           will close
    '   [glInstances]
    '           decrements by one
    '-------------------------------------------------------------------------
    Dim oWorker As clsWorker
    On Error GoTo Class_TerminateError
    glInstances = glInstances - 1
    'If already started unloading don't check
    'instance count again
    If Not gbUnloading Then
        If glInstances = 0 Then
            gbUnloading = True
            goExpediter.StopTest
            For Each oWorker In gcWorkers
                oWorker.Worker.ShutDown
            Next
            For Each oWorker In gcWorkers
                Set oWorker.Worker = Nothing
                Set oWorker = Nothing
            Next
            Set goLogger = Nothing
            Set gcWorkers = Nothing
            giWorkerCount = 0
            Set gcWorkerMachines = Nothing
            Set goExpediter = Nothing
            Set gcQueue = Nothing
            Unload frmQueueMgr
        End If
    End If
    Exit Sub
Class_TerminateError:
    LogError Err, gsNULL_SERVICE_ID
    Resume Next
End Sub

Public Sub LogEvent(intMessage As Integer, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   Receives Message key which is used to look
    '           up a resource string.  The logrecord is sent to the
    '           Logger object if gbLog is true
    'In:        [intMessage]
    '               A valid Resource string key for the message to be logged
    '           [sServiceID]
    '               Service Request ID to be logged
    'Assumption:
    '           If gbLog is true then goLogger is a valid reference to
    '           AELogger.Logger class object
    '-------------------------------------------------------------------------
    On Error GoTo LogEventError
    If gbLog And Not gbStopTest Then
        goLogger.Record LoadResString(giQUEUE_NAME), sServiceID, LoadResString(intMessage), GetTickCount()
    End If
    'If the form is visible display log on form
    #If ccShowList Then
        DisplayString sServiceID & gsSEPERATOR & LoadResString(intMessage)
    #End If
    Exit Sub
LogEventError:
    LogError Err, sServiceID
    Exit Sub
End Sub

Public Sub LogText(sMsg As String, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   Passes that passed string and ServiceID as a log record
    '           to the logger
    'In:        [sMsg]
    '               String to be logged
    '           [sServiceID]
    '               Service Request ID to be logged
    'Assumption:
    '           If gbLog is true then goLogger is a valid reference to
    '           AELogger.Logger class object
    '-------------------------------------------------------------------------
    On Error GoTo LogTextError
    If gbLog And Not gbStopTest Then
        goLogger.Record LoadResString(giQUEUE_NAME), sServiceID, sMsg, GetTickCount()
    End If
    'If the form is visible display log on form
    #If ccShowList Then
        DisplayString sServiceID & gsSEPERATOR & sMsg
    #End If
    Exit Sub
LogTextError:
    LogError Err, sServiceID
    Exit Sub
End Sub

Public Sub LogError(ByVal oErr As ErrObject, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   Display error description on forms Status box if the form is
    '           visible; log error if logging is on
    'In:        [oErr]
    '               Valid error object
    '           [sServiceID]
    '               Service Request ID logged with the error message
    'Assumption:
    '           If gbShow is true the form is loaded and visible
    '           If gbLog is true the goLogger is a valid AELogger.Logger class
    '               object
    '-------------------------------------------------------------------------
    
    Dim s As String
    s = LoadResString(giERROR_PREFIX) & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description
    #If ccShowList Then
        If Not gbShow Then
            frmQueueMgr.Show
            gbShow = True
        End If
        DisplayString s
    #Else
        If oErr.Number <> 0 Then DisplayStatus oErr.Description
    #End If
    If gbLog And glInstances <> 0 Then
        goLogger.Record LoadResString(giQUEUE_NAME), sServiceID, s, GetTickCount()
    End If
End Sub

Sub DisplayStatus(s As String)
    '-------------------------------------------------------------------------
    'Purpose:   If gbShow is true, displays passed string on forms status box
    'Assumes:   If gbShow is true, form is loaded and visible
    '-------------------------------------------------------------------------
    If gbShow Then AlignTextToBottom frmQueueMgr.lblStatus, s
End Sub

Sub DisplayString(sText As String)
    '-------------------------------------------------------------------------
    'Purpose:   Adds the passed text to to the list box.  Only used if conditional
    '           compile ccShowList is true.
    'Assumes:   If gbShow is true, form is visible
    '           If ccShowList is true, lstLog is visible and positioned
    '-------------------------------------------------------------------------
    'Controls the length of the list box
    'and sets ListIndex
    #If ccShowList Then
        Dim lstLog As ListBox
        If gbShow Then
            Set lstLog = frmQueueMgr.lstLog
            If lstLog.ListCount = giLIST_BOX_MAX Then lstLog.Clear
            lstLog.AddItem sText, 0
            DoEvents
        End If
    #End If
End Sub

Function gFormatPath(sPath As String) As String
    '-------------------------------------------------------------------------
    'Purpose:   Assures that the passed path has a "\" at the end of it
    'IN:
    '   [sPath]
    '           a valid path name
    'Return:    the same path with a "\" on the end if it did not already
    '           have one.
    '-------------------------------------------------------------------------
    If Right$(sPath, 1) <> "\" Then
        gFormatPath = sPath & "\"
    Else
        gFormatPath = sPath
    End If
End Function

Sub StopQueue()
    '-------------------------------------------------------------------------
    'Purpose:   Stops processing of Service Requests by deleging the pending
    '           requests
    'Assumes:   Assumes that clients have already stopped posting new requests
    '-------------------------------------------------------------------------
    LogEvent giSTOP_TEST_RECEIVED, gsNULL_SERVICE_ID
    DisplayStatus LoadResString(giSTOP_TEST_RECEIVED)
    Set gcQueue = Nothing
    Set gcQueue = New Collection
End Sub

Public Function SetWorkersOnMachine(bRemote As Boolean, sMachineName As String, lQuantityOnMachine As Long) As String
    '-------------------------------------------------------------------------
    'Purpose:   Sets the quantity of instanciated Workers on a particular machine
    'IN:
    '   [bRemote]
    '           If true adjust number of workers on a remote machine; else,
    '           adjust the number on the local machine.
    '   [sMachineName]
    '           Name of machine to adjust the level of instanciated Workers
    '   [lQuantityOnMachine]
    '           Number of Instantiated Workers that should be on specified
    '           machine.
    'Return:    Discription of Errors that should be displayed to user
    'Effects:
    '   [gcWorkers]
    '           The number of Workers in this collection will be adjusted
    '   [gcWorkerMachines]
    '           An item may be added or removed or edited
    '-------------------------------------------------------------------------
    Dim oRacReg As RacReg.RegClass          'Object to set automation connection settings
    Dim oWorkerMachine As clsWorkerMachines 'Object that stores how many workers are on
                                            'a machine, retrieved from global collection
    Dim lWorkerToRemove As Long             'ID of Worker found to remove
    Dim oWork As clsWorker                  'clsWorker object that hold reference to a Worker
                                            'and information related to it
    Dim oWorkerProvider As APEInterfaces.IWorkerProvider  'Server that can be instanciated on remote
                                                            'machines to provide Worker objects
    Dim lAdd As Long                        'New ID for New Worker
    Dim sErrors As String                   'Discription of Errors that will be returned
    Dim bAddingWorker As Boolean            'If true, adding and configuring worker
                                            'used by error handling
    Dim iRetry As Integer                   'Error retry counter
    Dim iResult As Integer                  'RacReg error code
    
    On Error GoTo SetWorkersOnMachineError
    
    'Validate lQuantityOnMachine
    If lQuantityOnMachine < 0 Then lQuantityOnMachine = 0
    
    'Set registry for local or remote machine name
    Set oRacReg = New RacReg.RegClass
    If bRemote Then
        If gbUseDCOM Then
            iResult = oRacReg.SetDCOMServerSettings(True, "AEWorkerProvider.WorkerProvider", , sMachineName)
        Else
            iResult = oRacReg.SetAutoServerSettings(True, "AEWorkerProvider.WorkerProvider", , sMachineName, gsProtocol, glAuthentication)
        End If
    Else
        'Make sure the Machine name string is zero length
        sMachineName = ""
        'Make sure AEWorker.Worker is registered for local instanciation
        'Because Clients may have been run on this machine and may have
        'left the connection settings remote if they did not unload properly
        iResult = oRacReg.SetAutoServerSettings(False, "AEWorker.Worker")
        
    End If
    If iResult <> 0 Then GoTo SetWorkersOnMachine_RacRegError
    
    'Get the clsWorkerMachines object to store information in
    If Not bRemote Then
        'it is definitely the first item in the collection
        Set oWorkerMachine = gcWorkerMachines.Item(1)
    Else
        'if it is in the collection it is stored by a key
        'equaling the machine name
        'If error equals ERR_INVALID_PROCEDURE_CALL there
        'are no Workers on specified machine and no clsWorkerMachines
        'class object to represent them
        On Error Resume Next
        Set oWorkerMachine = gcWorkerMachines.Item(sMachineName)
        If Err.Number = ERR_INVALID_PROCEDURE_CALL Then
            On Error GoTo SetWorkersOnMachineError
            'Don't create a new clsWorkerMachine object of
            'lQuantityOnMachine is zero
            If lQuantityOnMachine <= 0 Then Exit Function
            Set oWorkerMachine = New clsWorkerMachines
            'If an error occurs creating WorkerProvider the current machine name
            'can not be used.  Treat error as if a Worker can not be created on
            'paticular machine.
            bAddingWorker = True
            Set oWorkerMachine.WorkerProvider = CreateObject("AEWorkerProvider.WorkerProvider")
            bAddingWorker = False
            gcWorkerMachines.Add oWorkerMachine, sMachineName
            With oWorkerMachine
                .Remote = True
                .MachineName = sMachineName
            End With
        End If
        On Error GoTo SetWorkersOnMachineError
        Set oWorkerProvider = oWorkerMachine.WorkerProvider
    End If
    
    'Now see if more workers need destroyed on this machine
    With oWorkerMachine
        If .WorkerKeys.Count > lQuantityOnMachine Then
            Do Until .WorkerKeys.Count <= lQuantityOnMachine
                'Find a worker on this machine
                lWorkerToRemove = .WorkerKeys(.WorkerKeys.Count)
                .WorkerKeys.Remove .WorkerKeys.Count
                'Remove the found worker
                'Do not destroy the Worker if it is busy
                'instead just flip its RemoveMe flag
                giWorkerCount = giWorkerCount - 1
                If gcWorkers.Item(CStr(lWorkerToRemove)).Busy Then
                    gcWorkers.Item(CStr(lWorkerToRemove)).RemoveMe = True
                Else
                    iRetry = 0
                    Set oWork = gcWorkers.Item(CStr(lWorkerToRemove))
                    oWork.Worker.ShutDown
                    Set gcWorkers.Item(CStr(lWorkerToRemove)).Worker = Nothing
                    gcWorkers.Remove CStr(lWorkerToRemove)
                End If
            Loop
        Else
            'Else lQuantityOnMachine must be greater than .WorkerKeys.count
            'So add to the collection
            bAddingWorker = True
            Do Until .WorkerKeys.Count = lQuantityOnMachine
                'Choose a unique key
                lAdd = GetUniqueID
                Set oWork = New clsWorker
                oWork.Busy = False
                'Get a new Worker object
                If bRemote Then
                    Set oWork.Worker = oWorkerProvider.GetWorker
                Else
                    Set oWork.Worker = CreateObject("AEWorker.Worker")
                End If
                'Set the WorkerID property of AEWorker.Worker
                'Set the new worker property to the properties
                'that have been set for the any other workers
                iRetry = 0
                oWork.Worker.SetProperties gbLogWorkers, gbEarlyBindServices, _
                        gbPersistentServices, lAdd
                'Add the clsWorker class object which holds a
                'reference to the Worker class object to gcWorkers collection
                'Use the WorkerID as the key
                .WorkerKeys.Add lAdd
                gcWorkers.Add oWork, CStr(lAdd)
                giWorkerCount = giWorkerCount + 1
                iRetry = 0
                Set oWork.Worker.QueueMgrRef = New clsQueueDelegator
                oWork.Worker.StartPollingQueue
            Loop
            bAddingWorker = False
        End If
    End With
SetWorkersOnMachineEnd:
    'Update the WorkerCount label in the U/I
    'Set connection settings back to local
    iResult = oRacReg.SetAutoServerSettings(False, "AEWorkerProvider.WorkerProvider")
    If iResult <> 0 Then GoTo SetWorkersOnMachine_RacRegError
    
    If gbShow Then
        With frmQueueMgr.lblWorkerCount
            .Caption = gcWorkers.Count
            .Refresh
        End With
    End If
    
    'If the WorkerKeys.count = 0 and bRemote is true
    'then the clsWorkerMachines class
    'object in gcWorkerMachines should be removed
    'Don't remove the clsWorkerMachines object representing the
    'local machine.  Index one is reserved for the local machine.
    If oWorkerMachine.WorkerKeys.Count = 0 And bRemote Then
        On Error Resume Next
        gcWorkerMachines.Remove sMachineName
    End If
    SetWorkersOnMachine = sErrors
    Exit Function
    
SetWorkersOnMachine_RacRegError:
    Err.Raise giCONNECTION_SETTING_FAILED
    
SetWorkersOnMachineError:
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
                GoTo SetWorkersOnMachineUnexpectedError
            End If
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
            Set oWorkerProvider = oInstancer.Object("AEWorkerProvider.WorkerProvider")
            Set oInstancer = Nothing
            Resume Next
        Case RPC_S_UNKNOWN_AUTHN_TYPE
            'Tried to connect to a server that does not support
            'specified authentication level.  Display message and
            'switch to no authentication and try again
            Dim s As String
            s = sMachineName & gsSEPERATOR & LoadResString(giUSING_NO_AUTHENTICATION)
            LogText s, gsNULL_SERVICE_ID
            sErrors = s & vbCrLf
            iResult = oRacReg.SetAutoServerSettings(True, "AEWorkerProvider.WorkerProvider", , sMachineName, gsProtocol, glAUTHENTICATION_DEFAULT)
            Resume
        Case giCONNECTION_SETTING_FAILED
            sErrors = ReplaceString(LoadResString(giCONNECTION_SETTING_FAILED), gsNAME_TOKEN, LoadResString(giRACREG_ERROR_CODE_OFFSET + iResult))
            Err.Raise giNO_WORKERS_CREATED, , sErrors & vbCrLf & LoadResString(giNO_WORKERS_CREATED)
        Case Else
SetWorkersOnMachineUnexpectedError:
            'There are three cases to respond to if there is an unexpected error
            '1- If the error occured while NOT adding a worker it most likely
            '   occured while removing one.  Resume Next to insure that the worker
            '   is removed from the workers collection.
            '2- If we were adding a worker and the worker class was registered local
            '   log the error, and add it to the sError string, but raise the
            '   giNO_WORKERS_CREATED error, because the system has a critical problem
            '   if a local worker can not be created.
            '3- If we were adding a worker and the worker class was registered remote
            '   log the error, and add it to the sError string.  Exit procedure so
            '   that calling procedure can try creating workers on another machine
            Dim sSource As String
            sSource = Err.Source
            sErrors = sErrors & sMachineName & gsSEPERATOR & sSource & gsSEPERATOR & Err.Description & vbCrLf
            LogError Err, gsNULL_SERVICE_ID
            If Not bAddingWorker Then
                Resume Next
            Else
                If bRemote Then
                    sErrors = sErrors & vbCrLf & ReplaceString(LoadResString(giCOULD_NOT_CREATE_WORKER_ON_MACHINE), gsNAME_TOKEN, sMachineName)
                    Resume SetWorkersOnMachineEnd
                Else
                    iResult = oRacReg.SetAutoServerSettings(False, "AEWorkerProvider.WorkerProvider")
                    sErrors = sErrors & vbCrLf & LoadResString(giCOULD_NOT_CREATE_LOCAL_WORKER)
                    Err.Raise giNO_WORKERS_CREATED, sSource, sErrors & vbCrLf & LoadResString(giNO_WORKERS_CREATED)
                End If
            End If
    End Select
End Function

Private Function GetUniqueID() As Long
' Returns a unique Long value on each call
    Static lID As Long
    lID = lID + 1
    GetUniqueID = lID
End Function
