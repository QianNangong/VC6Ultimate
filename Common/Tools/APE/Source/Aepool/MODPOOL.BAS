Attribute VB_Name = "modPoolMgr"
Option Explicit
'Declarations
Declare Function GetTickCount Lib "Kernel32" () As Long

'Constants
Public Const gbSHOW_FORM_DEFAULT As Boolean = False
Public Const gbLOG_DEFAULT As Boolean = False
Public Const glWORKER_QUANTITY_DEFAULT As Long = 1
Public Const glLIST_BOX_MAX As Long = 500
Public Const giMAX_ALLOWED_RETRIES = 500
Public Const giRETRY_WAIT_MIN  As Integer = 500       'Retry Wait is measure in DoEvent cycles
Public Const giRETRY_WAIT_MAX  As Integer = 2500
Public Const gsPROTOCOL_DEFAULT As String = "ncacn_ip_tcp"
Public Const glAUTHENTICATION_DEFAULT As Long = 1
Public Const giWORKER_QUANTITY_DEFAULT As Integer = 1
Public Const gbWORKER_EARLYBIND_DEFAULT As Integer = True
Public Const giERROR_THRESHOLD As Integer = 32700
Public Const glMAX_LONG As Long = 2147483647

'User Defined Errors which also serve as string
'resource indexes
Public Const giNO_WORKERS_CREATED As Integer = 32764
Public Const giINVALID_PARAMETER As Integer = 32765
Public Const giCONNECTION_SETTING_FAILED As Integer = 32750     'An error was returned by RacReg32

'String resourse keys for logging messages
Public Const giPOOL_NAME As Integer = 2
Public Const giGET_WORKER As Integer = 3
Public Const giRELEASE_WORKER As Integer = 4
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

'String resource keys for Form captions
Public Const giLBL_SATISFIED As Integer = 50
Public Const giLBL_REJECTED As Integer = 51
Public Const giLBL_NUM_WORKERS As Integer = 52
Public Const giPOOLMGR_CAPTION As Integer = 53

Public Const giRACREG_ERROR_CODE_OFFSET = 200               'Add offset to racreg32 error codes
                                                            'to make corresponding resource string key

'Public variables
Public gcWorkers As Collection              'This is basically the pool of available workers
Public gcWorkerMachines As Collection        'Collection of clsWorkerMachines objects used
                                             'keep track of how many worker objects are on
                                             'each remote worker machine.
Public glInstances As Long                  'A count of the number of instances made of PoolMgr
Public gbShow As Boolean                    'If true show PoolMgr form
Public gbLog As Boolean                     'If True log PoolMgr Events
Public goLogger As APEInterfaces.ILogger
Public gbLogWorkers As Boolean      'Flag to track status of
                                    'Worker property Log
Public gbUnloading As Boolean       'Flag used by Class_terminate
Public giWorkerCount As Integer              'Number of Worker instanciated, This can be different
                                             'than gcWorkers.Count if a Worker in the collection
                                             'is marked for removal it will not be included in giWorkerCount
Public gbPersistentServices As Boolean      'Flag keeps track of Worker
                                            'property PersistentServices
                                            'If true Workers keep reference to
                                            'all Service objects used else they
                                            'drop references after each use.
Public gbEarlyBindServices As Boolean       'Flag to track status of
                                            'Worker property EarlyBound
Public gbStopTest As Boolean        'Stop Test flag, checked by many procedures
                                    'that will discontinue their processes if true
Public gsProtocol As String                 'Protocol sequence to use when connecting to Workers
Public glAuthentication As Long             'Authentication level to use when connecting to Workers
Public gbUseDCOM As Boolean                 'If true use DCOM to create workers instead of Remote Automation
Public glRequestsSatisfied As Long
Public glRequestsRejected As Long

Public Sub CountInitialize()
    '-------------------------------------------------------------------------
    'Purpose:   Keep track of number instances of PoolMgr and Pool
    '           To be called by a public creatable class in its initialize
    '           event. To keep track of how many public creatable objects
    '           are initialized.  Initialize the PoolMgr application if
    '           this is the first time it is called.
    'Effects:
    '           If this is the first instanciation
    '           Put the PoolMgr in a "Ready" state.  Load  Workers
    '           Set default properties, Show form and load logger if necessary.
    '   [glInstances]
    '           increments by one
    '-------------------------------------------------------------------------
    Dim i As Integer
    Dim oWork As clsWorker                   'Object storing Workers and related informantion
    Dim oWorkerMachine As clsWorkerMachines  'Object that stores how many
                                             'Workers are on what machines
    Dim sReturn As String           'Return of SetWorkersOnMachine function
    
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
        'Create Logger class object early so
        'potential errors could be logged
        If gbLog Then Set goLogger = CreateObject("AELogger.Logger")
        'Create collection objects
        Set gcWorkers = New Collection
        Set gcWorkerMachines = New Collection
        'Add an item to represent number of workers on the local machine
        Set oWorkerMachine = New clsWorkerMachines
        gcWorkerMachines.Add oWorkerMachine
        'Load the default amount of workers and add
        'them to the gcWorkers Collection
        sReturn = SetWorkersOnMachine(False, "", giWORKER_QUANTITY_DEFAULT)
        
        'Only show the form if gbShow is true
        If gbShow Then
            With frmPoolMgr
                .Show
                .lblStatus.Caption = ""
                .lblWorkers.Caption = CStr(giWorkerCount)
                .lblSatisfied.Caption = 0
                .lblRejected.Caption = 0
            End With
        End If
        gbUnloading = False
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
            Set goLogger = oInstancer.Object("AELogger.Logger")
            Set oInstancer = Nothing
            Resume Next
        Case Else
            LogError Err
            Resume Next
    End Select
