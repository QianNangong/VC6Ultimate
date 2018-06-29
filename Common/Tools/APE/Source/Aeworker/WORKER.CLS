VERSION 1.0 CLASS
BEGIN
  MultiUse = 0   'False
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Worker"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Worker"
Option Explicit
'-------------------------------------------------------------------------
'The Class is the only public class in this project.  See notes in
'modWorker for purpose.
' It implements the IWorker interface.
'-------------------------------------------------------------------------

Implements APEInterfaces.IWorker

'***********************
'Public Properties
'***********************

Public Property Set IWorker_QueueMgrRef(ByVal oQueueMgr As APEInterfaces.IQueueDelegator)
Attribute IWorker_QueueMgrRef.VB_Description = "Sets the QueueDelegator object that the Worker uses to receive Service Requests and to return Service Request results to the AEQueueMgr."
    '-------------------------------------------------------------------------
    'Purpose:   Called by the the QueueMgr to pass a reference of itself to
    '           the Worker.
    'In:        [oQueueMgr]
    '               A valid reference to a QueueMgr class object
    'Effects:   [goQueueDelegator]
    '               Sets the global object variable equal to the passed reference
    '-------------------------------------------------------------------------
    Set goQueueDelegator = oQueueMgr
    
End Property

Public Property Let IWorker_Log(ByVal bLog As Boolean)
Attribute IWorker_Log.VB_Description = "Determines if the Worker logs its events and errors to the AELogger.Logger object."
    '-------------------------------------------------------------------------
    'Purpose:   If property is true, the Worker worker generates log records
    '           for events and passes the records to the logger.
    'Effects:   [goLogger]
    '-------------------------------------------------------------------------
    gbLog = bLog
End Property

Public Property Get IWorker_Log() As Boolean
    IWorker_Log = gbLog
End Property

Public Property Let IWorker_ID(ByVal lID As Long)
Attribute IWorker_ID.VB_Description = "Returns or Sets the ID used by the AEQueueMgr or AEPoolMgr to manage this Worker."
    '-------------------------------------------------------------------------
    'Purpose:   Called by QueueMgr to give the Worker a unique ID.  This ID
    '           can only be set once.  The Worker must have this ID to poll
    '           the QueueMgr.
    'Effects:
    '   [glWorkerID]
    '           Is set equal to the passed long, if it has not already happened
    '-------------------------------------------------------------------------
    Static stbAlreadySet As Boolean
    If Not stbAlreadySet Then
        glWorkerID = lID
        stbAlreadySet = True
    End If
End Property

Public Property Get IWorker_ID() As Long
    IWorker_ID = glWorkerID
End Property

Public Property Let IWorker_PersistentServices(ByVal bPersistent As Boolean)
Attribute IWorker_PersistentServices.VB_Description = "Determines whether the Worker retains references to service objects that it instantiates or if it releases their references after each use."
    '-------------------------------------------------------------------------
    'Purpose:   If true, the worker keeps reference to
    '           every Service object it has used, else
    '           worker releases Service object after
    '           each use.
    'Effects:
    '   [gcServices]
    '           If property is being changed to false
    '           then set collection of Service Object
    '           references to nothing, if it is change
    '           to true set equal to new collection
    '-------------------------------------------------------------------------
    If gbPersistent <> bPersistent Then
        gbPersistent = bPersistent
        If Not bPersistent Then
            Set gcServices = Nothing
        Else
            Set gcServices = New Collection
        End If
    End If
End Property

Public Property Get IWorker_PersistentServices() As Boolean
    IWorker_PersistentServices = gbPersistent
End Property

Public Property Let IWorker_EarlyBindServices(ByVal bEarlyBind As Boolean)
Attribute IWorker_EarlyBindServices.VB_Description = "Specifies whether service objects should be instantiated as APEInterfaces.Service class objects or as Object class objects.  If true, all service objects must implement the APEInterfaces.Service interface."
    '-------------------------------------------------------------------------
    'Purpose:   If true, the worker uses code that utilizes
    '           early binding.  This option is only
    '           available for Service objects classes that
    '           were developed with the worker class,
    '           because early binding is only available
    '           if class names are hard coded.
    'Effects:
    '   [gbEarlyBind] is made equal to passed Boolean
    '-------------------------------------------------------------------------
    gbEarlyBind = bEarlyBind
