VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "Expediter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Expediter"
Option Explicit
'-------------------------------------------------------------------------
'The Class is the only public class in this project.  See notes in
'modExpediter for purpose.
' It implements the IExpediter interface.
'-------------------------------------------------------------------------

Implements APEInterfaces.IExpediter

'***********************
'Public Properties
'***********************

Public Property Set IExpediter_QueueMgrRef(ByVal oQueueMgr As APEInterfaces.IQueueDelegator)
Attribute IExpediter_QueueMgrRef.VB_Description = "Sets the QueueDelegator object that the Expediter uses to receive Service Request results from the AEQueueMgr."
    '-------------------------------------------------------------------------
    'Purpose:   Called by the the QueueMgr to pass a reference of itself to
    '           the Expediter
    'In:        [oQueueMgr]
    '               A valid reference to a QueueMgr class object
    'Effects:   [goQueueDelegator]
    '               Sets the global object variable equal to the passed reference
    '-------------------------------------------------------------------------
    Set goQueueDelegator = oQueueMgr
    
End Property

Public Property Let IExpediter_Show(ByVal bShow As Boolean)
Attribute IExpediter_Show.VB_Description = "Determines whether the Expediter shows a form."
    '-------------------------------------------------------------------------
    'Purpose:   Show property determines whether or not a form
    '           is displayed while expediter is loaded
    'Effects:   [gbShow] becomes value of parameter
    '           If parameter is true frmExpediter is show, else form
    '           is hidden.  Form is never unloaded because the timer is needed
    '-------------------------------------------------------------------------
    
    If Not gbShow = bShow Then
        gbShow = bShow
        If bShow = True Then
            frmExpediter.Show
            'Update U/I values
            With frmExpediter
                .lblBacklog.Caption = glBacklog
                .lblPeak = glPeakBacklog
                .lblBacklog.Refresh
                .lblPeak.Refresh
            End With
        Else
            'Never Unload form because it has a timer
            frmExpediter.Hide
        End If
    End If
    
End Property

Public Property Get IExpediter_Show() As Boolean
    IExpediter_Show = gbShow
End Property

Public Property Let IExpediter_Log(ByVal bLog As Boolean)
Attribute IExpediter_Log.VB_Description = " Determines if the Expediter logs its events and errors to the AELogger.Logger object."
    '-------------------------------------------------------------------------
    'Purpose:   If log is true create logger class object and log Services
    'Effects:   [gbLog] becomes value of parameter
    '           [goLogger] is set to a new AELogger.Logger object if parameter
    '                      is true.  If false goLogger is destroyed
    '-------------------------------------------------------------------------
    
    If Not gbLog = bLog Then
        gbLog = bLog
        If bLog = True Then
            Set goLogger = CreateObject("AELogger.Logger")
        Else
            Set goLogger = Nothing
        End If
     End If
End Property

Public Property Get IExpediter_Log() As Boolean
    IExpediter_Log = gbLog
End Property

'*****************
'Public Methods
'*****************

Public Sub IExpediter_SetProperties(ByVal bShow As Boolean, Optional ByVal bLog As Variant)
Attribute IExpediter_SetProperties.VB_Description = "Sets properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   To set the Logger properties in one method call
    'Effects:   Sets the following properties to parameter values
    '           Show, Log
    '-------------------------------------------------------------------------
    With Me
        .IExpediter_Show = bShow
        If Not IsMissing(bLog) Then .IExpediter_Log = bLog
    End With
End Sub

Public Sub IExpediter_StopTest()
Attribute IExpediter_StopTest.VB_Description = "Causes the Expediter to stop processing Service Request results and to empty its queue."
    '-------------------------------------------------------------------------
    'Purpose:   Call this to halt the Expediter and have its
    '           collection of Service requests and their
    '           respective CallBack objects removed
    'Effects:
    '           DestroyReferences may be called
    '       [gbStopTest]
    '           becomes true
    '-------------------------------------------------------------------------
    
    gbStopTest = True
    If Not gbBusy Then DestroyReferences
End Sub

Public Sub IExpediter_StartTest()
Attribute IExpediter_StartTest.VB_Description = "Prepares the Expediter to process Service Request results after StopTest has been called."
    '-------------------------------------------------------------------------
    'Purpose:   Call this to allow processing of Services
    '           after calling StopTest
    'Effects:
    '           Reinitialize values on U/I
    '       [gcCallback]
    '           Make sure it is a zero count collection
    '       [gbStopTest]
    '           becomes false
    '       [frmExpediter.tmrExpediter]
    '           becomes enabled
    '-------------------------------------------------------------------------
    glPeakBacklog = 0
    glBacklog = 0
    glTotalCallBacks = 0
    With frmExpediter
        .lblPeak = 0
        .lblCount = 0
        .lblBacklog = 0
        .lblPeak.Refresh
        .lblCount.Refresh
        .lblBacklog.Refresh
    End With
    gbStopTest = False
    DisplayStatus ""
    Set gcCallBack = Nothing
    Set gcCallBack = New Collection
    If Not goQueueDelegator Is Nothing Then frmExpediter.tmrExpediter.Interval = giTIMER_INTERVAL
End Sub

Public Function IExpediter_GetEventObject() As Object
    Set IExpediter_GetEventObject = New EventReturn
End Function

'********************
'Private Procedures
'********************

Private Sub Class_Initialize()
    '-------------------------------------------------------------------------
    'Purpose:   If this is the first instance, initialize the whole application
    '           Set defaults and create needed objects
    'Effects:
    '       [glInstances]
    '           iterated once to count instances
    '-------------------------------------------------------------------------
    'Count how many times this class is instanced
    'to react to the first instance or the release
    'of the last instance.
    On Error GoTo Class_InitializeError
    glInstances = glInstances + 1
    If glInstances = 1 Then
        App.OleServerBusyRaiseError = True
        App.OleServerBusyTimeout = 10000
        gbUnloading = False
        'Set default property values
        'Create Logger class object if gbLog is true
        If gbLog Then Set goLogger = CreateObject("AELogger.Logger")
        gbShow = gbSHOW_FORM_DEFAULT
        gbLog = gbLOG_DEFAULT
        'Create gcCallBack collection
        Set gcCallBack = New Collection
        'Load frmExpediter because it has a timer
        Load frmExpediter
        'Only show the form if gbShow is true
        If gbShow Then frmExpediter.Show
    End If
    Exit Sub
Class_InitializeError:
    LogError Err, 0
    Resume Next
End Sub

Private Sub Class_Terminate()
    '-------------------------------------------------------------------------
    'Purpose:   If this is the last termination unload form and destroy objects
    'Effects:
    '       [glInstances]
    '           decrease once to count instances
    '-------------------------------------------------------------------------
    'Count how many times this class is instanced
    'so subtract one every terminate event
    'If the last terminate event is occuring
    'make sure forms are unloaded and objects
    'are released
    On Error GoTo Class_TerminateError
    glInstances = glInstances - 1
    If glInstances = 0 Then
        gbUnloading = True
        IExpediter_StopTest
    End If
    Exit Sub
Class_TerminateError:
    LogError Err, 0
    Resume Next
End Sub



