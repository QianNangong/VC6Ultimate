VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsQueueDelegator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "Provides an interface for the AEExpediter and the AEWorker to interact with the AEQueueMgr."
Option Explicit
'-------------------------------------------------------------------------
'The Class is public but not creatable.  It is provide as an OLE interface
'for the Expediter and Workers to call.  The Worker calls the GetServiceRequest Method
'to return Service results and retrieve a new Service Request.
'-------------------------------------------------------------------------

Implements APEInterfaces.IQueueDelegator


Private Function IQueueDelegator_GetServiceRequest(ByVal lWorkerID As Long, Optional ByVal sReturnServiceID As String, Optional ByVal vReturnData As Variant, Optional ByVal sReturnError As String = "") As Variant
    '-------------------------------------------------------------------------
    'Purpose:   Worker should call this method to poll for a
    '           Service Request to satisfy.
    'IN:
    '   [lWorkerID]
    '           Worker's ID, it is the same as its key to the gcWorkers collection
    'Optional IN:
    '           The following optional parameters allow a Worker to return
    '           the results of a service request at the same time it is
    '           calling for a new service to accomplish.
    '   [sReturnServiceID]
    '           Service Request ID of Service whose results are being returned
    '           Store the returned results so the Expediter can get them
    '           and return them to the client application
    '   [vReturnData]
    '           Return data from the accomplished service or task.  Unknown
    '           data type.  Just store it and Expediter will get it to pass
    '           back to client application
    '   [sReturnError]
    '           String that contains error information that occured during
    '           service competion.  Expediter will get this to pass back
    '           to client application.
    'Return:    Is a variant array with Service Request data if the QueueMgr
    '           has a Service Request for it to satify.  Otherwise, a Null
    '           is returned.  The Service ID, the Command string, UseCallback
    '           flag, Data Present flag, and ServiceData are passed in the
    '           returned array
    'Effects:
    '   [gbBusyGetServiceRequest]
    '           is true during procedure
    '   [gcQueue]
    '           The Service Request item, a clsService class object, in the
    '           collection will have its status property changed to giDELEGATED_TO_WORKER
    '           if it is returned to the Worker
    '           Another Service Request item in the collection whose results
    '           are being returned may have it status property changed to
    '           giHAVE_SERVICE_RESULTS
    '   [gcWorkers]
    '           An item's Busy flag that corresponds with calling Worker is
    '           flipped to false if no Service Request is returned.  If a
    '           Service Request is returned it is set to true.
    'Assumes:
    '   [gcWorkers]
    '           Is a collection of valid AEWorker.Worker objects
    '   [Calling Object]
    '           Is a Worker in the gcWorkers collection and is passing an
    '           ID that matches the key to it in the gcWorkers collection
    '   [gcQueue]
    '           Is a collection of clsService objects
    '-------------------------------------------------------------------------
    'First check to see if there is an Service request in the queue
    'Pass back a variant array to the Worker if there is another Service
    Dim oService As clsService  'Will be the clsService object to store the
                                'passed results in and then it will be the
                                'clsService object to retrieve Service Request
                                'Data from and pass back to worker
    Dim sKeyToRemove As String  'Key of clsService object in gcQueue to remove
    Dim oa As clsService        'clsService object used in For...Next loop
    Dim bGotService As Boolean  'Flag meaing Service Request is chosen to pass back
    Dim vServiceData(3) As Variant  'Array that will contain Service request data
                                    'to be returned to Worker
    Dim lCount As Long              'Count of items in gcQueue
    Dim l As Long                   'For...Next counter
    Dim oWork As clsWorker      'clsWorker object that contains a reference to the
                                'calling Worker object
    On Error GoTo QueueDelegator_GetServiceRequestError
    
    gbBusyGetServiceRequest = True
    
    'Get the clsWorker object that contains the Worker that is calling
    Set oWork = gcWorkers.Item(CStr(lWorkerID))
    
    'See if Service Request results were returned.  If they were
    'store the Service Request results in gcQueue in the clsService
    'object if the objects UseCallback property is true.  If it is
    'false, ignore any results and remove item from queue now.
    If IsNumeric(sReturnServiceID) And (Not gbStopTest) Then
        'We have a return, now see if results should be stored
        'for expediter to get and return to client application
        Set oService = gcQueue.Item(sReturnServiceID)
        With oService
            Select Case .CallBackMode
                Case giUSE_DEFAULT_CALLBACK, giUSE_PASSED_CALLBACK, giRETURN_BY_SYNC_EVENT
                    'store values and change status flag
                    LogEvent giGETREQUEST_RECEIVED_RETURNED_RESULTS, sReturnServiceID
                    .Status = giHAVE_SERVICE_RESULTS
                    .ReturnError = sReturnError
                    If Not IsMissing(vReturnData) Then
                        'Check what data type vReturnData is
                        'in order to determine how to handle it
                        Select Case VarType(vReturnData)
                            Case vbEmpty, vbNull
                                .ReturnData = Null
                            Case vbObject, vbError, vbDataObject
                                Set .ReturnData = vReturnData
                            Case Else
                                .ReturnData = vReturnData
                        End Select
                    End If
                    gbHaveServiceResults = True
                Case Else
                    'if a callback is not to be returned just
                    'remove the clsService object from gcQueue
                    gcQueue.Remove sReturnServiceID
            End Select
        End With
        Set oService = Nothing
    End If
    
    'Exit sub if Stopping Queue
    If gbStopTest Then
        GoTo NoServiceToReturn
    End If
   
    'See if the calling Worker is Marked for removal.  If it is
    'return the integer value giCLOSE_WORKER_NOW instead of returning
    'a Service request.  Also, remove the clsWorker object from
    'gcWorkers so that when the local reference to it (oWork)
    'goes out of scope the Worker will unload
    lCount = gcQueue.Count

    If oWork.RemoveMe Then
        gcWorkers.Remove CStr(lWorkerID)
        'Update worker count U/I
        If gbShow Then
            With frmQueueMgr.lblWorkerCount
                .Caption = gcWorkers.Count
                .Refresh
            End With
        End If
        IQueueDelegator_GetServiceRequest = giCLOSE_WORKER_NOW
        Exit Function
    Else
        If lCount > 0 Then
            'Pass another Service throught the parameters passed ByRef
            'It seems that this procedure or the Delegate procedure is dropped into
            'using the same oService in gcQueue so Status flag is
            'added so it can be flipped immediately
            bGotService = False
            'Use For...Next instead of For...Each to make sure that
            'correct priority is given to items in the collection
            For l = 1 To gcQueue.Count
                'If an item is removed during this loop by another process
                'an Invalid Procedure call error will be produced if
                'try to reference a object that no longer exists
                On Error Resume Next
                Set oa = gcQueue(l)
                If Err.Number = ERR_INVALID_PROCEDURE_CALL Then
                    On Error GoTo QueueDelegator_GetServiceRequestError
                    Exit For
                End If
                On Error GoTo QueueDelegator_GetServiceRequestError
                If oa.Status = giWAITING_FOR_WORKER Then
                    oa.Status = giDELEGATED_TO_WORKER
                    sKeyToRemove = CStr(oa.ID)
                    Set oService = oa
                    bGotService = True
                    Exit For
                End If
            Next
            If Not bGotService Then
                'event though gcQueue.Count is greater than
                'zero all the items are already delgated so
                'Mark the worker as not busy and exit
                GoTo NoServiceToReturn
            End If
            
            'Fill the variant array to be returned
            With oService
                LogEvent giGETREQUEST_RECEIVED_NEW_SERVICE, .ID
                vServiceData(giSERVICE_ID_ELEMENT) = .ID
                vServiceData(giCOMMAND_ELEMENT) = .Command
                vServiceData(giDATA_PRESENT_ELEMENT) = .DataPresent
                If .DataPresent Then
                    'Check what data type vService return is
                    'in order to determine how to handle it
                    Select Case VarType(.Data)
                        Case vbEmpty, vbNull
                            vServiceData(giSERVICE_DATA_ELEMENT) = Null
                        Case vbObject, vbError, vbDataObject
                            Set vServiceData(giSERVICE_DATA_ELEMENT) = .Data
                        Case Else
                            vServiceData(giSERVICE_DATA_ELEMENT) = .Data
                    End Select
                End If
            End With
            Set oService = Nothing
            
            IQueueDelegator_GetServiceRequest = vServiceData()
            On Error GoTo QueueDelegator_GetServiceRequestError
        Else