End Property

Public Property Get IWorker_EarlyBindServices() As Boolean
    IWorker_EarlyBindServices = gbEarlyBind
End Property

'************************
'Public Methods
'************************
Public Sub IWorker_SetProperties(Optional ByVal bLog As Variant, _
        Optional ByVal bEarlyBindServices As Variant, _
        Optional ByVal bPersistentServices As Variant, _
        Optional ByVal lID As Variant)
Attribute IWorker_SetProperties.VB_Description = "Sets Worker properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   Called by QueueMgr or Client to set properties with one method
    '           call
    'Effects:
    '       [Properties]
    '           Log, EarlyBindServices, ID, PersistentServices, UseQueueMgr
    '-------------------------------------------------------------------------
    With Me
        .IWorker_Log = bLog
        If Not IsMissing(bEarlyBindServices) Then gbEarlyBind = bEarlyBindServices
        If Not IsMissing(lID) Then .IWorker_ID = lID
        If Not IsMissing(bPersistentServices) Then .IWorker_PersistentServices = bPersistentServices
    End With
    ' If a Service object already exists, initialize it because a new test is starting
    If Not goLastServiceUsed Is Nothing Then
        goLastServiceUsed.Initialize gcServiceConfigurations(gsLastLibClassUsed)
    End If
End Sub
        
Public Function IWorker_DoService(ByVal sServiceID As String, ByVal sCommand As String, Optional ByVal vData As Variant) As Variant
Attribute IWorker_DoService.VB_Description = "Receives a Service Request, loads the object needed to fulfill the Service Request, and returns the results."
    '-------------------------------------------------------------------------
    'Purpose:   This method allows a client to accomplish the same tasks
    '           accomplished by the return of a task request when the worker calls
    '           the QueueMgr in PollQueue or CheckService, but uses a synchronous
    '           process instead of asynchronous.
    '           This method is intended to be called directly by a client rather than
    '           the QueueMgr.  It is provided for using a Pool Manager system or
    '           Direct instanciation system
    '
    '           This loads a service object, and may or may not keep it in a collection
    '           for future use.  The service object is called to accomplish the
    '           requested task and then the return results of the service object
    '           are returned directly to the calling client
    'IN:
    '   [sServiceID]
    '           An ID for the requested Service request.  It is only useful for
    '           tracing log records.
    '   [sCommand]
    '           ProgID and Method or Task name in the formate of "Library.Class.Method"
    '           The "Library.Class" is used to load the needed Service object.  The
    '           "Method" string is passed to the Execute method of the service object
    '   [vData]
    '           (Optional) Data passed by the client that gets passed to the Service
    '           object.  This data is not manipulated by the Worker at all.
    'Return:
    '           Variant: this value is obtained by calling the service object.  It is
    '           the results of the task accomplished by the service object.  It is not
    '           manipulated by the Worker at all.
    'Effects:
    '   [goEarlyBoundService]
    '           Set equal to AEService.Service class object if passed ProgID
    '           equals "AEService.Service"
    '   [gsLastCommandUsed]
    '           Set equal to sCommand parameter
    '   [gsLastLibClassUsed]
    '           Set equal to the Library.Class in the sCommand Parameter
    '   [gsLastMethodUsed]
    '           Set equal to the method portion of the passed sCommand parameter
    '   [goLastServiceUsed]
    '           Set equal to the object created using the passed ProgID
    '-------------------------------------------------------------------------
    Dim vReturn As Variant
    Dim bDataPresent As Boolean
    If Not IsMissing(vData) Then bDataPresent = True Else bDataPresent = False
    If Not gsLastCommandUsed = sCommand Or goLastServiceUsed Is Nothing Then
        'Get the library.class from sCommand
        'sCommand is in the format "library.class.method"
        GetLibClassMethod gsLastLibClassUsed, gsLastMethodUsed, sCommand
        ' Cache the service configuration options
        Debug.Assert bDataPresent   ' Service configuration options must be specified
        If (VarType(vData) And vbArray) <> 0 Then
            CacheServiceConfiguration gsLastLibClassUsed, vData(giRECORD_SERVICE_CONFIGURATION)
        Else
            ' If the RPC call is rejected we report a special error message
            On Error Resume Next
            CacheServiceConfiguration gsLastLibClassUsed, vData(CStr(giRECORD_SERVICE_CONFIGURATION))
            Dim lError As Long
            lError = IIf(Err.Number = RPC_E_CALL_REJECTED, giRPC_ERROR_ACCESSING_COLLECTION, Err.Number)
            On Error GoTo 0
            If lError <> 0 Then
                Err.Raise lError
            End If
        End If
        'Get the Service object
        Set goLastServiceUsed = GetServiceObject(gsLastLibClassUsed)
        If gbEarlyBind Then
            Set goEarlyBoundService = goLastServiceUsed
        End If
        gsLastCommandUsed = sCommand
    End If
    
    'Call the execute method of the class object
    'passing the method string and the send data as variant,
    'and the return data as variant by reference
    LogEvent giEXECUTE_BEGIN, sServiceID
    If gbEarlyBind Then
    'Use the Earlybound object reference
        If bDataPresent Then
            goEarlyBoundService.Execute sServiceID, gsLastMethodUsed, vData, vReturn
        Else
            goEarlyBoundService.Execute sServiceID, gsLastMethodUsed
        End If
    Else
        If bDataPresent Then
            goLastServiceUsed.Execute sServiceID, gsLastMethodUsed, vData, vReturn
        Else
            goLastServiceUsed.Execute sServiceID, gsLastMethodUsed
        End If
    End If
    LogEvent giEXECUTE_END, sServiceID
    'Return the data
    Select Case VarType(vReturn)
        Case vbNull, vbEmpty
            IWorker_DoService = Null
        Case vbObject, vbError, vbDataObject
            Set IWorker_DoService = vReturn
        Case Else
            IWorker_DoService = vReturn
    End Select
