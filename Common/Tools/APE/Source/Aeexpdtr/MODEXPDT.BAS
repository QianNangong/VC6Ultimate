Attribute VB_Name = "modExpediter"
Option Explicit
'-------------------------------------------------------------------------
'The project is the Expediter component of the Application Performance Explorer
'The Expediter is a multi-use server that is instanced by the QueueMgr.
'The Expediter pulls Service Results data and Callbacks objects from
'the QueueMgr and then sends the Service Results using the Callback objects
'
'Key Files:
'   frmExpdt.frm    Only form in this project
'   CallbkRf.cls    Class used to store callback object and related
'                   Service request data
'   clsPosFm.cls    Class used to store Form position in registry
'   Expeditr.cls    Multi-use creatable class provides OLE interface to app
'-------------------------------------------------------------------------

'Declares
Declare Function GetTickCount Lib "Kernel32" () As Long

'U/I captions resource string keys
Public Const giFORM_CAPTION As Integer = 101
Public Const giCURRENT_BACKLOG_CAPTION As Integer = 102
Public Const giPEAK_BACKLOG_CAPTION As Integer = 103
Public Const giTOTAL_CALLBACK_CAPTION As Integer = 104

'Constants
Public Const gbSHOW_FORM_DEFAULT As Boolean = False
Public Const gbLOG_DEFAULT As Boolean = False
Public Const glMAX_COUNT As Long = 2147483647        'max size of long data type
Public Const giMAX_ALLOWED_RETRIES As Integer = 500  'maximum number of times one object can be
                                                     'called with call rejection before giving up
Public Const giRETRIES_ALLOWED_BEFORE_MOVING_ON = 10 'Number of retries made on a callback before
                                                     'it is skipped to try again later
Public Const giRETRY_WAIT_MIN As Integer = 500       'Retry Wait is measure in DoEvent cyles
Public Const giRETRY_WAIT_MAX As Integer = 2500
Public Const giTIMER_INTERVAL As Integer = 1000

'Message Constants, resourse string
Public Const giCALLBACK_CALLED As Integer = 4
Public Const giEXPEDITER_NAME As Integer = 5
Public Const giCALLING_CALLBACK As Integer = 7
Public Const giSTOP_TEST_RECEIVED As Integer = 8
Public Const giCALL_REJECTED_RETRIES_EXHAUSTED As Integer = 9
Public Const giRETRY_CALLBACK As Integer = 10
Public Const giGETRESULTS_CALLED_WITH_RETURN = 11
Public Const giCOULD_NOT_FIND_SYNC_OBJECT = 12
Public Const giERROR_PREFIX = 13
Public Const giFONT_CHARSET_INDEX As Integer = 30
Public Const giFONT_NAME_INDEX As Integer = 31
Public Const giFONT_SIZE_INDEX  As Integer = 32

'Public Variables

Public gbShow As Boolean                    'If true show form
Public glInstances As Long                  'Count of created instances of Expediter Class
Public gcCallBack As Collection             'Collection of CallBackRef class
Public gbLog As Boolean                     'If true log Service
Public goLogger As APEInterfaces.ILogger          'Logger class object
Public goQueueDelegator As APEInterfaces.IQueueDelegator 'QueueMgr object
Public gbStopTest As Boolean                'Flag used to stop processing
Public glBacklog As Long                    'The current number of Callbacks ready to be called
Public glPeakBacklog As Long                'The largest that of Callbacks that were ready to be
                                            'called has been as once
Public glTotalCallBacks As Long             'The total number of Callbacks made
Public gbBusy As Boolean                    'If true in frmExpediter.tmrExpediter.Timer event
Public gbUnloading As Boolean               'If true Class_Terminate of Expediter has been entered

Sub Main()
End Sub

