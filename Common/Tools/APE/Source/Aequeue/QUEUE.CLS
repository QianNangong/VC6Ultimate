VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Queue"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Job Manager"
Option Explicit
'-------------------------------------------------------------------------
'The Class is public, creatable, multi-use.  It is provide as an OLE interface
'for the Client applications to call, adding Service Requests to the Queue
' It implements the IQueue interface
'-------------------------------------------------------------------------

Implements APEInterfaces.IQueue

Public Enum APECallbackNotificationConstants
    apeCallbackModeNone = giNO_CALLBACK
    apeCallbackModeRegisterEveryRequest = giUSE_PASSED_CALLBACK
    apeCallbackModeRegisterOnce = giUSE_DEFAULT_CALLBACK
    apeCallbackModeUseRaiseEvent = giRETURN_BY_SYNC_EVENT
End Enum

Private moDefaultCallback As APEInterfaces.IClientCallback   'See DefaultCallback property comments
Private moEventObject As Object
Private mbHaveEventObject As Boolean

'******************
'Public Properties
'******************

Public Property Set IQueue_DefaultCallBack(ByVal oCallback As APEInterfaces.IClientCallback)
Attribute IQueue_DefaultCallBack.VB_Description = "Set the callback object to use when apeCallbackModeRegisterOnce is passed to the Add method as the callback mode."
    '-------------------------------------------------------------------------
    'Purpose:   The property allows a client to set a default
    '           callback specific to the Queue class object
    '           that is referenced by the client.  By setting
    '           this property a client can omit the CallBack parameter
    '           of the Queue.Add method and QueueMgr will use
    '           the default callback if a call back is required
    'In:
    '   [oCallback]
    '           Must be a valid callback object having a callback method
    'Effects:
    '   [moDefaultCallback]
    '           Class level variable is set equal to the passed object
    '-------------------------------------------------------------------------
    
    If oCallback Is Nothing Then Err.Raise giINVALID_CALLBACK + vbObjectError, Err.Source, LoadResString(giINVALID_CALLBACK)
    Set moDefaultCallback = oCallback
End Property

Public Property Get IQueue_DefaultCallBack() As APEInterfaces.IClientCallback
    Set IQueue_DefaultCallBack = moDefaultCallback
    
End Property

'*****************
'Public Methods
'*****************

Private Function IQueue_Add(ByVal sCommand As String, ByVal sServiceID As String, _
        ByVal iCallBackMode As APEInterfaces.APECallbackNotificationConstants, Optional ByVal vData As Variant, _
        Optional ByVal CallBack As APEInterfaces.IClientCallback) As Boolean
    '-------------------------------------------------------------------------
    'Purpose:   Called by client Applications to add a Service request to the
    '           Queue.
    'IN:
    '   [sCommand]
    '           The string that will be given to a worker with the passed data
    '           The worker uses this string to determine what OLE Server to
    '           use as a Service provider.  Part of the string is passed to the
    '           Service provider from the Worker.
    '   [sServiceID]
    '           An ID that identifies the service request
    '   [iCallBackMode]
    '           Defines if and how data is returned to client calling this function
    '   [vData]
    '           (Optional) Variant data that will be given to the Worker also.
    '           The Worker and the QueueMgr do not know what type of data this is
    '           The Worker will just pass it the the Service provider
    '   [Callback]
    '           (Optional) Callback object.  If present, it will be immediately passed
    '           to the Expediter.  The expediter will use it to callback
    '           delivering results that the Worker gives to it
    'Return:    True if the Service request was processed
    '           else returns false.
    'Effects:
    '           The Expediter will get called if a Callback is passed or bUseDefaultCallback
    '           is true
    '   [gbBusyAdding]
    '           is true during this procedure
    '   [gcQueue]
    '           will get a clsService class object, storing the Service request
    '           information.
    'Assumptions:
    '   [gcQueue]
    '           Is a valid collection object
    '-------------------------------------------------------------------------
    Dim oService As clsService      'clsService class object which is filled
                                    'with Service request data and added to collection
    Dim bDataPresent As Boolean     'Flag that data is present
    Dim oCallback As APEInterfaces.IClientCallback    'Callback object that will be passed to Expediter
    
    Dim lCount As Long              'gcQueue.count
    
    On Error GoTo AddError
    
    IQueue_Add = False
    
    'Exit sub if Stopping Queue
    If gbStopTest Then Exit Function
    
    gbBusyAdding = True
    'Check if the QueueMgr is too busy to process request
    If gcQueue.Count >= glMaxQueueSize Then Err.Raise giQUEUE_MGR_IS_BUSY
    'Check if data was passed
    If IsMissing(vData) Then bDataPresent = False Else bDataPresent = True
    'Validate that the Expediter was created successfully
    If iCallBackMode <> giNO_CALLBACK Then
        If gbFailedToCreateExpediter Then Err.Raise giCOULD_NOT_CREATE_EXPEDITER
    End If
    'Validate callback object
    Select Case iCallBackMode
        Case giUSE_PASSED_CALLBACK
            If CallBack Is Nothing Then
                Err.Raise giINVALID_CALLBACK
            Else
                Set oCallback = CallBack
            End If
        Case giUSE_DEFAULT_CALLBACK
            If moDefaultCallback Is Nothing Then
                Err.Raise giINVALID_CALLBACK
            Else
                Set oCallback = moDefaultCallback
            End If
        Case giRETURN_BY_SYNC_EVENT
            If Not mbHaveEventObject Then Err.Raise giFIRST_GET_WITHEVENTS_OBJECT
    End Select
    
    'Iterate count of this method call
    glAddCallCount = glAddCallCount + 1
    
    'Update U/I if form is visible
    If gbShow Then frmQueueMgr.lblCount = glAddCallCount
    
    Set oService = New clsService
    
    LogEvent giADD_RECEIVED, sServiceID
    
    'Create the put the Service request values
    'in the clsService object
    With oService
        .ID = sServiceID
        .Command = sCommand
        .CallBackMode = iCallBackMode
        Select Case iCallBackMode
            Case giUSE_PASSED_CALLBACK, giUSE_DEFAULT_CALLBACK
                Set .CallBack = oCallback
            Case giRETURN_BY_SYNC_EVENT
                Set .EventObject = moEventObject
        End Select
        .DataPresent = bDataPresent
        'Check what data type vData return is
        'in order to determine how to handle it
        If bDataPresent Then
            Select Case VarType(vData)
                Case vbEmpty, vbNull
                    .Data = Null
                Case vbObject, vbError, vbDataObject
                    Set .Data = vData
                Case Else
                    .Data = vData
            End Select
        End If
    End With
    'Add oService to Queue using ID as Key
    gcQueue.Add oService, sServiceID
    
    'Display stats
    lCount = gcQueue.Count
    If gbShow Then frmQueueMgr.lblQueue = lCount
    If lCount > glPeakQueueSize Then
        glPeakQueueSize = lCount
        If gbShow Then frmQueueMgr.lblPeak = glPeakQueueSize
    End If

    If gbStopTest And Not gbBusyGetServiceRequest And Not gbBusyGetServiceResults Then StopQueue
    
    'Flip the status flag right before the calling client is released
    'A worker should not be allowed to take an activity request
    'until the client is released.  This keeps the the expediter
    'from calling the client with Service results before the client
    'is released with the return value, the Service ID
    oService.Status = giWAITING_FOR_WORKER
    Set oService = Nothing
    gbBusyAdding = False
    
    IQueue_Add = True

    Exit Function