End Function


Public Sub IWorker_LoadServiceObject(ByVal ServiceLibClass As String, ByVal vServiceConfiguration As Variant)
Attribute IWorker_LoadServiceObject.VB_Description = "Loads an object whose ProgID matches ServiceLibClass, if PersistentServices is true."
    '-------------------------------------------------------------------------
    'Purpose:   Method is provided to instruct worker to Load an Service
    '           object without calling the execute method of the Service
    '           object.
    'IN:
    '   [ServiceLibClass]
    '           String that contains a ProgID. ServiceLibClass may be in the
    '           format "library.class.method" or "library.class"
    'Effects:
    '   [goEarlyBoundService]
    '           Set equal to AEService.Service class object if passed ProgID
    '           equals "AEService.Service"
    '   [gsLastCommandUsed]
    '           Set equal to ServiceLibClass parameter
    '   [gsLastLibClassUsed]
    '           Set equal to the Library.Class in the ServiceLibClass Parameter
    '   [gsLastMethodUsed]
    '           Set equal to the method portion of the passed ServiceLibClass parameter
    '   [goLastServiceUsed]
    '           Set equal to the object created using the passed ProgID
    '-------------------------------------------------------------------------
    'Method is provided to instruct worker to Load an Service
    'object without calling the execute method of the Service
    'object
    Dim oService As Object
    Dim sMethod As String
    Dim iPos As Integer
    gsLastCommandUsed = ServiceLibClass
    'Get the library.class from ServiceLibClass
    'ServiceLibClass may be in the format "library.class.method"
    'or "library.class"
    iPos = InStr(ServiceLibClass, gsCOMMAND_DELIMITER)
    If iPos = 0 Then Err.Raise giINVALID_COMMAND_PARAMETER, , LoadResString(giINVALID_COMMAND_PARAMETER)
    iPos = InStr((iPos + 1), ServiceLibClass, gsCOMMAND_DELIMITER)
    If iPos = 0 Then
        gsLastLibClassUsed = ServiceLibClass
    Else
        gsLastLibClassUsed = Left$(ServiceLibClass, (iPos - 1))
        gsLastMethodUsed = Right$(ServiceLibClass, Len(ServiceLibClass) - iPos)
    End If
    'Get the Service object
    CacheServiceConfiguration gsLastLibClassUsed, vServiceConfiguration
    Set goLastServiceUsed = GetServiceObject(gsLastLibClassUsed)
    If gbEarlyBind Then
        Set goEarlyBoundService = goLastServiceUsed
    End If
