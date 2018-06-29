Attribute VB_Name = "modWorker"
Option Explicit
'-------------------------------------------------------------------------
'The project is the Worker component of the Application Performance Explorer
'The Worker is designed to function as a thread per object or
'single-use OLE Server to be created by the QueueMgr
'component.  The Worker loads Service provider objects to accomplish
'tasks.  The Worker gets task requests from the Queue Manager and returns
'results to the back to the Queue Manager.
'
'Key Files:
'   Worker.cls      public class used as controling OLE interface
'-------------------------------------------------------------------------
'Declares
Private Declare Function GetTickCount Lib "kernel32" () As Long
Private Declare Function SetTimer Lib "user32" (ByVal hWnd As Long, ByVal nIDEvent As Long, ByVal uElapse As Long, ByVal lpTimerFunc As Long) As Long
Private Declare Function KillTimer Lib "user32" (ByVal hWnd As Long, ByVal nIDEvent As Long) As Long

'Public Constants
Public Const giERROR_THRESHOLD As Integer = 32000
Public Const giTIMER_INTERVAL As Integer = 500
Public Const giMAX_ALLOWED_RETRIES  As Integer = 500
Public Const giRETRY_WAIT_MIN  As Integer = 500      'Retry Wait is measure in DoEvent cyles
Public Const giRETRY_WAIT_MAX  As Integer = 2500

'Property defaults
Public Const gbLOG_DEFAULT As Boolean = False
Public Const gbPERSISTENCE_DEFAULT As Boolean = True
Public Const gbEARLY_BIND_DEFAULT As Boolean = True
Public Const gsCOMMAND_DELIMITER As String * 1 = "."

'Message Constants, resourse string
Public Const giEXECUTE_BEGIN As Integer = 3
Public Const giEXECUTE_END As Integer = 4
Public Const giGET_REQUEST_BEGIN As Integer = 6
Public Const giWORKER_NAME As Integer = 7
Public Const giGET_REQUEST_END_NEW_SERVICE As Integer = 8
Public Const giGET_REQUEST_END_NO_SERVICE As Integer = 9
Public Const giDO_SERVICE_RECEIVED As Integer = 10
Public Const giCALL_REJECTED_RETRY As Integer = 11
Public Const giERROR_PREFIX As Integer = 12

'User Defined Error Codes which serve as resource string Indexes
Public Const giINVALID_COMMAND_PARAMETER As Integer = 32767

'Default Timer interval
Private Const mlDEFAULT_INTERVAL As Long = 1

'User Defined Type
Public Type Service
    ID As String                  'The ID of the Service Request, known by QueueMgr, and Expediter
    Command As String           'Format is "Library.Class.Method"
    Data As Variant             'Variant received by Queue Mgr pass from a client
    Return As Variant           'Variant returned by Service object
    DataPresent As Boolean      'Flag received by QueueMgr, if true pass data to Service object
End Type

Public glWorkerID As Long       'This is given by the queue manager
Public gbLog As Boolean         'If true log Service
Public gbPersistent As Boolean  'If true, the worker keeps reference to
                                'every Service object it has used, else
                                'worker releases Service object after
                                'each use.
Public gbEarlyBind As Boolean   'If true, the worker uses code that utilizes
                                'early binding.  This option is only
                                'available for Service objects classes that
                                'were developed with the worker class,
                                'because early binding is only available
                                'if class names are hard coded.
Public goLogger As APEInterfaces.ILogger     'Logger object
Public goQueueDelegator As APEInterfaces.IQueueDelegator  'QueueManager class object
Public gcServices As Collection        'Collection of Service objects, using
                                        '"Library.Class" as the key
                                        'Object and its name
Public gcServiceConfigurations As New Collection    ' Collection of Service Configuration Options variants
                                        ' indexed by "Library.Class"
Public gudtService As Service          'Stores the Service Request that needs processed
Public gbNewService As Boolean         'If true a new Service is being processed.
                                        'Do not retrieve another
Public gbShutDown As Boolean

'The following object variable is only used if gbEarlyBind is true
Public goEarlyBoundService As APEInterfaces.IService
'The following variables are used to speed up successive
'Service request that have the same Command value (Lib.Class.Method)
Public goLastServiceUsed As Object
Public gsLastCommandUsed As String
Public gsLastMethodUsed As String
Public gsLastLibClassUsed As String