Public Function PollQueue() As Boolean
    '-------------------------------------------------------------------------
    'Purpose:   Get Service Results and corresponding Callback objects from the
    '           QueueMgr
    'Return:    True if one or more Service Result was received from the QueueMgr
    'Assumes:
    '   [goQueueDelegator]
    '           is a valid AEQueueMgr.QueueDelegator object
    '   [gcCallback]
    '           is a valid collection object
    'Effects:
    '   [gcCallback]
    '           A CallBkRf object will be added for every Service Result received
    '           from the QueueMgr.
    '-------------------------------------------------------------------------
    Dim vaResults As Variant    'Variant array that will be received from call
                                'to the QueueMgr.  Two dimensions: first dimension
                                'is fixed each index representing a Service Result
                                'element; the second dimension each index represents
                                'one Service result.  See index constants in
                                'modAEConstants
    Dim lCount As Long          'Counter used to loop through indexes of the
                                'arrays second dimension
    Dim oCallBkRef As CallBackRef   'Object to store service results in and add
                                    'to gcCallback
    Dim bReturn As Boolean          'Value to be returned by this function
    Dim lUB As Long             'Ubound
    
    On Error GoTo PollQueueError
    bReturn = False
    
    'Call the QueueMgr
    vaResults = goQueueDelegator.GetServiceResults
    
    'Check to see if results were returned
    If VarType(vaResults) = vbArray + vbVariant Then
        'Results were returned
        bReturn = True
        LogEvent giGETRESULTS_CALLED_WITH_RETURN, 0
        'Put each service result in a CallBackRef object
        'and at it to the gcCallback collection
        lUB = UBound(vaResults, 2)
        For lCount = 0 To lUB
            Set oCallBkRef = New CallBackRef
            With oCallBkRef
                .ServiceID = vaResults(giRESULT_ID_ELEMENT, lCount)
                If vaResults(giRESULT_CALLBACK_TYPE_ELEMENT, lCount) = giRETURN_BY_SYNC_EVENT Then
                    .UseSyncEvent = True
                    Set .SyncObject = vaResults(giRESULT_CALLBACK_ELEMENT, lCount)
                Else
                    .UseSyncEvent = False
                    Set .Object = vaResults(giRESULT_CALLBACK_ELEMENT, lCount)
                End If
                .Error = vaResults(giRESULT_ERROR_ELEMENT, lCount)
                'Check what data type the data element is
                'in order to determine how to handle it
                Select Case VarType(vaResults(giRESULT_DATA_ELEMENT, lCount))
                    Case vbEmpty, vbNull
                        .Result = Null
                    Case vbObject, vbError, vbDataObject
                        Set .Result = vaResults(giRESULT_DATA_ELEMENT, lCount)
                    Case Else
                        .Result = vaResults(giRESULT_DATA_ELEMENT, lCount)
                End Select
            End With
            gcCallBack.Add oCallBkRef
            Set oCallBkRef = Nothing
        Next
        'Update Expediter U/I
        glBacklog = glBacklog + lUB + 1
        If glBacklog > glPeakBacklog Then
            glPeakBacklog = glBacklog
        End If
        If gbShow Then
            With frmExpediter
                .lblBacklog.Caption = glBacklog
                .lblPeak = glPeakBacklog
                .lblBacklog.Refresh
                .lblPeak.Refresh
            End With
        End If
    End If
    PollQueue = bReturn
    Exit Function
PollQueueError:
    Dim iRetry As Integer
    Dim il As Integer
    Dim ir As Integer
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
            'Collision error, the OLE server is busy
            'First check for stop test
            If gbStopTest Then Exit Function
            If iRetry < giRETRIES_ALLOWED_BEFORE_MOVING_ON Then
                iRetry = iRetry + 1
                ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                For il = 0 To ir
                    DoEvents
                    If gbStopTest Then Exit For
                Next il
                'Stop test may have been called during doevents loop
                If gbStopTest Then Exit Function Else Resume
            End If
        Case Else
            LogError Err, 0
    End Select
    PollQueue = bReturn
End Function