AddError:
    Select Case Err.Number
        Case giQUEUE_MGR_IS_BUSY
            gbBusyAdding = False
            Err.Raise Err.Number + vbObjectError, Err.Source, LoadResString(Err.Number)
            Exit Function
        Case Is > giERROR_THRESHOLD
            LogError Err, gsNULL_SERVICE_ID
            gbBusyAdding = False
            Err.Raise Err.Number + vbObjectError, Err.Source, LoadResString(Err.Number)
            Exit Function
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            Dim iRetry As Integer
            Dim il As Integer
            Dim ir As Integer
            'First check for stop test
            If gbStopTest And Not gbBusyGetServiceRequest Then StopQueue: Exit Function
            If iRetry < giMAX_ALLOWED_RETRIES Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                Next il
                LogEvent giCALL_REJECTED_RETRY, sServiceID
                Resume
            Else
                'We reached our max retries
                LogError Err, sServiceID
                gbBusyAdding = False
                Err.Raise Err.Number, Err.Source, Err.Description
            End If
        Case giCOULD_NOT_CREATE_EXPEDITER
            LogError Err, sServiceID
            gbBusyAdding = False
            Err.Raise giCOULD_NOT_CREATE_EXPEDITER + vbObjectError, Err.Source, LoadResString(giCOULD_NOT_CREATE_EXPEDITER)
        Case giFIRST_GET_WITHEVENTS_OBJECT
            LogError Err, sServiceID
            gbBusyAdding = False
            Err.Raise giFIRST_GET_WITHEVENTS_OBJECT + vbObjectError, Err.Source, LoadResString(giFIRST_GET_WITHEVENTS_OBJECT)
        Case giINVALID_CALLBACK
            LogError Err, sServiceID
            gbBusyAdding = False
            Err.Raise giINVALID_CALLBACK + vbObjectError, Err.Source, LoadResString(giINVALID_CALLBACK)
        Case ERR_OVER_FLOW
            LogError Err, sServiceID
            If glAddCallCount = glMAX_ID Then glAddCallCount = 0
            Resume
        Case Else
            LogError Err, sServiceID
            gbBusyAdding = False
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Function
    End Select
End Function

Public Function IQueue_GetEventObject() As Object
Attribute IQueue_GetEventObject.VB_Description = "Returns the event source object that a client must respond to when apeCallbackModeUseRaiseEvent is passed to the Add method as the callback mode."
    If Not mbHaveEventObject Then
        Set moEventObject = goExpediter.GetEventObject
        mbHaveEventObject = True
    End If
    Set IQueue_GetEventObject = moEventObject
End Function

'*******************
'Private methods
'*******************

Private Sub Class_Initialize()
    CountInitialize
End Sub

Private Sub Class_Terminate()
    CountTerminate
End Sub