'Timer related variables
Private mlTimerID As Long               'The ID of the system timer created by this object
Private mlInterval As Long              'The interval of this timer
Private mbTimerStarted As Boolean       'If true, a system timer is set for this object
Private mbTimerEnabled As Boolean       'Equals the enabled property of this object

'***********************
'Public Procedures
'***********************

Public Sub GetLibClassMethod(sLibClass As String, sMethod As String, sReceived As String)
    '-------------------------------------------------------------------------
    'Purpose:   Called by the DoService and CheckService procedure.  Get the
    '           library.class  and the method from sReceived.  sReceived is in
    '           the format "library.class.method".
    'In:        [sReceive]
    '               Is the Command received by a client or the QueueMgr, in the
    '               format "library.class.method"
    'Out:       [sLibClass]
    '               Returns the "library.class" of sReceive
    '           [sMethod]
    '               Returns the "method" of sReceive
    '-------------------------------------------------------------------------
    Dim iPos As Integer
    
    iPos = InStr(sReceived, gsCOMMAND_DELIMITER)
    If iPos = 0 Then Err.Raise giINVALID_COMMAND_PARAMETER, , LoadResString(giINVALID_COMMAND_PARAMETER)
    iPos = InStr((iPos + 1), sReceived, gsCOMMAND_DELIMITER)
    If iPos = 0 Then Err.Raise giINVALID_COMMAND_PARAMETER, , LoadResString(giINVALID_COMMAND_PARAMETER)
    sLibClass = Left$(sReceived, (iPos - 1))
    sMethod = Right$(sReceived, (Len(sReceived) - iPos))
End Sub

Public Function GetServiceObject(sLibClass As String) As Object
    '-------------------------------------------------------------------------
    'Purpose:   Called by CheckService and LoadServiceObject.  Loads the Server
    '           object specified by sLibClass.  If mbPerssistent is true, see
    '           if the object is already loaded and in gcServices collection.
    '           If it isn't in collection, load it and add it to the collection
    'In:        [sLibClass]
    '               ProgID of needed server object
    'Return:    Object specified by passed ProgID
     'Assumption:
    '           gcServices is a valid collection object if gbPersistent is true
    '           gcServiceConfigurations contains an object indexed by sLibClass
    '-------------------------------------------------------------------------
    Dim oService As Object      'Object that will be return after being
                                'created a retrieved according to ProgID
    If gbPersistent Then
        'Turn off error handling in case
        'the key sLibClass does not exist in the collection
        Err.Clear
        On Error Resume Next
        Set oService = gcServices.Item(sLibClass)
        'If Error, Object variable or With block variable not set occured
        'Create object and add it to gcServices
        If Err.Number = ERR_INVALID_PROCEDURE_CALL Then
            'Use user defined function that handles
            'Late binding versus early binding options
            On Error GoTo 0
            Set oService = CreateObject(sLibClass)
            gcServices.Add oService, sLibClass
        End If
    Else
        Set oService = CreateObject(sLibClass)
    End If
    oService.Initialize gcServiceConfigurations(sLibClass)
    Set GetServiceObject = oService
End Function