Public Sub DeliverResults()
    '-------------------------------------------------------------------------
    'Purpose:   Try to make calls to Callback objects, to deliver Service Results
    '           to the corresponding Callback objects.  After all callback are
    '           at least attempted to be called, call PollQueue to get more
    '           Service Results.  Try to make calls to all the new Callback
    '           objects.  Continue cycle until the QueueMgr does not return
    '           new Service Results.  If the cycle is broken because the QueueMgr
    '           did not return Service Results, start the timer so that it
    '           will poll the QueueMgr until ServiceResults are obtained
    'Assumes:
    '   [gcCallback]
    '           is a valid collection object
    '   [oCallBkRf.Object]
    '           has a valid Callback method
    'Effects:
    '   [gcCallback]
    '           Is decreased by one CallBkRf object every time a callback is
    '           successfully made.
    '           After polling the QueueMgr the count will increment for every
    '           received Service Result.
    '-------------------------------------------------------------------------
    Dim oCallBkRf As CallBackRef    'Object for storing Service Result data and
                                    'its callback
    Dim lCurrentIndex As Long       'Index of oCallBkRf in gcCallBack currently
                                    'being processed
    Dim sCurrentID As String         'Current Service ID being processed
                                    'used for reporting and logging errors
    Dim bResult As Boolean          'Result from Calling PollQueue
    Dim iRetry As Integer           'Number of retries made to call a specific
                                    'object using a resume statement
    On Error GoTo DeliverResultsError
    lCurrentIndex = 1
    
TryNextCallback:
    Do While lCurrentIndex <= gcCallBack.Count And Not gbStopTest
        Set oCallBkRf = gcCallBack.Item(lCurrentIndex)
        sCurrentID = oCallBkRf.ServiceID
        'Call Callback object
        LogEvent giCALLING_CALLBACK, sCurrentID
        iRetry = 0
        If oCallBkRf.UseSyncEvent Then
            oCallBkRf.SyncObject.RaiseServiceResult sCurrentID, oCallBkRf.Result, oCallBkRf.Error
        Else
            oCallBkRf.Object.CallBack sCurrentID, oCallBkRf.Result, oCallBkRf.Error
        End If
        LogEvent giCALLBACK_CALLED, sCurrentID
        'Explicitely set callback object to nothing
        Set oCallBkRf.Object = Nothing
        Set gcCallBack.Item(lCurrentIndex).Object = Nothing
        gcCallBack.Remove lCurrentIndex
        
        'Update Expediter U/I
        glBacklog = glBacklog - 1
        glTotalCallBacks = glTotalCallBacks + 1
        If gbShow Then
            With frmExpediter
                .lblBacklog.Caption = glBacklog
                .lblCount.Caption = glTotalCallBacks
                .lblBacklog.Refresh
                .lblCount.Refresh
            End With
        End If
            
        'Loop without iterating lCurrentIndex because the lCurrentIndex item
        'will be replaced by one above it after it is removed.
        'lCurrentIndex is only iterated by Error Handling, which will move
        'the process on to another callback after a few retries.
    Loop
    
    'After going through the whole gcCallBack collection
    'Poll the queuemgr trying to get more ServiceResults
    'Go back to the top of the Loop using index 1 if
    'there are items in gcCallBack after Polling the QueueMgr
    bResult = PollQueue
    lCurrentIndex = 1
    'Got to top of loop if there are any items in gcCallBack
    'Do not use the result of the PollQueue function because
    'even if the QueueMgr did not return results there may
    'be items in gcCallBack representing exhausted Callbacks
    'that need to be tried again.
    If gcCallBack.Count > 0 And Not gbStopTest Then GoTo TryNextCallback
    
    'Before exiting the function start the timer
    'so that the Expediter will keep polling the QueueMgr
    frmExpediter.tmrExpediter.Interval = giTIMER_INTERVAL
    Exit Sub