End Sub

Public Sub CountTerminate()
    '-------------------------------------------------------------------------
    'Purpose:   Keep track of number instances of PoolMgr and Pool
    '           To be called by a public creatable class in its terminate
    '           event. To keep track of how many public creatable objects
    '           are initialized.  Terminate the PoolMgr application if
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
            For Each oWorker In gcWorkers
                Set oWorker.Worker = Nothing
                Set oWorker = Nothing
            Next
            Set goLogger = Nothing
            Set gcWorkers = Nothing
            Set gcWorkerMachines = Nothing
            giWorkerCount = 0
            Unload frmPoolMgr
        End If
    End If
    Exit Sub
Class_TerminateError:
    LogError Err
    Resume Next
End Sub

Public Sub LogEvent(intMessage As Integer)
    'Receives Message key which is used to look
    'up a resource string.  The logrecord is sent to the
    'Logger object if gbLog is true
    On Error GoTo LogEventError
    If gbLog Then
        goLogger.Record LoadResString(giPOOL_NAME), 0, LoadResString(intMessage), GetTickCount()
    End If
    #If ccShowList Then
        'If the form is visible display log on form
        If gbShow Then
            DisplayString "0" & gsSEPERATOR & LoadResString(intMessage)
        End If
    #End If
    Exit Sub
LogEventError:
    LogError Err
    Exit Sub
End Sub

Public Sub LogError(ByVal oErr As ErrObject)
    'Display error on form with no user input required
    'Log error if logging is on
    Dim s As String
    s = LoadResString(giERROR_PREFIX) & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description
    #If ccShowList Then
        If Not gbShow Then
            frmPoolMgr.Show
            gbShow = True
        End If
        DisplayString s
    #Else
        DisplayStatus s
    #End If
    If gbLog And glInstances <> 0 Then
        goLogger.Record LoadResString(giPOOL_NAME), 0, LoadResString(giERROR_PREFIX) & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description, GetTickCount()
    End If
End Sub

Sub DisplayStatus(sText As String)
    If gbShow Then AlignTextToBottom frmPoolMgr.lblStatus, sText
End Sub

Sub DisplayString(sText As String)
    #If ccShowList Then
        'Controls the length of the list box
        'and sets ListIndex
        Dim lstLog As ListBox
        If gbShow Then
            Set lstLog = frmPoolMgr.lstLog
            If lstLog.ListCount = glLIST_BOX_MAX Then lstLog.Clear
            lstLog.AddItem sText, 0
        End If
    #End If
End Sub

Sub Main()

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
    Dim oWorkerProvider As APEInterfaces.IWorkerProvider  'Server that can be instanciated on remote
                                                            'machines to provide Worker objects
    Dim lWorkerToRemove As Long             'ID of Worker found to remove
    Dim oWork As clsWorker                  'clsWorker object that hold reference to a Worker
                                            'and information related to it
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
                Debug.Assert .WorkerKeys.Count = gcWorkers.Count
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
                lAdd = gcWorkers.Count + 1
                Set oWork = New clsWorker
                oWork.Busy = False
                oWork.ID = lAdd
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
        With frmPoolMgr.lblWorkers
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
                LogEvent giCALL_REJECTED_RETRY
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
            s = ReplaceString(LoadResString(giUSING_NO_AUTHENTICATION), gsNAME_TOKEN, sMachineName)
            LogText s
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
            LogError Err
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

Public Sub LogText(sMsg As String)
    '-------------------------------------------------------------------------
    'Purpose:   Passes that passed string as a log record
    '           to the logger
    'In:        [sMsg]
    '               String to be logged
    'Assumption:
    '           If gbLog is true then goLogger is a valid reference to
    '           AELogger.Logger class object
    '-------------------------------------------------------------------------
    On Error GoTo LogTextError
    If gbLog And Not gbStopTest Then
        goLogger.Record LoadResString(giPOOL_NAME), 0, sMsg, GetTickCount()
    End If
    'If the form is visible display log on form
    #If ccShowList Then
        DisplayString sMsg
    #End If
    Exit Sub
LogTextError:
    LogError Err
    Exit Sub
End Sub