Public Sub LogEvent(intMessage As Integer, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   To add a record to the AELogger.Logger object, if logging is on.
    'In:        [intMessage]
    '               Resource string key of the string that should put in the log
    '               record
    '           [sServiceID]
    '               The ID of the Service or Task request that will form part of
    '               the log record
    'Assumption:
    '           goLogger is a valid AELogger.Logger object if gbLog is true
    '-------------------------------------------------------------------------
    On Error GoTo LogEventError
    If gbLog Then
        goLogger.Record LoadResString(giWORKER_NAME) & Str$(glWorkerID), sServiceID, LoadResString(intMessage), GetTickCount()
    End If
    Exit Sub
LogEventError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
               'Collision error, the OLE server is busy
               Dim iRetry As Integer
               Dim il As Integer
               Dim ir As Integer
               If iRetry < giMAX_ALLOWED_RETRIES Then
                   iRetry = iRetry + 1
                   ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                   For il = 0 To ir
                       DoEvents
                   Next il
                   Resume
               Else
                   'We reached our max retries
                   'This would occur when clients are sending
                   'there logs
                   LogError Err, gudtService.ID
                   Exit Sub
               End If
        Case Else
            LogError Err, gudtService.ID
            Exit Sub
    End Select
    Exit Sub
End Sub

Public Sub LogError(ByVal oErr As ErrObject, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   To add an error record to the AELogger.Logger object
    '
    'In:        [oErr]
    '               Error object which hold the error information that will
    '               form the log record
    '           [sServiceID]
    '               The ID of the Service or Task request that will form part of
    '               the log record
    'Assumption:
    '           goLogger is a valid AELogger.Logger object
    '-------------------------------------------------------------------------
    goLogger.Record LoadResString(giWORKER_NAME) & Str$(glWorkerID), sServiceID, LoadResString(giERROR_PREFIX) & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description, GetTickCount()
End Sub

Public Sub SetInterval(lInterval As Long)
    '-------------------------------------------------------------------------
    'Purpose:   Changes the interval of the Timer
    'In:        [lInterval]
    '               The new interval to set the timer to.
    'Effects:   [mlInterval]
    '               Becomes equal to lInterval
    '           Calls SetInterval only if there is a system timer corresponding
    '           to this object
    '-------------------------------------------------------------------------
    If mlInterval <> lInterval Then
        mlInterval = lInterval
        If mbTimerStarted Then
            ChangeInterval lInterval, mlTimerID
        End If
    End If
End Sub

Public Sub SetEnabled(bEnabled As Boolean)
    '-------------------------------------------------------------------------
    'Purpose:   Starts a system timer if bEnabled is true
    '           Stops the timer if bEnabled is false
    'Effects:   [mbTimerEnabled] is set equal to bEnabled
    '           [mbTimerStarted] is set to true if StartTimer succeeds
    '                            is set to false if StopTimer succeeds
    '           If true a new system timer is started and the TimerID
    '           is stored in a class level variable so that this object
    '           can effect the specific system timer.
    '-------------------------------------------------------------------------
    Dim lReturn As Long
    'If shut down flag is true always disable timer
    If gbShutDown Then bEnabled = False
    
    mbTimerEnabled = bEnabled       'Even if calling KillTimer fails
                                    'This flag will stop processing of TimerProc events
    If bEnabled <> mbTimerStarted Then
        If bEnabled Then
            mlTimerID = StartTimer(mlInterval)
            If mlTimerID <> 0 Then
                mbTimerStarted = True
            End If
        Else
            lReturn = StopTimer(mlTimerID)
            If lReturn = 1 Then
                mbTimerStarted = False
            End If
        End If
    End If
End Sub

Public Sub CacheServiceConfiguration(sLibClass As String, vServiceConfiguration As Variant)
    Dim vTemp As Variant
    Dim lError As Long
    
    On Error Resume Next
    vTemp = gcServiceConfigurations(sLibClass)
    lError = Err.Number
    On Error GoTo 0
    Select Case lError
        Case 0  ' Item exists
            If IsDifferent(vTemp, vServiceConfiguration) Then       ' Update configuration
                With gcServiceConfigurations
                    .Remove sLibClass
                    .Add vServiceConfiguration, sLibClass
                End With
            End If
        Case ERR_INVALID_PROCEDURE_CALL ' Item doesn't exist
            gcServiceConfigurations.Add vServiceConfiguration, sLibClass
        Case Else
            Err.Raise lError
    End Select
End Sub

'********************
'Private Procedures
'********************
Private Function IsDifferent(vA As Variant, vB As Variant) As Boolean
' Compares two variants and returns True if they are different.
    If VarType(vA) <> VarType(vB) Then
        IsDifferent = True
    Else
        Select Case VarType(vA)
            Case vbArray Or (VarType(vA) Xor vbArray)
                ' Presumes array of intrinsic types
                If LBound(vA) = LBound(vB) And UBound(vA) = UBound(vB) Then
                    Dim i As Long
                    IsDifferent = False
                    For i = LBound(vA) To UBound(vA)
                        If vA(i) <> vB(i) Then
                            IsDifferent = True
                            Exit For
                        End If
                    Next
                Else
                    IsDifferent = True
                End If
            Case vbObject, vbDataObject
                IsDifferent = Not (vA Is vB)
            Case Else
                IsDifferent = (vA <> vB)
        End Select
    End If
End Function

Private Sub CheckService()
    '-------------------------------------------------------------------------
    'Purpose:   Called by the PollQueue procedure.  Checks to see of service
    '           task request data was returned by call to QueueMgr.  If it
    '           was, CheckService loads the needed Service provider object
    '           and calls it to accomplish the task. The this procedure poles
    '           the QueueMgr again to get a new task request.  In the same call
    '           the finished service request results are returned as parameters.
    '           The new service request is received as the result.
    '           If no task, request is returned
    '           the timer is started which will continue poling the queue.
    '           Otherwise, CheckService will continue a cycle as long as the
    '           the QueueMgr returns a task request when poled.
    'Assumptions:
    '           If [gbNewService] is true, there is valid task request data
    '           in the [gudtService] type.
    '           [goQueueDelegator] is a valid reference to AEQueueMgr.clsQueueDelegator
    '           if the goQueueDelegator.GetServiceRequest returns an integer value of giCLOSE_WORKER_NOW
    '           then the Worker object has been released from the QueueMgr.
    'Effects:
    '           If the goQueueDelegator.GetServiceRequest returns a value of giCLOSE_WORKER_NOW
    '           this procedure will close this Worker application
    '           [gudtService]
    '               After using the data in this type, this procedure will call
    '               the QueueMgr again, and fill this type with new data if a
    '               task request was returned.
    '           [goEarlyBoundService]
    '               Set equal to AEService.Service class object if passed ProgID
    '               equals "AEService.Service"
    '           [gsLastCommandUsed]
    '               Will get updated with the current value of gudtService.Command
    '           [gsLastLibClassUsed]
    '               Will get updated with the current Lib.Class portion of
    '               of gudtService.Command
    '           [gsLastMethodUsed]
    '               Will get updated with the current .Method portion of
    '               gudtService.Command
    '           [goLastServiceUsed]
    '               Set equal to the object created using the passed ProgID
    '           Timer is disabled on entrance and enabled if
    '               loop is exited becaue QueueMgr did not return a task request
    '-------------------------------------------------------------------------
    Dim sError As String        'Error handling places error information into
                                'this string in the format of "number; source; description"
                                'passed to QueueMgr with return data.  Client
                                'receives this.  Elements of error collection
                                'are delimited by a semicolon and a space.
    Dim iRetry As Integer       'Counter for how many attempts are made to call
                                'an OLE server.
    Dim vServiceData As Variant 'value returned by QueueMgr as a variant array
                                'Elements of it are placed into gudtService type
    Dim bCallingQueue As Boolean
    
    On Error GoTo CheckServiceError
    'Disable timer because it only needs enabled
    'When there is a new Service
    SetEnabled False
    'If there is a new Service process it
    If gbNewService Then
        Do
            iRetry = 0
            
            If Not gsLastCommandUsed = gudtService.Command Or goLastServiceUsed Is Nothing Then
                'Get the library.class from gudtService.Command
                'gudtService.Command is in the format "library.class.method"
                gsLastCommandUsed = gudtService.Command
                GetLibClassMethod gsLastLibClassUsed, gsLastMethodUsed, gudtService.Command
                If (VarType(gudtService.Data) And vbArray) <> 0 Then
                    CacheServiceConfiguration gsLastLibClassUsed, gudtService.Data(giRECORD_SERVICE_CONFIGURATION)
                Else
                    CacheServiceConfiguration gsLastLibClassUsed, gudtService.Data(CStr(giRECORD_SERVICE_CONFIGURATION))
                End If
                'Get the Service object
                Set goLastServiceUsed = GetServiceObject(gsLastLibClassUsed)
                If gbEarlyBind Then
                    Set goEarlyBoundService = goLastServiceUsed
                End If
            End If
            'Call the execute method of the class object
            'passing the method string and the send data as variant,
            'and the return data as variant by reference
            LogEvent giEXECUTE_BEGIN, gudtService.ID
            If gbEarlyBind Then
            'Use the Earlybound object reference
                If gudtService.DataPresent Then
                    goEarlyBoundService.Execute gudtService.ID, gsLastMethodUsed, gudtService.Data, gudtService.Return
                Else
                    goEarlyBoundService.Execute gudtService.ID, gsLastMethodUsed
                End If
            Else
                If gudtService.DataPresent Then
                    goLastServiceUsed.Execute gudtService.ID, gsLastMethodUsed, gudtService.Data, gudtService.Return
                Else
                    goLastServiceUsed.Execute gudtService.ID, gsLastMethodUsed
                End If
            End If
            LogEvent giEXECUTE_END, gudtService.ID
            
SendServiceResult:

            'Notify QueueManager that Service is completed
            'And worker is ready for a new Service
            'QueueMgr can return another Service
            'In the same call pass the Service Request results
            'to the QueueMgr as parameters.  The Expediter will
            'get the Service results from the Queue manager
            LogEvent giGET_REQUEST_BEGIN, gudtService.ID
            iRetry = 0
            
            'Call the QueueMgr to get a new Service Request
            bCallingQueue = True
            vServiceData = goQueueDelegator.GetServiceRequest(glWorkerID, gudtService.ID, gudtService.Return, CVar(sError))
            bCallingQueue = False
            
            'Check if results were returned
            'The QueueMgr will not return results if it has no Service Requests
            'The QueueMgr may return an integer value of giCLOSE_WORKER_NOW
            'If it does close this worker
            If VarType(vServiceData) = (vbArray + vbVariant) Then
                'Results were returned
                With gudtService
                    .ID = vServiceData(giSERVICE_ID_ELEMENT)
                    .Command = vServiceData(giCOMMAND_ELEMENT)
                    .DataPresent = vServiceData(giDATA_PRESENT_ELEMENT)
                    Select Case VarType(vServiceData(giSERVICE_DATA_ELEMENT))
                        Case vbNull, vbEmpty
                            .Data = Null
                        Case vbObject, vbError, vbDataObject
                            Set .Data = vServiceData(giSERVICE_DATA_ELEMENT)
                        Case Else
                            .Data = vServiceData(giSERVICE_DATA_ELEMENT)
                    End Select
                End With
                'If an Service was passed back call CheckService again
                'Log whether or not a new Service was received
                LogEvent giGET_REQUEST_END_NEW_SERVICE, gudtService.ID
                gbNewService = True
            Else
                'Check to see if the Worker should shut down!!!!
                If VarType(vServiceData) = vbInteger Then
                    If vServiceData = giCLOSE_WORKER_NOW Then
                        'Close the Worker
                        Exit Sub        'The Worker Terminate event will do the rest
                    End If
                End If
                
                'Results were not returned. See if Worker needs kept alive by Setting
                'a timer or if it needs shutdown because ShutDown was called.
                LogEvent giGET_REQUEST_END_NO_SERVICE, gudtService.ID
                gbNewService = False
                If Not gbShutDown Then
                    SetEnabled True
                End If
            End If
        Loop Until (VarType(vServiceData) <> vbArray + vbVariant) Or gbShutDown
    End If
    Exit Sub
CheckServiceError:
    Dim il As Integer
    Dim ir As Integer
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
               'Collision error, the OLE server is busy
               If iRetry < giMAX_ALLOWED_RETRIES Then
                   iRetry = iRetry + 1
                   ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                   For il = 0 To ir
                       DoEvents
                   Next il
                   'Shutdown may be called during DoEvents loop
                   If gbShutDown Then
                        Exit Sub
                   Else
                        LogEvent giCALL_REJECTED_RETRY, gudtService.ID
                        Resume
                   End If
               Else
                   'We reached our max retries
                    LogError Err, gudtService.ID
               End If
        Case Is > giERROR_THRESHOLD
            'Create error string and pass it back to the client
            'Through the QueueMgr and Expediter
            sError = Err.Source & "; " & CStr(Err.Number + vbObjectError) & "; " & Err.Source & gsSEPERATOR & Err.Description
            LogError Err, gudtService.ID
            Resume SendServiceResult
        Case ERR_OBJECT_VARIABLE_NOT_SET
            LogError Err, gudtService.ID
            If Not bCallingQueue Then
                sError = Err.Source & "; " & CStr(Err.Number) & "; " & Err.Description
                Resume SendServiceResult
            End If
        Case Else
            LogError Err, gudtService.ID
            sError = Err.Source & "; " & CStr(Err.Number) & "; " & Err.Description
            Resume SendServiceResult
    End Select
    'Reset timer and flag so that Worker will continue
    'processing other Services even though this one may have failed.
    SetEnabled True
    gbNewService = False
    Exit Sub
End Sub

Private Sub PollQueue()
    '-------------------------------------------------------------------------
    'Purpose:   Called by the TimerProc.  Calls QueueMgr to
    '           received task request data.  If data is returned, timer is
    '           disabled and CheckService is called to process it.
    'Assumptions:
    '           If CheckService is processing a TaskRequest [gbNewService] is true.
    '           [moQueueDelegatorMgr] is a valid refererence to AEQueueMgr.goQueueDelegator
    'Effects:
    '           [gudtService]
    '               is filled with task request data if call to QueueMgr
    '               returns a task request.
    '           Timer is disabled if task request is received
    '           [gbNewService] is made true if a task request is received
    '-------------------------------------------------------------------------
    Dim vServiceData As Variant     'Return value of moQueueDelegatorMgr.GetServiceRequest
                                    'if a task request is returned this will be
                                    'a variant array.  Elements of the array will
                                    'be placed in the gudtService type
    On Error GoTo PoleQueueError
    'If I am already processing a Service do not pole the QueueManager
    If gbNewService Or gbShutDown Then Exit Sub
    vServiceData = goQueueDelegator.GetServiceRequest(glWorkerID)
    If VarType(vServiceData) = vbArray + vbVariant Then
        With gudtService
            .ID = vServiceData(giSERVICE_ID_ELEMENT)
            .Command = vServiceData(giCOMMAND_ELEMENT)
            .DataPresent = vServiceData(giDATA_PRESENT_ELEMENT)
            Select Case VarType(vServiceData(giSERVICE_DATA_ELEMENT))
                Case vbNull, vbEmpty
                    .Data = Null
                Case vbObject, vbError, vbDataObject
                    Set .Data = vServiceData(giSERVICE_DATA_ELEMENT)
                Case Else
                    .Data = vServiceData(giSERVICE_DATA_ELEMENT)
            End Select
        End With
        'If an Service was passed back call CheckService again
        'Log whether or not a new Service was received
        LogEvent giGET_REQUEST_END_NEW_SERVICE, gudtService.ID
        gbNewService = True
        SetEnabled False
        CheckService
    Else
        gbNewService = False
        SetEnabled True
    End If
    Exit Sub
PoleQueueError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim il As Integer
            Dim ir As Integer
            Dim iRetry As Integer
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                'Shutdown may be called during DoEvents loop
                If gbShutDown Then Exit Sub Else Resume
            Else
                'We reached our max retries
                LogError Err, gudtService.ID
                Exit Sub
            End If
        Case Is > giERROR_THRESHOLD
            'Create error string and pass it back to the client
            'Through the expediter and QueueMgr
            LogError Err, gudtService.ID
            SetEnabled True
            Exit Sub
        Case Else
            LogError Err, gudtService.ID
            SetEnabled True
            Exit Sub
    End Select
End Sub

Private Function StartTimer(lInterval As Long) As Long
    '-------------------------------------------------------------------------
    'Purpose:   Starts a system timer
    'In:        [lInterval]
    '           The interval in milliseconds for the desired timer
    'Effects:   Calls the SetTimer API, passing the AddressOF the TimerProc
    '           Function and lInterval
    '-------------------------------------------------------------------------
    StartTimer = SetTimer(0, 0, lInterval, AddressOf TimerProc)
End Function

Private Function StopTimer(lTimerID As Long) As Long
    '-------------------------------------------------------------------------
    'Purpose:   Stops a specific system timer
    'In:        [lTimerID]
    '           The ID of the specific system timer to stop
    'Effects:   Calls the KillTimerID API, passing lTimerID
    '-------------------------------------------------------------------------
    StopTimer = KillTimer(0, lTimerID)
End Function

Private Sub ChangeInterval(lInterval As Long, lTimerID As Long)
    '-------------------------------------------------------------------------
    'Purpose:   Changes the interval of an alreading existing system timer
    'In:        [lTimerID]
    '           The ID of the specific system timer to change
    '           [lInterval]
    '           The interval to change the timer to.
    'Effects:   Calls the SetTimer API, passing lTimerID, lInterval, and the
    '           AddressOf TimerProc
    '-------------------------------------------------------------------------
    Dim lResult As Long
    lResult = SetTimer(0, lTimerID, lInterval, AddressOf TimerProc)
End Sub

Private Sub TimerProc(ByVal lHwnd As Long, ByVal lMsg As Long, ByVal lTimerID As Long, ByVal lTime As Long)
    '-------------------------------------------------------------------------
    'Purpose:   Address of this function is passed in the SetTimer API.  When
    '           a system timer is started it calls this function every set
    '           interval
    '-------------------------------------------------------------------------
    Static stbInPollQueue As Boolean    'Static flag is used to keep PollQueue
                                        'from being entered while it is in a
                                        'DoEvents loop to handle error
                                        'RPC_E_CALL_REJECTED
    On Error Resume Next
    If mbTimerEnabled And (Not stbInPollQueue) Then
        stbInPollQueue = True
        PollQueue
        stbInPollQueue = False
    End If
End Sub