DeliverResultsError:
    Dim il As Integer
    Dim ir As Integer
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
               'Collision error, the OLE server is busy
               'First check for stop test
               If gbStopTest Then Exit Sub
               If iRetry < giRETRIES_ALLOWED_BEFORE_MOVING_ON Then
                   'Iterate the object's retry count
                   oCallBkRf.CallAttempts = oCallBkRf.CallAttempts + 1
                   'Iterate the number of try's make with Resume
                   iRetry = iRetry + 1
                   ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                   For il = 0 To ir
                       DoEvents
                   Next il
                   LogEvent giRETRY_CALLBACK, sCurrentID
                   Resume
               Else
                    'We reached our max retries either move on
                    'to the next object in the collection leaving this
                    'object to be tried again later or remove the object
                    'because this object was had too many callattempts on
                    'it specifically.
                    If oCallBkRf.CallAttempts >= giMAX_ALLOWED_RETRIES Then
                        'Give up trying to call this particulary object
                        'it will be removed at the end of Select Case block
                        'Since it is being removed do not iterate the lCurrenIndex
                        LogEvent giCALL_REJECTED_RETRIES_EXHAUSTED, sCurrentID
                        DisplayStatus LoadResString(giCALL_REJECTED_RETRIES_EXHAUSTED)
                    Else
                        'Iterate the lCurrentIndex and do not remove this
                        'object.  It will be reattempted later
                        lCurrentIndex = lCurrentIndex + 1
                        Resume TryNextCallback
                    End If
               End If
        Case ERR_OVER_FLOW
            glTotalCallBacks = 0
            LogError Err, sCurrentID
            Resume Next
        Case ERR_CALL_FAILED_DIDNOT_EXECUTE
            LogError Err, sCurrentID
        Case Else
            LogError Err, sCurrentID
    End Select
    On Error Resume Next
    'Explicitely set callback object to nothing
    Set oCallBkRf.Object = Nothing
    Set gcCallBack.Item(lCurrentIndex).Object = Nothing
    gcCallBack.Remove lCurrentIndex
    Exit Sub
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
        goLogger.Record LoadResString(giEXPEDITER_NAME), sServiceID, LoadResString(intMessage), GetTickCount()
    End If
    'If the form is visible display log on form
    #If ccShowList Then
        DisplayString sServiceID & gsSEPERATOR & LoadResString(intMessage)
    #End If
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
                   LogError Err, sServiceID
                   Exit Sub
               End If
        Case Else
            LogError Err, sServiceID
            Exit Sub
    End Select
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
            frmExpediter.Show
            gbShow = True
        End If
        DisplayString s
    #Else
        If Err.Number <> 0 Then DisplayStatus oErr.Description
    #End If
    If gbLog And glInstances <> 0 Then
        goLogger.Record LoadResString(giEXPEDITER_NAME), sServiceID, s, GetTickCount()
    End If
    Exit Sub
End Sub

Sub DisplayStatus(s As String)
    '-------------------------------------------------------------------------
    'Purpose:   If gbShow is true, displays passed string on forms status box
    'Assumes:   If gbShow is true, form is loaded and visible
    '-------------------------------------------------------------------------
    If gbShow Then AlignTextToBottom frmExpediter.lblStatus, s
End Sub

Sub DisplayString(sText As String)
    '-------------------------------------------------------------------------
    'Purpose:   Adds the passed text to to the list box.  Only used if conditional
    '           compile ccShowList is true.
    'Assumes:   If gbShow is true, form is visible
    '           If ccShowList is true, lstLog is visible and positioned
    '-------------------------------------------------------------------------
    'Controls the length of the list box
    'and adds items to the top
    #If ccShowList Then
        Dim lstLog As ListBox
        If gbShow Then
            Set lstLog = frmExpediter.lstLog
            If lstLog.ListCount = glLIST_BOX_MAX Then lstLog.Clear
            lstLog.AddItem sText, 0
            DoEvents
        End If
    #End If
End Sub

Sub DestroyReferences()
    '-------------------------------------------------------------------------
    'Purpose:   Called by in the event of a StopTest call
    '           to destroy callback objects
    '-------------------------------------------------------------------------
    
    Dim oCallback As CallBackRef
    LogEvent giSTOP_TEST_RECEIVED, 0
    frmExpediter.tmrExpediter.Interval = 0
    For Each oCallback In gcCallBack
        Set oCallback.Object = Nothing
    Next
    Set gcCallBack = Nothing
    Set gcCallBack = New Collection
    Set goQueueDelegator = Nothing
    If gbUnloading Then
        If gbLog Then Set goLogger = Nothing
        Unload frmExpediter
    End If
End Sub