End Sub

Public Sub IWorker_ShutDown()
Attribute IWorker_ShutDown.VB_Description = "Causes Worker to stop processing a Service Requests and destroy its QueueDelegator object."
    '-------------------------------------------------------------------------
    'Purpose:   Shut down the Worker.  Timer is Killed.  Reference to Queue
    '           Manager is destroyed.
    'Effects:
    '   [gbShutDown]
    '           Is set to false.
    '-------------------------------------------------------------------------
    gbShutDown = True
    SetEnabled False
End Sub

Public Function IWorker_GetLogger() As APEInterfaces.ILogger
Attribute IWorker_GetLogger.VB_Description = "Returns the AELogger.Logger object instantiated by this Worker."
    '-------------------------------------------------------------------------
    'Purpose:   Get the logger object local to this worker
    'Return:    A valid AELogger.Logger object on the same machine as
    '           this Worker class object
    '-------------------------------------------------------------------------
    Set IWorker_GetLogger = goLogger
End Function

Public Sub IWorker_StartPollingQueue()
Attribute IWorker_StartPollingQueue.VB_Description = "Causes the Worker to start polling the GetServiceRequest method of the QueueDelegator object,  if the QueueDelegator object is set."
    '-------------------------------------------------------------------------
    'Purpose:   Provided for the QueueMgr to cause the Worker to start
    '           polling the QueueMgr
    'Effects:
    '           Starts timer so that Worker begins polling Queue
    '-------------------------------------------------------------------------
    'Start timer to pole queuemgr if not processing a service already
    If (Not gbNewService) And (Not goQueueDelegator Is Nothing) Then SetEnabled True
End Sub

'*********************
'Private procedures
'*********************

Private Sub Class_Initialize()
    On Error GoTo Class_InitializeError
    '-------------------------------------------------------------------------
    'Purpose:   Puts Worker in its initial state setting many globals
    '           to their defaults, if mlInstances = 1 after adding one to it.
    '-------------------------------------------------------------------------
    App.OleServerBusyRaiseError = True
    App.OleServerBusyTimeout = 10000
    'Set default property values
    Set goLogger = CreateObject("AELogger.Logger")
    gbLog = gbLOG_DEFAULT
    gbPersistent = gbPERSISTENCE_DEFAULT
    gbEarlyBind = gbEARLY_BIND_DEFAULT
    'Create cServices collection if gbPersistent
    If gbPersistent Then Set gcServices = New Collection
    SetInterval giTIMER_INTERVAL
    Exit Sub
Class_InitializeError:
    LogError Err, gsNULL_SERVICE_ID
    Resume Next
End Sub

Private Sub Class_Terminate()
    '-------------------------------------------------------------------------
    'Purpose:   Shuts down Worker and destroys objects that it has created,
    '           if mlInstances = 0
    '-------------------------------------------------------------------------
    On Error GoTo Class_TerminateError
    SetEnabled False
    Set goLogger = Nothing
    Set goLastServiceUsed = Nothing
    Set goEarlyBoundService = Nothing
    If gbPersistent Then Set gcServices = Nothing
    Exit Sub
Class_TerminateError:
    LogError Err, gsNULL_SERVICE_ID
    Resume Next
End Sub

