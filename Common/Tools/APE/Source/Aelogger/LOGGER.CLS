VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "Logger"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Logger"
Option Explicit
'-------------------------------------------------------------------------
'This is the only public class in this application.  See modLogger for
'purpose.
' This class implements the ILogger interface.
'-------------------------------------------------------------------------

Implements APEInterfaces.ILogger

Public Property Let ILogger_Show(ByVal bShow As Boolean)
Attribute ILogger_Show.VB_Description = "Determines whether the Logger shows a form."
    '-------------------------------------------------------------------------
    'Purpose:   Show property determines whether or not a form is displayed
    '           while the logger is loaded.
    '
    'Effects:   [gbShowForm]
    '               Becomes equal to the passed parameter
    '           [frmLogger]
    '               Becomes loaded and visible if parameter is true, but is
    '               unloaded if parameter is false
    '-------------------------------------------------------------------------
    If Not gbShowForm = bShow Then
        gbShowForm = bShow
        If bShow = True Then
            frmLogger.Show
        Else
            Unload frmLogger
        End If
    End If
End Property

Public Property Get ILogger_Show() As Boolean
    ILogger_Show = gbShowForm
End Property

Public Property Let ILogger_AutomaticWrite(ByVal bWrite As Boolean)
Attribute ILogger_AutomaticWrite.VB_Description = "Determines whether log records are written to a file and purged from memory when the log threshold is reached."
    '-------------------------------------------------------------------------
    'Purpose:   AutomaticWrite property determines if the Logger should
    '           automatically write to a file when a record threshold is met.
    'Effects:   [gbWriteRecords]
    '               Becomes equal to the passed parameter
    '-------------------------------------------------------------------------
    gbWriteRecords = bWrite
End Property

Public Property Get ILogger_AutomaticWrite() As Boolean
    ILogger_AutomaticWrite = gbWriteRecords
End Property

Public Property Let ILogger_Threshold(ByVal lThreshold As Long)
Attribute ILogger_Threshold.VB_Description = "Sets the log threshold in kilobytes that determines when log records are written to a file and purged from memory."
    '-------------------------------------------------------------------------
    'Purpose:   If AutomaticWrite property is true, logger uses the
    '           Threshold property to determine how many kilobytes should
    '           be held in memory before writing to a file and emptying
    '           log record array.
    'Effects:   [glThreshold]
    '               Becomes equal to the passed parameter
    '           [glThresholdRecs]
    '               Becomes an estimated number of records equivalent
    '-------------------------------------------------------------------------
    On Error Resume Next
    glThreshold = lThreshold
    glThresholdRecs = lThreshold * giLOG_RECORD_KILOBYTES
End Property

Public Property Get ILogger_Threshold() As Long
    ILogger_Threshold = glThreshold
End Property

'************************
'Public Methods
'************************

Public Sub ILogger_SetProperties(ByVal bShow As Boolean, Optional ByVal bAutomaticWrite As Variant, Optional ByVal lThreshold As Variant)
Attribute ILogger_SetProperties.VB_Description = "Sets all Logger properties in one method call."
    '-------------------------------------------------------------------------
    'Purpose:   Provided so that properties can be set by one method call
    'Effects:   Sets the following properties:
    '           Show, AutomaticWrite, Threshold
    '-------------------------------------------------------------------------
    Me.ILogger_Show = bShow
    If Not IsMissing(bAutomaticWrite) Then gbWriteRecords = bAutomaticWrite
    If Not IsMissing(lThreshold) Then Me.ILogger_Threshold = lThreshold
End Sub

Public Sub ILogger_Record(ByVal sComponent As String, ByVal sServiceID As String, ByVal sComment As String, ByVal lMilliseconds As Long)
Attribute ILogger_Record.VB_Description = "Adds a log record."
    '-------------------------------------------------------------------------
    'Purpose:   Provided for any app to call to add one log record
    'Effects:   Calls AddLogRecord
    '           Calls WriteRecords when the Threshold is reached
    '-------------------------------------------------------------------------
    AddLogRecord sComponent, sServiceID, sComment, lMilliseconds
    If gbWriteRecords Then
        If glLastAddedRecord >= glThresholdRecs And glThresholdRecs > 0 Then
            WriteRecords
        End If
    End If
End Sub

Public Function ILogger_GetRecords() As Variant
Attribute ILogger_GetRecords.VB_Description = "Returns a variant array containing log records.  Must be called multiple times until until Null is returned."
    '-------------------------------------------------------------------------
    'Purpose:   Use to retrieve all of the log records passed to the Logger
    '           Keep calling until, it returns does not return a variant array
    'Return:    Returns a two dimension array in which
    '           the first four elements of the first dimension
    '           are Component(string), ServiceID(Long),Comment(string),
    '           and Milliseconds(long) respectively
    '           the second dimension represents the number of log records
    '           User Defined Types can not be returned from public
    '           procedures of public classes
    'Effects:   [gaRecords]
    '               Redimensioned after calling GetRecords to not have empty
    '               records at the end
    '           [glLastAddedRecord]
    '               becomes equal to giNO_RECORDS
    '-------------------------------------------------------------------------
    
    GetWrittenLog
    'Trim the array to only send the filled elements
    If glLastAddedRecord >= 0 Then
        If UBound(gaRecords, 2) <> glLastAddedRecord Then ReDim Preserve gaRecords(giLOG_ARRAY_DIMENSION_ONE, glLastAddedRecord)
        ILogger_GetRecords = gaRecords()
        'Changing the glLastAddedRecord flag to giNO_RECORDS causes
        'WriteRecords to ignore records at next call
        glLastAddedRecord = giNO_RECORDS
    Else
        ILogger_GetRecords = Null
    End If
End Function

'*******************
'Private Procedures
'*******************

Private Sub Class_Initialize()
    '-------------------------------------------------------------------------
    'Purpose:   Set the initial state of the logger when the first logger
    '           class object is initialized
    'Effects:   [glInstances]
    '               Iterates it once
    '-------------------------------------------------------------------------
    'Count how many times this class is instanced
    'to react to the first instance or the release
    'of the last instance.
    glInstances = glInstances + 1
    If glInstances = 1 Then
       'Set default property values
       gbShowForm = gbSHOW_FORM_DEFAULT
       gbWriteRecords = gbWRITE_RECORDS_DEFAULT
       Me.ILogger_Threshold = gbTHRESHOLD_DEFAULT
       gsFileName = GetTempFile
       glLastAddedRecord = giNO_RECORDS
       'Load frmLogger if gbShowForm is True
       If gbShowForm Then frmLogger.Show
    End If
End Sub

Private Sub Class_Terminate()
    '-------------------------------------------------------------------------
    'Purpose:   Closes the form and destroys the tempfile when the last
    '           instance is terminated
    'Effects:   [glInstances]
    '               decreases by one
    '-------------------------------------------------------------------------
    'Count how many times this class is instanced
    'so subtract one every terminate event
    'If the last terminate event is occuring
    'make sure forms are unloaded and write records
    On Error GoTo Class_TerminateError
    glInstances = glInstances - 1
    If glInstances = 0 Then
        Unload frmLogger
        Close   'Close here incase getting logs got canceled
        Kill gsFileName
    End If
    Exit Sub
Class_TerminateError:
    Select Case Err.Number
        Case ERR_FILE_NOT_FOUND
            'There is no file to kill
            Resume Next
        Case Else
            Resume Next
    End Select
End Sub