NoServiceToReturn:
            'If there is not pending Service request
            'mark Busy equal false in the clsWorker class
            'object that has a reference to the Worker
            'calling the GetServiceRequest method.
            If gbShow Then frmQueueMgr.lblQueue = 0
            oWork.Busy = False
            IQueueDelegator_GetServiceRequest = Null
        End If
    End If
    
    'Display stats
    If gbShow Then frmQueueMgr.lblQueue = lCount
    If lCount > glPeakQueueSize Then
        glPeakQueueSize = lCount
        If gbShow Then frmQueueMgr.lblPeak = glPeakQueueSize
    End If
    
    gbBusyGetServiceRequest = False
    If gbStopTest And Not gbBusyAdding And Not gbBusyGetServiceResults Then StopQueue
    Exit Function
QueueDelegator_GetServiceRequestError:
    LogError Err, gsNULL_SERVICE_ID
    Err.Raise Err.Number, Err.Source, Err.Description
    Exit Function

End Function

Private Function IQueueDelegator_GetServiceResults() As Variant
    '-------------------------------------------------------------------------
    'Purpose:   This method is provided for the Expediter to call and retrieve
    '           all completed Service Request results and there respective
    '           callback objects
    'Return:    Is a variant array with Service Results if the QueueMgr
    '           has completed Service Results for it to satify.  Otherwise, a Null
    '           is returned.  The Service ID, the Data to be returned, the Callback
    '           object, and the Error description string are returned with in
    '           The variant array for each Service Result returned.  The array
    '           will have two dimensions.  The first dimension will have an
    '           index to represent each data element of the Service Results --
    '           see modAEConstants for the index constants--the second dimension
    '           will have an index for each Service Result
    'Effects:
    '   [gbBusyGetServiceResults]
    '           Is true during this procedure
    '   [gcQueue]
    '           Any clsService object with its Status property equaling
    '           giHAVE_SERVICE_RESULTS will be removed.
    '-------------------------------------------------------------------------
    Dim vaResults As Variant        'Variant array to be returned to Expediter
    Dim lResultCount As Long        'Count of results added to Result array
    Dim oService As clsService      'Object for For...Each loop
    Dim lUB As Long                 'Upper Bound of the 2nd dimension of vaResults
    
    gbBusyGetServiceResults = True
    
    'Check the gbHaveServiceResults flag so we don't check ever
    'clsService object in gcQueue if we know that there are no
    'ready Service Results
    If gbHaveServiceResults Then
        gbHaveServiceResults = False
        
        ReDim vaResults(giRESULT_DIMENSION_ONE, giRESULT_ARRAY_REDIM_CHUNK_SIZE)
        lUB = giRESULT_ARRAY_REDIM_CHUNK_SIZE
        
        'Check if any clsService objects in gcQueue are ready to be returned
        For Each oService In gcQueue
            With oService
                If oService.Status = giHAVE_SERVICE_RESULTS Then
                    'Put the data of this clsService object in
                    'the array then remove the object from the collection
                    'See if vaResults needs redimensioned
                    If lResultCount > lUB Then
                        lUB = lUB + giRESULT_ARRAY_REDIM_CHUNK_SIZE
                        ReDim Preserve vaResults(giRESULT_DIMENSION_ONE, lUB)
                    End If
                    
                    'Get values
                    vaResults(giRESULT_ID_ELEMENT, lResultCount) = .ID
                    vaResults(giRESULT_CALLBACK_TYPE_ELEMENT, lResultCount) = .CallBackMode
                    Select Case .CallBackMode
                        Case giUSE_PASSED_CALLBACK, giUSE_DEFAULT_CALLBACK
                            Set vaResults(giRESULT_CALLBACK_ELEMENT, lResultCount) = .CallBack
                        Case giRETURN_BY_SYNC_EVENT
                            Set vaResults(giRESULT_CALLBACK_ELEMENT, lResultCount) = .EventObject
                    End Select
                    vaResults(giRESULT_ERROR_ELEMENT, lResultCount) = .ReturnError
                    'Check what data type .ReturnData is
                    'in order to determine how to handle it
                    Select Case VarType(.ReturnData)
                        Case vbEmpty, vbNull
                            vaResults(giRESULT_DATA_ELEMENT, lResultCount) = Null
                        Case vbObject, vbError, vbDataObject
                            Set vaResults(giRESULT_DATA_ELEMENT, lResultCount) = .ReturnData
                        Case Else
                            vaResults(giRESULT_DATA_ELEMENT, lResultCount) = .ReturnData
                    End Select
                    
                    'Remove the current clsService object from gcQueue
                    gcQueue.Remove CStr(.ID)
                    lResultCount = lResultCount + 1
                    'exit the loop if the array has reached the max size
                    'the rest of the results will be returned on another call
                    If lResultCount - 1 = giRESULT_ARRAY_MAX_SIZE Then
                        gbHaveServiceResults = True     ' Make sure the remaining items are processed
                        Exit For
                    End If
                End If
            End With
        Next
        
        'Check if any results were put in the array
        'If they were redimension the array to trim of indexes that do not have
        'data in them and return the array as the result of this function
        'If no results were put in the array return null
        If lResultCount >= 1 Then
            LogEvent giGETRESULTS_RECEIVED_RETURNED_RESULTS, gsNULL_SERVICE_ID
            ReDim Preserve vaResults(giRESULT_DIMENSION_ONE, lResultCount - 1)
            IQueueDelegator_GetServiceResults = vaResults
        Else
            IQueueDelegator_GetServiceResults = Null
        End If
    End If
    
    'Display stats
    If gbShow Then frmQueueMgr.lblQueue = gcQueue.Count
    
    If gbStopTest And Not gbBusyGetServiceRequest And Not gbBusyAdding Then StopQueue
    gbBusyGetServiceResults = False
End Function
